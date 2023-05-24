import jax
import jax.numpy as jnp
import numpy as np
from jax.scipy.sparse.linalg import bicgstab
import timeit
import csv

gpu_device = jax.devices('gpu')[0]
cpu_device = jax.devices('cpu')[0]

def timeBICGSTAB(A, b):
  x = bicgstab(A, b)
  return x

dimensions = [10, 100, 1000, 2000]

elapsed_time_cpu = []
elapsed_time_gpu = []

for i, dimension in enumerate(dimensions):
    # loading data
    fileNameMatrixA = 'data/matrixA' + str(dimension) + '.csv'
    fileNameMatrixb = 'data/matrixb' + str(dimension) + '.csv'

    A = np.loadtxt(fileNameMatrixA, delimiter=',')
    b = np.loadtxt(fileNameMatrixb, delimiter=',')

    # converting to jax arrays
    A = jnp.array(A)
    b = jnp.array(b)

    with jax.default_device(cpu_device):
        start_time = timeit.default_timer()
        x_cpu = jax.jit(timeBICGSTAB)(A, b)
        end_time = timeit.default_timer()
        elapsed_time_cpu.append([i, (end_time - start_time)])


    with jax.default_device(gpu_device):
        start_time = timeit.default_timer()
        x_gpu = jax.jit(timeBICGSTAB)(A, b)
        end_time = timeit.default_timer()
        elapsed_time_gpu.append([i, (end_time - start_time)])

    print("\nCpu timing: ")
    print(elapsed_time_cpu[i][1])
    print("\nGpu timing: ")
    print(elapsed_time_gpu[i][1])

with open('timing_jax_cpu.csv', 'w', newline='') as csvfile:
    writer = csv.writer(csvfile)
    writer.writerows(elapsed_time_cpu)

with open('timing_jax_gpu.csv', 'w', newline='') as csvfile:
    writer = csv.writer(csvfile)
    writer.writerows(elapsed_time_gpu)