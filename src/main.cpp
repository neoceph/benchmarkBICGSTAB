// standard headers
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

// third party headers
#include <armadillo>
#include <Eigen/Sparse>
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


// main method

// int main (int arg, char *argv[])
// {
//     std::string matrixPath = "./data/matrixA10.csv";
//     std::string vectorPath = "./data/matrixb10.csv";

//     std::vector<std::vector<double>> matrixA;
//     std::vector<double> vectorB;

//     matrixA = getMatrix(matrixPath);
//     vectorB = getVector(vectorPath);

//     // PetSc begginings

//     PetscInitialize(&arg, &argv, NULL, NULL);

//     Mat A;
//     Vec x, b, r;
//     KSP solver;
//     PC preconditioner;

//     // Create the matrix
//     MatCreate(PETSC_COMM_WORLD, &A);
//     MatSetSizes(A, PETSC_DECIDE, PETSC_DECIDE, 10, 10);
//     MatSetFromOptions(A);
//     MatSetUp(A);


//     return 0;


// };




int main(int argc, char **argv) {
  PetscInitialize(&argc, &argv, NULL, NULL);

  // Create the matrix and vector
  Mat A;
  Vec b, x;
  PetscInt n = 100;  // Size of the matrix
  PetscInt i, start, end;

  MatCreate(PETSC_COMM_WORLD, &A);
  MatSetSizes(A, PETSC_DECIDE, PETSC_DECIDE, n, n);
  MatSetFromOptions(A);
  MatSetUp(A);

  VecCreate(PETSC_COMM_WORLD, &b);
  VecSetSizes(b, PETSC_DECIDE, n);
  VecSetFromOptions(b);
  VecSetUp(b);

  VecDuplicate(b, &x);

  // Set the matrix coefficients
  MatGetOwnershipRange(A, &start, &end);
  for (i = start; i < end; i++) {
    if (i > 0) {
      MatSetValue(A, i, i - 1, -1.0, INSERT_VALUES);
    }
    MatSetValue(A, i, i, 2.0, INSERT_VALUES);
    if (i < n - 1) {
      MatSetValue(A, i, i + 1, -1.0, INSERT_VALUES);
    }
  }
  MatAssemblyBegin(A, MAT_FINAL_ASSEMBLY);
  MatAssemblyEnd(A, MAT_FINAL_ASSEMBLY);

  // Set the right-hand side vector
  VecSet(b, 1.0);

  // Create the linear solver context
  KSP ksp;
  KSPCreate(PETSC_COMM_WORLD, &ksp);
  KSPSetOperators(ksp, A, A);
//   KSPSetType(ksp, KSPCG); // Set the solver type to CG (Conjugate Gradient)
  KSPSetType(ksp, KSPIBCGS);
  KSPSetFromOptions(ksp);

  // Set the tolerances
  KSPSetTolerances(ksp, 1e-6, PETSC_DEFAULT, PETSC_DEFAULT, PETSC_DEFAULT);

  // Solve the linear system
  KSPSolve(ksp, b, x);

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
  VecView(x, PETSC_VIEWER_STDOUT_WORLD);

  // Free the resources
  KSPDestroy(&ksp);
  VecDestroy(&b);
  VecDestroy(&x);
  MatDestroy(&A);
  PetscFinalize();

  return 0;
}
