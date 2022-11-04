/*
 * memory.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_CORE_MEMORY_H_
#define ATTO_CORE_MEMORY_H_

#include <cstdlib>
#include <cstring>
#include <memory>
#include "atto/core/error.hpp"

namespace atto {
namespace core {

/** ---- Memory aligned allocator ---------------------------------------------
 * align_alloc
 * @brief Allocate a memory block with size bytes on a boundary specified by
 * the alignment argument.
 *
 * @throw
 *  If block size is invalid.
 *  If alignment is not a power of 2.
 *  If posix_memalign fails to allocate.
 *
 * @note
 *  The alignment must be a power of two and a multiple of sizeof(void *).
 *  Given n > 1:
 *
 *  If n is a power of 2, then n takes the form, e.g., 01000000.
 *  Subtract 1 from n,
 *      (n-1) = 01000000 - 00000001 = 00111111.
 *  Bitwise n & (n-1) == 0
 *      (00111111) & (01000000) = (00000000).
 *
 *  Any n not a power of 2, will have an extra 1, e.g., 01000010.
 *  Subtract 1 from n,
 *      (n-1) = 01000010 - 00000001 = 01000001.
 *  Bitwise n & (n-1) != 0
 *      (01000001) & (01000010) = (01000000).
 * @note
 * align_alloc and align_free are paired functions: an object created
 * with align_alloc should be freed with align_free.
 */
core_inline
void *align_alloc(size_t size, size_t alignment = 32)
{
    /*
     * Check size is valid and alignment is power of 2.
     */
    core_assert(size > 0, "invalid size");
    core_assert(alignment > 0 && !((alignment-1) & alignment),
        "alignment value is not a power of 2");

    /*
     * Allocate block and check for errors.
     */
    void *ptr = nullptr;
    int ret = posix_memalign(&ptr, alignment, size);

    core_assert(ret != EINVAL,
        "alignment value is not a power of 2 nor multiple of sizeof(void *)");
    core_assert(ret != ENOMEM,
        "insufficient memory available with the requested alignment");
    core_assert(ptr != nullptr, "failed to allocate");

    /*
     * Ensure the memory block is initialized before returning the ptr.
     */
    std::memset(ptr, 0, size);
    return ptr;
}

/**
 * align_free
 * @brief Free a memory block.
 *
 * @note Ensure ptr is set to null after deallocation.
 *  If ptr is not null and refers to a space that has been deallocated
 *  by a call to align_free or align_realloc, the result of align_free
 *  is undefined.
 * @note align_alloc and align_free are paired functions: any object
 * created with align_alloc should be freed with align_free.
 */
core_inline
void align_free (void *ptr)
{
    if (ptr == nullptr) {
        return;
    }
    std::free(ptr);
}

/**
 * align_realloc
 * @brief Reallocate an array of size(oldsize = oldelms*size) bytes to a
 * new array of size(newsize = newelms*size) bytes on a boundary specified
 * by the default alignment set by the L1 cache line size.
 *
 * The address of a block returned by malloc or realloc in 32-bit systems
 * is always a multiple of eight(or a multiple of sixteen on 64-bit systems).
 *
 * The current default L1 cache line size is 32-bit. The function reimplements
 * the realloc specification to the new boundary size.
 *
 * @note Summary of the relevant standard requirements for realloc:
 * - The contents of the object shall remain unchanged up to the lesser
 *   of the new and old sizes. If the new block size requires movement
 *   of the object, the space for the previous instantiation of the
 *   object is freed.
 *
 * - If the new block size is smaller(larger) than the old block size,
 *   align_realloc will allocate a new block of the appriate size,
 *   copy the contents of the original block up to the new(old) size,
 *   free the original block and return a pointer to the newly created
 *   block.
 *   If the new block size is larger, the contents of the newly allocated
 *   portion are initialised to 0 as set by align_alloc.
 *
 * - If the input pointer is null, align_realloc behaves exactly as if
 *   align_alloc has been called.
 *
 * - If the newsize is 0 and ptr is not a null pointer, align_realloc
 *   behaves exactly as if align_free has been called and return a null
 *   value.
 *
 * - If the space cannot be allocated, the object shall remain unchanged.
 *   If the input pointer is null and the requested size is 0, then the
 *   result is undefined.
 */
core_inline
void *align_realloc (void *ptr, size_t oldsize, size_t newsize)
{
    /*
     * New size is 0 - free the ptr and return null. Align free
     * a null pointer is a noop, otherwise the memory is freed.
     */
    if (newsize == 0) {
        align_free(ptr);
        return nullptr;
    }

    /* If ptr is a null pointer, return a newly allocated block */
    if (ptr == nullptr) {
        return align_alloc(newsize);
    }

    /*
     * Otherwise, create a new block and copy the contents of the
     * original block up to the lesser of the new and old sizes.
     */
    size_t size = newsize > oldsize ? oldsize : newsize;
    void *mem = align_alloc(newsize);
    std::memcpy(mem, ptr, size);

    /*
     * Free the original block and return the newly created block.
     */
    align_free(ptr);
    return mem;
}

