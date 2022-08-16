/*
 * file.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_CORE_FILE_H_
#define ATTO_CORE_FILE_H_

#include <vector>
#include <limits>
#include "atto/core/error.hpp"
#include "atto/core/string.hpp"

namespace atto {
namespace core {

/** ---- File class -----------------------------------------------------------
 *                         ->  File  <-
 *                       /             \
 *                  FileStdIn       FileStdOut
 *                     ^                ^
 *                     |                |
 *                 FileIn           FileOut
 * @brief File is a base class representing a C-style std::FILE stream.
 * Together with the derived classes - FileStdIn and FileIn, FileStdOut
 * and FileOut - it provides an abstraction of the C I/O subset of the
 * C++ standard library.
 *
 * @note C-streams are objects of type std::FILE that can only be accessed
 * and manipulated through pointers of type std::FILE*.
 * Important: while it may be possible to create a local object of type
 * std::FILE by dereferencing and copying a valid std::FILE*, using the
 * address of such copy in the I/O functions is undefined behavior.
 *
 * @see https://en.cppreference.com/w/cpp/io/c
 */
struct File {
    /* File member variables. */
    enum : uint32_t {
        Text    = 0,
        Binary  = 1,
    };
    std::FILE *m_fp;        /* pointer to file object */
    bool m_binary;          /* is the file mode binary? */

    /* File query functions. */
    bool is_open(void) const { return (m_fp != nullptr); }
    bool is_binary(void) const { return m_binary; }
    bool is_good(void) const;
    bool is_error(void) const;
    bool is_eof(void) const;
    void clear(void) const;

    /* File constructor/destructor. */
    File() : m_fp(nullptr), m_binary(false) {}
    virtual ~File() {}
    /*
     * Disable copy constructor/assignment semantics. Each stream is uniquely
     * associated with the File object.
     */
    File(const File &) = delete;
    File &operator=(const File &) = delete;
};

/**
 * File::is_good
 * @brief Return true if most recent I/O stream operation was successful.
 * Specifically, check whether the error and the eof indicators associated
 * with an open file stream are set.
 */
core_inline
bool File::is_good(void) const
{
    return !(is_error() || is_eof());
}

/**
 * File::is_error
 * @brief Check whether the error indicator associated with the stream
 * is set, given by std::error return value - non zero if it is.
 */
core_inline
bool File::is_error(void) const
{
    if (is_open()) {
        return (std::ferror(m_fp) != 0);
    }
    return false;
}

/**
 * File::is_eof
 * @brief Check whether the eof indicator associated with the stream
 * is set, given by std::eof return value - non zero if it is.
 */
core_inline
bool File::is_eof(void) const
{
    if (is_open()) {
        return (std::feof(m_fp) != 0);
    }
    return false;
}

/**
 * File::clear
 * @brief Reset both error and eof indicators associated with the stream.
 */
core_inline
void File::clear(void) const
{
    if (is_open()) {
        std::clearerr(m_fp);
    }
}

/** ---- FileStdIn class ------------------------------------------------------
 *
 * FileStdIn
 * @brief FileStdIn is a File derived class providing methods for reading
 * from standard input:
 *
 *  - Read a line from input stream until end of line or the specified
 *    delimiter delim. The delim character is extracted but not stored.
 *  - Read all lines from input stream and returns a vector of strings.
 *  - Read a formated input from input stream.
 */
struct FileStdIn : File {
    /*
     * FileStdIn readline functions.
     */
    bool readline(
        std::string &line,
        size_t limit = std::numeric_limits<size_t>::max(),
        const char delim = '\n',
        const char ignore = '\0') const;
    bool readlines(
        std::vector<std::string> &lines,
        size_t hint = std::numeric_limits<size_t>::max(),
        const char delim = '\n',
        const char ignore = '\0') const;

    /*
     * FileStdIn scanf functions.
     */
    template<typename... Args>
    int scanf(const char* format, Args&&... args) const {
        core_assert(is_open(), "file stream is not open");
        core_assert(is_good(), "file stream is not good");
        core_assert(!is_binary(), "file stream mode is binary");
        return std::fscanf(m_fp, format, static_cast<Args&&>(args)...);
    }

