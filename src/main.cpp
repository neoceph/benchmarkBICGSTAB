// standard headers
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

// third party headers
#include <armadillo>
#include <Eigen/Core>
#include <Eigen/IterativeLinearSolvers>
#include <petscksp.h>
#include <petscvec.h>
#include <petsc.h>

// method for reading multi column csv file into an array
std::vector<std::vector<double>> getMatrix(std::string filePath){
    std::ifstream  file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
    }
    std::string line;
    std::vector<std::vector<std::string>> parsedCsv;
    std::vector<std::vector<double>> parsedDoubleCsv;
    while(std::getline(file,line))
    {
        std::stringstream lineStream(line);
        std::string cell;
        std::vector<double> parsedRow;
        while(std::getline(lineStream,cell,','))
        {
            parsedRow.push_back(std::stod(cell));
        }

        parsedDoubleCsv.push_back(parsedRow);
    
    }
    return parsedDoubleCsv;

};

// method for reading single column csv file into a vector
std::vector<double> getVector(std::string filePath){
    std::ifstream  file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
    }
    std::string line;
    std::vector<std::vector<std::string>> parsedCsv;
    std::vector<double> parsedVector;
    while(std::getline(file,line))
    {
        // std::stringstream lineStream(line);
        parsedVector.push_back(std::stod(line));
    
    }
    return parsedVector;

};


