using DelimitedFiles, IterativeSolvers, SparseArrays, LinearAlgebra, CSV, Tables

function timeBICGSTAB(A, b, l)
    x = bicgstabl(A, b, l, reltol=1.0e-5)
    return x
end

dimensions = [10, 100]

println(dimensions)

l = 2
timing = zeros(Float16, length(dimensions), 2)
for (i, dimension) in enumerate(dimensions)
    println(i)
    A = readdlm("data/matrixA" * string(dimension) * ".csv", ',', Float64, '\n')
    b = readdlm("data/matrixb" * string(dimension) * ".csv", ',', Float64, '\n')
    # timing[i, 1] = i
    timing[i, :] = [i, @elapsed timeBICGSTAB(A, b, l)]

    # CSV.write("X_Julia_" * string(i) * ".csv", Tables.table(x), writeheader=false)
    # println("Time: ", timing[i])
end

CSV.write("timing_Julia.csv", Tables.table(timing), writeheader=false)