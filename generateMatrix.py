import numpy as np
import os, csv, random

directoryName = "data"

if not os.path.exists(directoryName):
    os.makedirs(directoryName)

dimensions = [10, 100, 1000, 10000]

for dimension in dimensions:
    matrixA = np.random.rand(dimension, dimension)
    matrixb = np.random.rand(dimension, 1)

    matrixAFilename = directoryName + "/matrixA" + str(dimension) + ".csv"
    matrixbFilename = directoryName + "/matrixb" + str(dimension) + ".csv"

    np.savetxt(matrixAFilename, matrixA, delimiter=",")
    np.savetxt(matrixbFilename, matrixb, delimiter=",")