    /*
     * FileStdIn constructor/destructor.
     */
    FileStdIn() {
        m_fp = stdin;        /* set the file stream pointer to stdin */
        m_binary = false;    /* stdin is opened in text mode by default */
    }
    virtual ~FileStdIn() {}
    /*
     * Disable copy constructor/assignment semantics.
     * Each stream is unique to the FileStdIn object.
     */
    FileStdIn(const FileStdIn &) = delete;
    FileStdIn &operator=(const FileStdIn &) = delete;
};

/** ---- FileStdIn text input functions ---------------------------------------
 *
 * FileStdIn::readline
 * @brief Read a line from an input stream and strip newline characters.
 *
 * Stop when either:
 *  limit characters are read (default max(size_t)), or
 *  delim character is read (default newline '\n'), or
 *  end of file EOF is reached, whichever comes first.
 *
 * Any characters read after an ignore marker (default '\0') are ignored.
 * Return the stream state after reading.
 *
 * @note C version ------------------------------------------------------------
 *
 *  int c;      // note: int, not char, required to handle EOF
 *  while ((c = std::fgetc(fp)) != EOF) {
 *      std::putchar(c);
 *  }
 *
 *  if (std::ferror(fp))
 *      std::puts("I/O error when reading");
 *  else if (std::feof(fp))
 *      std::puts("End of file reached successfully");
 *
 * The function signature,
 *
 *  int std::fgetc(std::FILE* stream);
 *
 * returns the obtained character on success or EOF on failure.
 *
 * If the failure has been caused by end of file condition,
 * additionally sets the eof indicator(see std::feof()) on stream.
 *
 * If the failure has been caused by some other error, sets the error
 * indicator(see std::ferror()) on stream.
 *
 * @note C++ version ----------------------------------------------------------
 *
 *  char c;
 *  while (stream.get(c)) {
 *      std::cout << c;
 *  }
 *
 * The function signature,
 *
 *  int_type get();
 *
 * reads and returns one character. Otherwise, returns Traits::eof()
 * and sets failbit and eofbit.
 *
 * The function signature
 *
 *  basic_istream& get( char_type& ch );
 *
 * reads one character and stores it to ch if available. Otherwise,
 * leaves c unmodified and sets failbit and eofbit.
 *
 * @note (Character handling) -------------------------------------------------
 *
 * Handle null character '\0':
 * std::string automatically appends a null character '\0' to
 * the written string, even if an empty string is extracted.
 *
 * Handle newline character '\n':
 * Instead of std::strlen which has undefined behaviour for an
 * empty buffer beginning with '\0',
 *
 * if (*(str + std::strlen(str) - 1) == '\n')
 *     *(str + std::strlen(str) - 1) = '\0';
 *
 * std::strcspn counts the number of characters until it finds
 * a '\n' character or a '\r' character.
 *
 * If none found, it stops at the null terminator '\0' and returns 0.
 * In the later case, the operation is redudant but safe by simply
 * replacing character '\0' with the same character '\0'.
 *
 * str[std::strcspn(str, "\n")] = '\0';
 *
 * Alternatively, the function std::strchr returns a pointer to the
 * matched character '\n' or null if none is found.
 *
 * @see https://isocpp.org/wiki/faq/input-output
 *      https://en.cppreference.com/w/cpp/io/c/fgetc
 *      https://en.cppreference.com/w/cpp/io/basic_istream/get
 *      http://www.cplusplus.com/reference/istream/istream/get
 *      https://stackoverflow.com/questions/8558907
 */
