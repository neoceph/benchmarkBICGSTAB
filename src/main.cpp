// standard headers
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

// third party headers
#include <armadillo>
#include <Eigen/Sparse>
#include <petscksp.h>

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

int add(int a, int b) 
{
return a + b;
};


// main method

int main (int arg, char *argv[])
{
    std::string filePath = "./data/matrixA10.csv";
    std::string matrixPath = "./data/matrixA10.csv";
    std::string vectorPath = "./data/matrixb10.csv";

    std::vector<std::vector<double>> matrixA;
    std::vector<double> vectorB;

    matrixA = getMatrix(matrixPath);
    vectorB = getVector(vectorPath);


    std::ifstream  file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return 1;
    }
    std::string line;
    std::vector<std::vector<std::string> > parsedCsv;
    while(std::getline(file,line))
    {
        std::stringstream lineStream(line);
        std::string cell;
        std::vector<std::string> parsedRow;
        while(std::getline(lineStream,cell,','))
        {
            parsedRow.push_back(cell);
        }

        parsedCsv.push_back(parsedRow);
    }
    float result = add(4, 5);
    std::cout << "Sum of two numbers: " << result << std::endl;

    return 0;


};
