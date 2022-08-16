__kernel void helloworld(
    __global const float *a,
    __global const float *b,
    __global float *result,
    const unsigned long count)
{
    size_t idx = get_global_id(0);
    if (idx < count) {
        result[idx] = 0.0;
        float coeff = 1.0 / (float)(count);
        for (size_t j = 0; j < 100; ++j) {
            result[idx] += (j%2 == 0) ? a[idx]*coeff : b[idx]*coeff;
        }
    }
}