core_inline
bool FileStdIn::readline(
    std::string &line,
    size_t limit,
    const char delim,
    const char ignore) const
{
    /* Assert I/O preconditions. */
    core_assert(is_open(), "file stream is not open");
    core_assert(is_good(), "file stream is not good");
    core_assert(!is_binary(), "file stream mode is binary");

    /*
     * Read a line from the input stream one character at a time.
     */
    bool isignore = false;      /* set to true if ignore character is read */
    line.clear();               /* clear line buffer before reading */
    int c;                      /* note: use int, not char, to handle EOF */
    while ((c = std::fgetc(m_fp)) != EOF) {
        if (c == delim) {
            break;              /* read newline or end character */
        }

        if (c == ignore) {
            isignore = true;    /* ignore any further characters */
        }

        if (!isignore) {
            line.push_back(c);  /* store character in the buffer */
        }

        if (line.size() == limit) {
            break;              /* read limit number of characters */
        }
    }

    /* Return the stream state. */
    return is_good();
}

/**
 * FileStdIn::readlines
 * @brief Read lines from a stream using readline() and return a vector
 * containing the lines thus read.
 *
 * Stop when either:
 *  - buffer number of lines reaches hint value (default max(size_t)), or
 *  - end-of-file is reached, whichever comes first.
 * Each line reads characters from the input stream until a delim character
 * is read (default newline '\n'). Any characters read after ignore marker
 * (default '\0') are ignored.
 */
core_inline
bool FileStdIn::readlines(
    std::vector<std::string> &lines,
    size_t hint,
    const char delim,
    const char ignore) const
{
    /* Assert I/O preconditions. */
    core_assert(is_open(), "file stream is not open");
    core_assert(is_good(), "file stream is not good");
    core_assert(!is_binary(), "file stream mode is binary");

    /*
     * Read lines from stream and store them in the buffer.
     */
    constexpr size_t limit = std::numeric_limits<size_t>::max();
    lines.clear();              /* clear buffer list before reading */
    std::string line;           /* line buffer */
    while (lines.size() < hint && readline(line, limit, delim, ignore)) {
        lines.push_back(line);
    }

    /* Return the stream state. */
    return is_good();
}

/** ---- FileIn class ---------------------------------------------------------
 *
 * FileIn
 * @brief FileIn is a FileStdIn derived class providing methods for reading
 * from file streams.
 *
 * In addition to text stream methods provided by FileStdIn, FileIn
 * implements:
 *  - Opening a file for reading with a given filename and binary mode.
 *  - Closing any open file.
 *  - Query the file length in bytes.
 *  - Read a block of size bytes from a file in binary mode.
 */
struct FileIn : FileStdIn {
    /*
     * FileIn open/close functions.
     */
    void open(const char *filename, const uint32_t mode = Text);
    void open(const std::string &filename, const uint32_t mode = Text) {
        core_assert(filename.size() > 0, "invalid filename");
        try {
            open(filename.c_str(), mode);
        } catch (std::exception& e) {
            core_throw(e.what());
        }
    }
    void close(void);

    /*
     * FileIn input functions.
     */
    int64_t length(void) const;
    bool read (void *ptr, size_t size) const;

    /*
     * FileIn constructor/destructor.
     */
    FileIn() {
        m_fp = nullptr;      /* m_fp = stdin, reset it by default */
        m_binary = false;    /* reset file mode to text by default */
    }
    FileIn(const char *filename, const uint32_t mode = Text) {
        core_assert(filename != nullptr, "null filename");
        m_fp = nullptr;      /* m_fp = stdin, reset it by default */
        try {
            open(filename, mode);
        } catch (std::exception& e){
            core_throw(e.what());
        }
    }
    FileIn(const std::string &filename, const uint32_t mode = Text) {
        core_assert(!filename.empty(), "empty filename");
        m_fp = nullptr;      /* m_fp = stdin, reset it by default */
        try {
            open(filename, mode);
        } catch (std::exception& e){
            core_throw(e.what());
        }
    }
    virtual ~FileIn() { close(); }
};

/**
 * FileIn::open
 * @brief Open an input stream for reading in a given binary mode.
 * The filename is a non null string with the name of the file.
 * The flag binary is a boolean flag specifying the file binary mode.
 */
