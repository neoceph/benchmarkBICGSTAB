using DelimitedFiles, IterativeSolvers, SparseArrays, LinearAlgebra, CSV, Tables

function timeBICGSTAB(A, b, l)
    x = bicgstabl(A, b, l, reltol=1.0e-5)
    return x
end

dimensions = [10, 100, 1000, 10000]

println(dimensions)

A = readdlm("Matrix_A.csv", ',', Float64, '\n')
b = readdlm("Matrix_b.csv", ',', Float64, '\n')

l = 2

# x = bicgstabl(A, b, l, reltol=1.0e-5)
time, x = @time timeBICGSTAB(A, b, l)

CSV.write("X_Julia.csv", Tables.table(x), writeheader=false)