/** ---- Memory templated aligned object allocator ----------------------------
 *
 * align_alloc<T>
 * @brief Allocate a block of memory of with a single object of type T, with
 * size sizeof(T) bytes and alignment given by the L1 cache line size.
 * Initialize the object by calling directly the T constructor at the newly
 * allocated address with placement new.
 *
 * @note align_alloc<T> and align_free<T> are paired functions.
 */
template<typename T, typename... Args>
T *align_alloc(Args&&... args)
{
    T *ptr = nullptr;
    try {
        ptr = (T *) align_alloc(sizeof(T));
        ::new(static_cast<void *>(ptr)) T(static_cast<Args&&>(args)...);
    } catch (std::exception& e) {
        align_free(ptr);
        core_throw(e.what());
    }
    return ptr;
}

/**
 * align_free<T>
 * @brief Delete a block of memory allocated by align_alloc<T> by manually
 * calling the T destructor for the object and freeing the block.
 *
 * @note align_alloc<T> and align_free<T> are paired functions.
 */
template<typename T>
void align_free(T *ptr)
{
    if (ptr == nullptr) {
        return;
    }

    /* Call the destructor for the object and free the memory block. */
    if (std::is_destructible<T>::value) {
        (*ptr).~T();
    }
    align_free((void *) ptr);
}

/** ---- Memory templated aligned array allocator -----------------------------
 *
 * align_alloc_array<T>
 * @brief Allocate a block of memory with count objects of type T, with size
 * (count*sizeof(T)) bytes, and alignment specified by the L1 cache line size.
 * Initialize each element in the array by calling directly the T constructor
 * at the newly allocated addresses with placement new.
 *
 * @note align_alloc_array<T> and align_free_array<T> are paired functions.
 */
template<typename T, typename... Args>
T *align_alloc_array(size_t count, Args&&... args)
{
    /*
     * Allocate a block with(count*sizeof(T)) bytes.
     */
    T *ptr = nullptr;
    try {
        ptr = (T *) align_alloc(count*sizeof(T));
    } catch (std::exception& e) {
        align_free(ptr);
        core_throw(e.what());
    }

    /*
     * Construct the objects in place at each memory address.
     */
    size_t i = 0;
    try {
        for (; i < count; ++i) {
            ::new(static_cast<void*>(ptr + i)) T(static_cast<Args&&>(args)...);
        }
    } catch (std::exception& e) {
        for (size_t j = 0; j < i; ++j) {
           ptr[j].~T();
        }
        core_throw(e.what());
    }

    return ptr;
}

/**
 * align_free_array<T>
 * @brief Delete a block of memory with count objects of type T, allocated
 * by align_alloc<T> by manually calling the T destructor for each object
 * in the array before freeing the block.
 *
 * @note align_alloc_array<T> and align_free_array<T> are paired functions.
 */
template<typename T>
void align_free_array(T *ptr, size_t count)
{
    if (ptr == nullptr) {
        return;
    }

    /* Call the destructor for each object */
    if (std::is_destructible<T>::value) {
        for (size_t i = 0; i < count; ++i) {
            ptr[i].~T();
        }
    }

    /* Free the memory block */
    align_free((void *) ptr);
}

/** ---- Memory aligned unique pointer ----------------------------------------
 * align_unique_ptr<T>
 * @brief align_unique_ptr is a template alias of unique_ptr with a custom
 * deleter, given by the default align_unique_ptr_deleter.
 *
 * It contains a pointer to an object of type T stored in a aligned block
 * of memory with sizeof(T) bytes.
 * The block is allocated with the default alignment used by align_alloc,
 * and initialised with the T constructor called at the allocated address
 * with placement new.
 *
 * The align_unique_ptr deleter handles the destruction of the object by
 * calling the T destructor directly if the object is destructible and
 * freeing the allocated block.
 *
 * The factory function make_align_unique_ptr creates a single object and
 * returns an align_unique_ptr owing the newly created object.
 * It allocates a block of memory with size sizeof(T) with placement new
 * and returns the align_unique_ptr owing the pointer *ptr to the object.
 * Dereferencing the pointer returns the allocated object.
 *
 * @note Ensure the arguments are forwarded correctly to the object constructor
 * in align_alloc.
 *
 * @see https://stackoverflow.com/questions/3582001/advantages-of-using-forward
 */
template<typename T>
struct align_unique_ptr_deleter {
    align_unique_ptr_deleter() = default;
    ~align_unique_ptr_deleter() = default;

    void operator()(T *ptr) { align_free<T>(ptr); }
};

/**
 * align_unique_ptr<T,D>
 */
template<typename T,
         template<typename> class D = align_unique_ptr_deleter>
using align_unique_ptr = std::unique_ptr<T, D<T>>;

/**
 * make_align_unique_ptr<T,D,Args>
 */
template<typename T,
         template<typename> class D = align_unique_ptr_deleter,
         typename... Args>
