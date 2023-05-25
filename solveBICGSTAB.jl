using DelimitedFiles, IterativeSolvers, SparseArrays, LinearAlgebra, CSV, Tables

function timeBICGSTAB(A, b, l)
    x = bicgstabl(A, b, l, reltol=1.0e-5)
    return x
end

dimensions = [10, 100, 1000, 2000]

println(dimensions)

l = 2
timing = zeros(Float16, length(dimensions), 2)
for (i, dimension) in enumerate(dimensions)
    println(i)
    A = readdlm("data/matrixA" * string(dimension) * ".csv", ',', Float64, '\n')
    b = readdlm("data/matrixb" * string(dimension) * ".csv", ',', Float64, '\n')
    timing[i, :] = [dimension, @elapsed timeBICGSTAB(A, b, l)]

    println("Time: ", timing[i, 2])
end

CSV.write("timing_Julia.csv", Tables.table(timing), writeheader=false)