core_inline
void FileIn::open(const char *filename, const uint32_t mode)
{
    /* Assert I/O preconditions. */
    core_assert(filename != nullptr, "null filename");
    core_assert(!is_open(), "file stream is already open");

    /* Set file mode and open file stream. */
    m_binary = (mode == Binary);
    std::string modestr = m_binary ? "rb" : "r";

    m_fp = std::fopen(filename, modestr.c_str());
    core_assert(is_open(), str_format(
        "failed to open %s, mode %s", filename, modestr.c_str()));
}

/**
 * FileIn::close
 * @brief Close an input stream. Any unread buffered data are discarded.
 *
 * If stream is successfully closed, a zero value is returned, otherwise
 * EOF is returned. Even if the call fails, the stream will no longer be
 * associated with the file.
 */
core_inline
void FileIn::close(void)
{
    /* File pointer is not attached, nothing to do. */
    if (!is_open()) {
        return;
    }

    /* Close the file stream and reset the file pointer. */
    std::fclose(m_fp);
    m_fp = nullptr;
    m_binary = false;
}

/**
 * FileIn::length
 * @brief Return the length of a given stream in bytes. On failure, return -1.
 *
 * For binary streams, this is the number of bytes from the beginning of
 * the file. For text streams, the numerical value may not be meaningful.
 *
 * @note C++ version
 * std::fstream has a joint file position maintained for both the input
 * sequence and the output sequence.
 * std::filebuf, the buffer used by std::fstream, has only one pointer.
 *
 * The functions seekg() and seekp() move the same pointer, and tellg()
 * and tellp() return its position.
 *
 * @see https://stackoverflow.com/questions/19958679
 */
core_inline
int64_t FileIn::length(void) const
{
    /* Assert I/O preconditions. */
    core_assert(is_open(), "file stream is not open");
    core_assert(is_good(), "file stream is not good");
    core_assert(is_binary(), "file stream is not binary");

    /* Compute the file length in bytes.  */
    std::fseek(m_fp, 0, SEEK_END);   /* go to end of file */
    int64_t len = std::ftell(m_fp);  /* get length in bytes */
    std::fseek(m_fp, 0, SEEK_SET);   /* go back to begining of file */
    return len;
}

/**
 * FileIn::read
 * @brief Read one block of size bytes from the input stream as if by
 * calling std::fgetc size times, and storing the results, in the order
 * obtained, into the successive positions of buffer.
 *
 * The number of objects successfully read may be less than count 1 if
 * an error or end-of-file condition occurs. This is asserted by the
 * stream state return value.
 *
 * @note Read one block with size bytes from the buffer stream if:
 *  - file pointer is attached, and
 *  - file pointer state is good, and
 *  - ptr is not nullptr(undefined behaviour)
 *
 * @note C++ version
 *  if (!fp.read(static_cast<char *>(ptr), size)) {
 *      throw runtime_error("I/O read error");
 *  }
 */
core_inline
bool FileIn::read (void *ptr, size_t size) const
{
    /* Assert I/O preconditions. */
    core_assert(is_open(), "file stream is not open");
    core_assert(is_good(), "file stream is not good");
    core_assert(is_binary(), "file stream mode is not binary");

    core_assert(ptr != nullptr, "invalid pointer");
    core_assert(size > 0, "invalid size");

    /* Read one block of size bytes from the file stream. */
    std::fread(ptr, size, 1, m_fp);

    /* Return the stream state. */
    return is_good();
}

/** ---- FileStdOut class -----------------------------------------------------
 *
 * FileStdOut
 * @brief FileStdIn is a File derived class providing methods for writing
 * to standard output:
 *  - write a null terminated string to output stream.
 *  - write all lines in a list of strings to output stream.
 *  - write the supplied data as a formatted string to output stream.
 */
struct FileStdOut : File {
    /*
     * FileStdOut text input/output functions.
     */
    bool writeline(const std::string &line,
                   const std::string &sep = std::string()) const;
    bool writelines(const std::vector<std::string> &lines,
                    const std::string &sep = std::string()) const;

