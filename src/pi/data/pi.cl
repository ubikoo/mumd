#ifdef cl_khr_fp64
#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#endif

/**
 * reduce
 */
void reduce(__global double *group_sums, __local double *local_sums);

/**
 * reset_pi
 * @brief Reset the pi partial sums.
 */
__kernel void reset_pi(__global double *group_sums, const ulong n_intervals)
{
    const ulong id = get_global_id(0);
    if (id < n_intervals) {
        group_sums[id] = 0.0f;
    }
}

/**
 * compute_pi
 * @brief Compute the pi integral
 */
__kernel void compute_pi(
    __global double *group_sums,
    __local double *local_sums,
    const ulong n_intervals,
    const ulong n_interval_steps,
    const double xlo,
    const double xhi)
{
    const ulong global_id = get_global_id(0);
    const ulong local_id = get_local_id(0);
    const ulong local_size = get_local_size(0);
    const ulong group_id = get_group_id(0);
    if (global_id >= n_intervals) {
        return;
    }

    /* Compute local sums */
    const ulong begin = (group_id * local_size + local_id) * n_interval_steps;
    const ulong end = begin + n_interval_steps;

    double step_size = (xhi - xlo) / (double) (n_intervals * n_interval_steps);
    double sum = 0.0;
    for(ulong i = begin; i < end; ++i) {
        double x = xlo + (i + 0.5) * step_size;
        sum += 4.0 / (1.0 + x*x);
    }

    /* Ensure local memory consistency before summing the elements.  */
    local_sums[local_id] = sum * step_size;
    barrier(CLK_LOCAL_MEM_FENCE);

    reduce(group_sums, local_sums);
}

/**
 * reduce
 * @brief Perform a reduction sum of the local sums and store it in the partial
 * sums array at the corresponding work group index.
 */
void reduce(__global double *group_sums, __local double *local_sums)
{
    const uint local_id = get_local_id(0);
    const uint local_size = get_local_size(0);
    const uint group_id = get_group_id(0);

    /*
     * Sum the array using a divide and conquer algorithm:
     *  - partition the work group in two halves.
     *  - add the upper half of the work group to the lower half.
     */
    for (uint stride = local_size / 2; stride > 0; stride >>=1) {
        /* Ensure all work item writes are complete before adding again. */
        barrier(CLK_LOCAL_MEM_FENCE);
        if (local_id < stride) {
            local_sums[local_id] += local_sums[local_id + stride];
        }
    }

    if (local_id == 0) {
        group_sums[group_id] = local_sums[0];
    }
}