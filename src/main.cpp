#include <armadillo>
#include <fstream>
#include <iostream>
#include <petscksp.h>
#include <sstream>
#include <vector>
#include <vector>

int main (int arg, char *argv[])
{
    std::string filePath = "./data/matrixA10.csv";
    std::string matrixPath = "./data/matrixA10.csv";
    std::string vectorPath = "./data/matrixb10.csv";

    std::vector<std::vector<double>> matrixA;
    std::vector<double> vectorB;

    // matrixA = getMatrix(matrixPath);
    // vectorB = getVector(vectorPath);


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
    return 0;
};

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