    /*
     * FileStdOut printf function.
     */
    template<typename... Args>
    int printf(const char* format, Args&&... args) const {
        core_assert(is_open(), "file stream is not open");
        core_assert(is_good(), "file stream is not good");
        core_assert(!is_binary(), "file stream mode is binary");
        return std::fprintf(m_fp, format, static_cast<Args&&>(args)...);
    }

    /*
     * FileStdIn constructor/destructor.
     */
    FileStdOut() {
        m_fp = stdout;       /* set the file stream pointer to stdout */
        m_binary = false;    /* stdout is opened in text mode by default */
    }
    virtual ~FileStdOut() {}
    /*
     * Disable copy constructor/assignment semantics.
     * Each stream is unique to the FileStdOut object.
     */
    FileStdOut(const FileStdOut &) = delete;
    FileStdOut &operator=(const FileStdOut &) = delete;
};

/**
 * FileStdOut::writeline
 * @brief Write every character from the null terminated string line
 * to the output stream, as if by repeatedly executing std::fputc.
 * The terminating null character from is not written.
 *
 * The argument sep provides a line separator.
 */
core_inline
bool FileStdOut::writeline(
    const std::string &line,
    const std::string &sep) const
{
    /* Assert I/O preconditions. */
    core_assert(is_open(), "file stream is not open");
    core_assert(is_good(), "file stream is not good");
    core_assert(!is_binary(), "file stream mode is binary");

    /*
     * Write the line if it contains data. If stream state
     * is valid, append the separator if it is not empty.
     */
    if (!line.empty()) {
        std::fputs(line.c_str(), m_fp);
    }

    if (is_good() && !sep.empty()) {
        std::fputs(sep.c_str(), m_fp);
    }

    /* Return the stream state. */
    return is_good();
}

/**
 * File::writelines
 * @brief Write lines to the stream using writeline().
 */
core_inline
bool FileStdOut::writelines(
    const std::vector<std::string> &lines,
    const std::string &sep) const
{
    /* Assert I/O preconditions. */
    core_assert(is_open(), "file stream is not open");
    core_assert(is_good(), "file stream is not good");
    core_assert(!is_binary(), "file stream mode is binary");

    /* Try writing lines to the stream. */
    std::vector<std::string>::const_iterator it = lines.begin();
    while (it != lines.end() && writeline(*it, sep)) {
        ++it;
    }

    /* Return the stream state. */
    return is_good();
}

/** ---- FileOut class --------------------------------------------------------
 *
 * FileOut
 * @brief FileOut is a FileStdOut derived class providing methods for
 * writing to file streams.
 * In addition to text stream methods provided by FileStdOut, FileOut
 * implements:
 *  - open a file for writing with a given filename and binary mode.
 *  - close any open file.
 *  - query the file length in bytes.
 *  - write a block of size bytes to a file in binary mode.
 */
struct FileOut : FileStdOut {
    /*
     * FileOut open/close functions.
     */
    void open(const char *filename, const uint32_t mode = Text);
    void open(const std::string &filename, const uint32_t mode = Text) {
        core_assert(filename.size() > 0, "invalid filename");
        try {
            open(filename.c_str(), mode);
        } catch (std::exception& e) {
            core_throw(e.what());
        }
    }
    void close(void);

    /*
     * FileOut input functions.
     */
    int64_t length(void) const;
    bool write (void *ptr, size_t size) const;

    /*
     * FileOut constructor/destructor.
     */
    FileOut() {
        m_fp = nullptr;      /* m_fp = stdout, reset it by default */
        m_binary = false;    /* reset file mode to text by default */
    }
    FileOut(const char *filename, const uint32_t mode = Text) {
        core_assert(filename != nullptr, "null filename");
        m_fp = nullptr;      /* m_fp = stdout, reset it by default */
        try {
            open(filename, mode);
        } catch (std::exception& e){
            core_throw(e.what());
        }
    }
    FileOut(const std::string &filename, const uint32_t mode = Text) {
        core_assert(!filename.empty(), "empty filename");
        m_fp = nullptr;      /* m_fp = stdout, reset it by default */
        try {
            open(filename, mode);
        } catch (std::exception& e){
            core_throw(e.what());
        }
    }
    virtual ~FileOut() { close(); }
};

