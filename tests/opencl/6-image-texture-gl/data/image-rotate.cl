/**
 * image_rotate kernel source
 */
__kernel void image_rotate(
    __read_only image2d_t src_image,
    __write_only image2d_t dst_image,
    sampler_t sampler,
    const long width,
    const long height,
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

    float4 color = (float4) ((float) x / width, (float) y / height, 0, 1);
    if (x1 >= 0 && x1 < width && y1 >= 0 && y1 < height) {
        color = read_imagef(src_image, sampler, (int2) (x1, y1));
    }
    write_imagef(dst_image, (int2) (x, y), color);
}