align_unique_ptr<T,D> make_align_unique_ptr(Args&&... args)
{
    /* Create a deleter of an object of type T */
    D<T> del;

    /* Create a single block of memory sizeof(T) */
    T *ptr = align_alloc<T>(static_cast<Args&&>(args)...);

    return align_unique_ptr<T,D>(ptr, del);
}

/** ---- Memory aligned unique array ------------------------------------------
 * align_unique_array<T,D>
 * @brief align_unique_array is a 1-dimensional array, defined as a template
 * alias to unique_ptr with a custom deleter, align_unique_array_deleter.
 *
 * It contains a contiguous block of count objects of type T, stored in
 * an aligned block of memory with count*sizeof(T) bytes.
 * The block is allocated with the default alignment used by align_alloc,
 * and initialised with the T constructor called for each object at the
 * allocated address with placement new.
 *
 * The align_unique_array_deleter stores the number of allocated elements
 * in the array.
 * At destruction point, it calls the T destructor directly for each one,
 * if T is desctructible, and frees the array.
 *
 * The factory function make_align_unique_array handles an  creation.
 *  Dereferencing the pointer returns
 * the array given by *ptr.
 *
 * The factory function make_align_unique_array creates an array of count
 * objects and returns an align_unique_array owning a pointer to the newly
 * created array.
 * It allocates a block of memory size count*sizeof(T) with placement new
 * and returns the align_unique_array owning the pointer **ptr to the array.
 * Dereferencing the pointer returns the allocated array.
 *
 * @note Ensure the arguments are forwarded correctly to the object constructor
 * in align_alloc<T>.
 *
 * @see https://stackoverflow.com/questions/3582001
 *
 * @note align_unique_array can be generalized to more than one dimension
 * by storing a large 1-dimensional block with size(n1*n2*...), and then
 * having a set of n-1 arrays with offset addresses for each dimension.
 *
 * This is not implemented because it is more efficient memorywise to just
 * use a 1-dimensional array of size(n1*n2*n3*...) and use the appropriate
 * multidimensional addressing of the the form,
 *
 *  &X(i,j,k,..) = X + i*(n2*n3*...) + j*(n3*...) + k*(...) + ...
 *
 * For example, for a 3d array:
 *
 *  template<typename T,
 *            template<typename> class D = Array3dDeleter>
 *  using Array3d = std::unique_ptr<T ***, D<T>>;
 *
 *  // Create a tensor array with n1*n2*n3 objects of type T
 *  T *vector = align_alloc<T>(n1*n2*n3, static_cast<Args&&>(args)...);
 *
 *  T **matrix = align_alloc<T *>(n1*n2);
 *  for (size_t i = 0; i < n1*n2; ++i) {
 *      *(matrix + i) = (vector + i*n3);
 *  }
 *
 *  T ***tensor = align_alloc<T **>(n1);
 *  for (size_t i = 0; i < n1; ++i) {
 *      *(tensor + i) = (matrix + i*n2);
 *  }
 *
 *  // Create a tensor array pointer and set ownership
 *  T ****ptr = align_alloc<T ***>(1);
 *  *ptr = tensor;
 *  return Array3d<T>(ptr, del);
 *
 *  // Call the destructor for each object
 *  if (std::is_destructible<T>::value) {
 *      for (size_t i = 0; i < n1; ++i) {
 *          for (size_t j = 0; j < n2; ++j) {
 *              for (size_t k = 0; k < n3; ++k) {
 *                  (*ptr)[i][j][k].~T();
 *              }
 *          }
 *      }
 *  }
 *
 *  align_free((void *) (***ptr));   // Free the vector
 *  align_free((void **) (**ptr));   // Free the matrix
 *  align_free((void ***) (*ptr));   // Free the tensor
 *  align_free((void ****) (ptr));   // Free the pointer
 */
template<typename T>
struct align_unique_array_deleter {
    size_t m_count;                          /* Array dimensions. */

    align_unique_array_deleter() : m_count(0) {};
    align_unique_array_deleter(const size_t count) : m_count(count) {}
    ~align_unique_array_deleter() = default;

    void operator()(T **ptr) {
        align_free_array<T>(*ptr, m_count);  /* Free the array */
        align_free<T *>(ptr);               /* Free the pointer */
    }
};

/**
 * align_unique_array<T,D>
 */
template<typename T,
         template<typename> class D = align_unique_array_deleter>
using align_unique_array = std::unique_ptr<T *, D<T>>;

/**
 * make_align_unique_array<T,D>
 *
 * @throw
 * If count elements is not a positive value.
 */
template<typename T,
         template<typename> class D = align_unique_array_deleter,
         typename... Args>
align_unique_array<T,D> make_align_unique_array(size_t count, Args&&... args)
{
    core_assert(count > 0, "invalid array dimensions");

    /* Create an array deleter of count objects of type T */
    D<T> del(count);

    /* Create a pointer owning an array with count objects of type T */
    T **ptr = align_alloc<T *>();
    *ptr = align_alloc_array<T>(count, static_cast<Args&&>(args)...);

    return align_unique_array<T,D>(ptr, del);
}

}  /* core */
}  /* atto */

#endif /* ATTO_CORE_MEMORY_H_ */
