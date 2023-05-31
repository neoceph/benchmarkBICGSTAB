import numpy as np
import matplotlib.pyplot as plt

# Load data
cppEigen = np.loadtxt('timing_cpp_Eigen.csv', delimiter=',')
cppPetsc = np.loadtxt('timing_cpp.csv', delimiter=',')
julia = np.loadtxt('timing_Julia.csv', delimiter=',')
jax_cpu = np.loadtxt('timing_jax_cpu.csv', delimiter=',')
jax_gpu = np.loadtxt('timing_jax_gpu.csv', delimiter=',')

# Plot
plt.figure()
plt.plot(cppEigen[:,0], cppEigen[:,1]/1e6, 'o-', label='C++ Eigen (single core)')
plt.plot(cppPetsc[:,0], cppPetsc[:,1]/1e6, 's-', label='C++ PETSc (single core)')
plt.plot(julia[:,0], julia[:,1], '^-', label='Julia')
plt.plot(jax_cpu[:,0], jax_cpu[:,1], '*-', label='JAX CPU')
plt.plot(jax_gpu[:,0], jax_gpu[:,1], '>-', label='JAX GPU')
plt.xlabel('Degrees of Freedom')
plt.ylabel('Time (s)')
# plt.yscale('log')
plt.title('Timing to Solve Sparse Matrix (BICGSTAB)')
plt.legend()
plt.savefig('timing.png')