int main(int argc, char **argv) {

    std::vector<int> dimensions = {10, 100, 1000, 2000, 3000, 4000, 5000};
    std::string pathMatrixA = "./data/matrixA";
    std::string pathMatrixB = "./data/matrixb";
    std::string matrixAFile, matrixbFile;
    std::vector<std::vector<double>> matrixA;
    std::vector<double> vectorB;
    std::ofstream outputEigenFile("timing_cpp_Eigen.csv");
    
    if (outputEigenFile.is_open())
    {

        for (const auto& dimension : dimensions)
        {
            outputEigenFile << dimension << ",";

            matrixAFile = pathMatrixA + std::to_string(dimension) + ".csv";
            matrixbFile = pathMatrixB + std::to_string(dimension) + ".csv";


            matrixA = getMatrix(matrixAFile);
            vectorB = getVector(matrixbFile);


            // Create the Eigen matrix and vector
            Eigen::Map<Eigen::VectorXd> b_eigen(vectorB.data(), vectorB.size());
            Eigen::MatrixXd A_eigen;
            A_eigen.resize(matrixA.size(), matrixA[0].size());

            for (int i = 0; i < matrixA.size(); ++i) 
            {
                for (int j = 0; j < matrixA[i].size(); ++j) 
                {
                    A_eigen(i, j) = matrixA[i][j];
                }
            }

            // std::cout << "Eigen Matrix:\n" << A_eigen << std::endl;
            // std::cout << "Eigen Vector:\n" << b_eigen << std::endl;

            Eigen::BiCGSTAB<Eigen::MatrixXd> solver;
            solver.setTolerance(1e-6);
            solver.setMaxIterations(10000);
            // Set the system matrix and perform the factorization
            solver.compute(A_eigen);

            // Solve the linear system

            auto beginTime  = std::chrono::high_resolution_clock::now();
            Eigen::VectorXd x = solver.solve(b_eigen);
            auto endTime  = std::chrono::high_resolution_clock::now();
            auto totalTime = endTime - beginTime;

            outputEigenFile << std::chrono::duration_cast<std::chrono::microseconds>(endTime-beginTime).count() << std::endl;

            std::cout << "#iterations:     " << solver.iterations() << std::endl;
            std::cout << "estimated error: " << solver.error()      << std::endl;
  


            // Check if the solver succeeded
            if (solver.info() == Eigen::Success) {
                // Print the solution
                // std::cout << "Solution: " << x.transpose() << std::endl;
                std::cout<<"Solution Successful!"<<std::endl;
            } 
            else {
                // The solver failed to converge
                std::cout << "Solver failed!" << std::endl;
            }
        }
        outputEigenFile.close();
    }
    else
    {
        std::cout << "Unable to open file";
    }




    PetscInitialize(&argc, &argv, NULL, NULL);
       
    std::ofstream outputFile("timing_cpp.csv");

    if (outputFile.is_open())
    {

        for (const auto& dimension : dimensions)
        {
            outputFile << dimension << ",";
            
            matrixAFile = pathMatrixA + std::to_string(dimension) + ".csv";
            matrixbFile = pathMatrixB + std::to_string(dimension) + ".csv";


            matrixA = getMatrix(matrixAFile);
            vectorB = getVector(matrixbFile);

            // Create the PETSC matrix and vector
            Mat A;
            Vec b, x;
            PetscInt n = dimension;  // Size of the matrix
            PetscInt i, j, start, end;


            MatCreate(PETSC_COMM_WORLD, &A);
            MatSetSizes(A, PETSC_DECIDE, PETSC_DECIDE, n, n);
            MatSetFromOptions(A);
            MatSetUp(A);

            VecCreate(PETSC_COMM_WORLD, &b);
            VecSetSizes(b, PETSC_DECIDE, n);
            VecSetFromOptions(b);
            VecSetUp(b);

            VecDuplicate(b, &x);

            MatGetOwnershipRange(A, &start, &end);
            for (i = start; i < end; i++) {
                for (j = start; j < end; j++) {
                    MatSetValue(A, i, j, matrixA[i][j], INSERT_VALUES);
                }
            }
            MatAssemblyBegin(A, MAT_FINAL_ASSEMBLY);
            MatAssemblyEnd(A, MAT_FINAL_ASSEMBLY);
            // MatView(A, PETSC_VIEWER_STDOUT_WORLD);

            for (i = start; i < end; i++) {
                VecSetValue(b, i, vectorB[i], INSERT_VALUES);
            }
            VecAssemblyBegin(b);
            VecAssemblyEnd(b);
            // VecView(b, PETSC_VIEWER_STDOUT_WORLD);

            // // Set the right-hand side vector
            // VecSet(b, 1.0);

            // Create the linear solver context
            KSP ksp;
            KSPCreate(PETSC_COMM_WORLD, &ksp);
            KSPSetOperators(ksp, A, A);
            //   KSPSetType(ksp, KSPCG); // Set the solver type to CG (Conjugate Gradient)
            KSPSetType(ksp, KSPIBCGS);
            KSPSetFromOptions(ksp);

            // Set the tolerances
            KSPSetTolerances(ksp, 1e-6, PETSC_DEFAULT, PETSC_DEFAULT, PETSC_DEFAULT);

            auto beginTime  = std::chrono::high_resolution_clock::now();
            // Solve the linear system
            KSPSolve(ksp, b, x);
            auto endTime  = std::chrono::high_resolution_clock::now();
            auto totalTime = endTime - beginTime;

            outputFile << std::chrono::duration_cast<std::chrono::microseconds>(endTime-beginTime).count() << std::endl;

            std::cout << "Time taken to solve the system: " << std::chrono::duration_cast<std::chrono::microseconds>(endTime-beginTime).count() << " micro-seconds" << std::endl;

            // Get the number of iterations and the residual norm
            PetscInt iterations;
            PetscReal residual_norm;
            KSPGetIterationNumber(ksp, &iterations);
            KSPGetResidualNorm(ksp, &residual_norm);

            // Print the result
            PetscPrintf(PETSC_COMM_WORLD, "Number of iterations = %d\n", iterations);
            PetscPrintf(PETSC_COMM_WORLD, "Residual norm = %e\n", residual_norm);

            // Print the solution vector x
            PetscPrintf(PETSC_COMM_WORLD, "Solution vector x:\n");
            // MatView(A, PETSC_VIEWER_STDOUT_WORLD);
            // VecView(x, PETSC_VIEWER_STDOUT_WORLD);

            // Free the resources
            KSPDestroy(&ksp);
            VecDestroy(&b);
            VecDestroy(&x);
            MatDestroy(&A);

        }
    outputFile.close();
    }
    else
    {
        std::cout << "Unable to open file";
    }
    PetscFinalize();
    return 0;
}