/**
 * FileOut::open
 * @brief Open an input stream for writing in a given binary mode.
 *
 * @param filename  non null string with the name of the file.
 * @param binary    boolean flag asserting the file binary mode.
 */
core_inline
void FileOut::open(const char *filename, const uint32_t mode)
{
    /* Assert I/O preconditions. */
    core_assert(filename != nullptr, "null filename");
    core_assert(!is_open(), "file stream is already open");

    /* Set file mode and open file stream. */
    m_binary = (mode == Binary);
    std::string modestr = m_binary ? "wb" : "w";

    m_fp = std::fopen(filename, modestr.c_str());
    core_assert(is_open(), str_format(
        "failed to open %s, mode %s", filename, modestr.c_str()));
}

/**
 * FileOut::close
 * @brief Close an input stream.
 *
 * If the stream is successfully closed, a zero value is returned.
 * Otherwise end-of-file is returned. Even if the call fails, the
 * stream will no longer be associated with the file.
 *
 * Any unwritten buffered data are flushed with std::fflush.
 * Flushing an input stream is undefined behaviour.
 *
 * @see https://en.cppreference.com/w/cpp/io/c/fflush
 */
core_inline
void FileOut::close(void)
{
    /* File pointer is not attached, nothing to do. */
    if (!is_open()) {
        return;
    }

    /* Close the file stream and reset the file pointer. */
    std::fflush(m_fp);
    std::fclose(m_fp);
    m_fp = nullptr;
    m_binary = false;
}

/**
 * FileOut::length
 * @brief Return the length of a given stream in bytes. On failure, return -1.
 *
 * For binary streams, this is the number of bytes from the beginning of
 * the file. For text streams, the numerical value may not be meaningful.
 *
 * @note C++ version
 * std::fstream has a joint file position maintained for both the input
 * sequence and the output sequence.
 * std::filebuf, the buffer used by std::fstream, has only one pointer.
 *
 * The functions seekg() and seekp() move the same pointer, and tellg()
 * and tellp() return its position.
 *
 * @see https://stackoverflow.com/questions/19958679
 */
core_inline
int64_t FileOut::length(void) const
{
    /* Assert I/O preconditions. */
    core_assert(is_open(), "file stream is not open");
    core_assert(is_good(), "file stream is not good");
    core_assert(is_binary(), "file stream is not binary");

    /* Compute the file length in bytes. */
    std::fseek(m_fp, 0, SEEK_END);   /* go to end of file */
    int64_t len = std::ftell(m_fp);  /* get length in bytes */
    std::fseek(m_fp, 0, SEEK_CUR);   /* go to current file position */
    return len;
}

/**
 * FileOut::write
 * @brief Write one block of size bytes from the input stream as if
 * by interpreting the block as an array of unsigned char and calling
 * std::fputc size times to write those unsigned chars into stream.
 * The file position indicator for the stream is advanced by the
 * number of characters.
 *
 * @note Write one block with size bytes into the buffer stream if:
 *  - file pointer is attached, and
 *  - file pointer state is good, and
 *  - ptr is not nullptr(undefined behaviour)
 *
 * @note C++ version
 *  if (!fp.write(static_cast<char *>(ptr), size)) {
 *      throw runtime_error("I/O write error");
 *  }
 */
core_inline
bool FileOut::write (void *ptr, size_t size) const
{
    /* Assert I/O preconditions. */
    core_assert(is_open(), "file stream is not open");
    core_assert(is_good(), "file stream is not good");
    core_assert(is_binary(), "file stream mode is not binary");

    core_assert(ptr != nullptr, "invalid pointer");
    core_assert(size > 0, "invalid size");

    /* Write one block of size bytes to the file stream. */
    std::fwrite(ptr, size, 1, m_fp);

    /* Return the stream state. */
    return is_good();
}

}  /* core */
}  /* atto */

#endif /* ATTO_CORE_FILE_H_ */
