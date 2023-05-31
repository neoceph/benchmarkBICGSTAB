import numpy as np
import os, csv, random

directoryName = "data"

if not os.path.exists(directoryName):
    os.makedirs(directoryName)

dimensions = [10, 100, 1000, 2000, 3000, 4000, 5000]

for dimension in dimensions:
    # generating tridiagonal matrix
    matrixA = np.zeros((dimension, dimension))
    for i in range(dimension):
        for j in range(dimension):
            if i == j:
                matrixA[i][j] = np.random.rand()
            elif i == j + 1 or i == j - 1:
                matrixA[i][j] = np.random.rand()
            else:
                matrixA[i][j] = 0
    matrixb = np.random.rand(dimension, 1)

    matrixAFilename = directoryName + "/matrixA" + str(dimension) + ".csv"
    matrixbFilename = directoryName + "/matrixb" + str(dimension) + ".csv"

    np.savetxt(matrixAFilename, matrixA, delimiter=",")
    np.savetxt(matrixbFilename, matrixb, delimiter=",")
