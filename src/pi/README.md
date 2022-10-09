## mpi-opencl

MPI/OpenCL interop implementations.

- **compute_pi** Compute the integral
  $$\pi = \int_{0}^{1}\textrm{d}x\,\frac{1.0}{(1.0 + x^2)}$$
  
   numerically as a sum over a set of MPI processes.
   Each process runs its own OpenCL kernel to compute the corresponding partial sum.

   At the end of each model execution the master process collects the partial
   sums and computes $\pi$

<!--
## References
## Acknowlegements
-->

## License
Distributed under the terms of the [MIT](https://choosealicense.com/licenses/mit/) license. See  accompanying `LICENSE.md` for more information.
