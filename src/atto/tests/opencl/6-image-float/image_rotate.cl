/**
 * image_rotate kernel source
 */
__kernel void image_rotate(
    __global float *dst_data,
    __global const float *src_data,
    const long width,
    const long height,
    const long stride,
    const float cos_theta,
    const float sin_theta)
{
    const long x = get_global_id(0);    // global pos in x-direction
    const long y = get_global_id(1);    // global pos in y-direction

    /* Bound checking */
    if (!(x < width && y < height)) {
        return;
    }

    /* Compute location of the data to move into (x,y). */
    float x0 = (float) width / 2.0f;
    float y0 = (float) height / 2.0f;

    float dx = (float) x - x0;
    float dy = (float) y - y0;

    long x1 = (long) (x0 + dx*cos_theta - dy*sin_theta);
    long y1 = (long) (y0 + dx*sin_theta + dy*cos_theta);

    long ix = stride*(y*width + x);
    if (x1 >= 0 && x1 < width && y1 >= 0 && y1 < height) {
        for (long k = 0; k < stride; ++k) {
            long ix1 = stride*(y1*width + x1);
            dst_data[ix + k] = src_data[ix1 + k];
        }
    } else {
        for (long k = 0; k < stride; ++k) {
            dst_data[ix + k] = 0.0f;
        }
    }
}
