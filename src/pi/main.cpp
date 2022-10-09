/*
 * main.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "model.hpp"
#include "mpi.h"
using namespace atto;

/**
 * main test client
 */
int main(int argc, char *argv[])
{
   /*
    * Initialize MPI context.
    */
    MPI_Init(&argc, &argv);

    /* Get MPI world size and rank */
    int n_procs;
    MPI_Comm_size(MPI_COMM_WORLD, &n_procs);
    int proc_id;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);

   /*
    * Execute the model.
    */
    Model model(proc_id, n_procs);
    for (cl_ulong iter = 0; iter < Params::n_iters; ++iter) {
        /* Compute pi partial sums on each process */
        model.execute();

        if (proc_id == Params::master_id) {
            std::cout << core::str_format("\niter %lu of %lu\n",
                iter, Params::n_iters);
        }
        /* Receive the CPU partial sums from each process and compute pi */
        if (proc_id == Params::master_id) {
            std::cout << "CPU\n";
            cl_double pi_sum = model.m_data.pi_cpu;
            for (int src_id = 1; src_id < n_procs; ++src_id) {
                cl_double pi_partial = 0.0;
                MPI_Recv(
                    (void *) &pi_partial,
                    sizeof(pi_partial),
                    MPI_BYTE,
                    src_id,
                    0,
                    MPI_COMM_WORLD,
                    MPI_STATUS_IGNORE);
                pi_sum += pi_partial;

                std::cout << core::str_format(
                    "recv: proc %d of %d, "
                    "pi_partial %.15lf, "
                    "pi_sum %.15lf, "
                    "err %.15lf\n",
                    src_id, n_procs, pi_partial, pi_sum, std::fabs(M_PI - pi_sum));
            }
        } else {
            cl_double pi_partial = model.m_data.pi_cpu;
            MPI_Send(
                (void *)&pi_partial,
                sizeof(pi_partial),
                MPI_BYTE,
                Params::master_id,
                0,
                MPI_COMM_WORLD);
        }

        /* Receive the GPU partial sums from each process and compute pi */
        if (proc_id == Params::master_id) {
            std::cout << "GPU\n";
            cl_double pi_sum = model.m_data.pi_gpu;
            for (int src_id = 1; src_id < n_procs; ++src_id) {
                cl_double pi_partial = 0.0;
                MPI_Recv(
                    (void *) &pi_partial,
                    sizeof(pi_partial),
                    MPI_BYTE,
                    src_id,
                    0,
                    MPI_COMM_WORLD,
                    MPI_STATUS_IGNORE);
                pi_sum += pi_partial;

                std::cout << core::str_format(
                    "recv: proc %d of %d, "
                    "pi_partial %.15lf, "
                    "pi_sum %.15lf, "
                    "err %.15lf\n",
                    src_id, n_procs, pi_partial, pi_sum, std::fabs(M_PI - pi_sum));
            }
        } else {
            cl_double pi_partial = model.m_data.pi_gpu;
            MPI_Send(
                (void *)&pi_partial,
                sizeof(pi_partial),
                MPI_BYTE,
                Params::master_id,
                0,
                MPI_COMM_WORLD);
        }
    }

    /*
     * Finalize MPI context.
     */
    MPI_Finalize();

    exit(EXIT_SUCCESS);
}
