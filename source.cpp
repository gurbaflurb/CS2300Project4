/*
Sean Chen
Last modified: Nov. 22, 2019
Written for CS 2300 Sec. 002
source.cpp

Desc: This program reads in two data files, one, the independent data file, and the other is the dependent data file. These data files are then used
to calculate a linear least squares regression for the first part, and then a quadratic least squares regression. Once these have been calculated, 
they are then tested with a 15 year mark to see the predicition of the salary at year 15.
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <tuple>
#include <string>
#include "lib/matrix.h"

/*
readData function reads in data from two provided files into two seperate vectors. Throws and invalid_argument exception if one or more files could
not be opened
*/
std::tuple<std::vector<float>,std::vector<float>> readData(std::string fileName1, 
                                                           std::string fileName2) {
    std::vector<float> matrix1, matrix2;
    std::fstream file;
    std::fstream file2;
    std::string line;

    file.open(fileName1);
    file2.open(fileName2);

    if(!file.is_open() || !file2.is_open()) {
        throw std::invalid_argument("One or more files could not be opened!\n");
    }
    while(!file.eof()) {
        std::getline(file, line, '\n');
        matrix1.push_back(std::stof(line));
        std::getline(file2, line, '\n');
        matrix2.push_back(std::stof(line));
    }

    return std::make_tuple(matrix1, matrix2);
}

/*
findMean function takes in a vector<float> as a parameter and finds the mean of the values in said vector 
*/
float findMean(std::vector<float> matrix) {
    float mean = 0.0;
    int totalElements = 0;
    for(std::vector<float>::iterator it = matrix.begin(); it != matrix.end(); it++) {
        mean += *it;
        totalElements++;
    }

    return mean/totalElements;
}

/*
findDistance function takes in a vector<float> and a mean of type float as a parameter and returns a new vector by subtracting all values from the
matrix from the mean and returns a vector<float> with all the new values
*/
std::vector<float> findDistance(std::vector<float> matrix, float mean) {
    std::vector<float> newMatrix;
    for (std::vector<float>::iterator it = matrix.begin(); it != matrix.end(); it++) {
        newMatrix.push_back(*it-mean);
    }
    return newMatrix;
}

/*
sumValues function takes a matrix and returns all the values in the matrix added together
*/
float sumValues(std::vector<float> matrix) {
    float sum = 0;
    for(std::vector<float>::iterator it = matrix.begin(); it != matrix.end(); it++) {
        sum += *it;
    }
    return sum;
}

/*
findSumOfXSquared function squares all the values in a given vector and then sums them all up and returns the floating point value
*/
float findSumOfXSquared(std::vector<float> matrix) {
    std::vector<float> tempMatrix;
    for(std::vector<float>::iterator it = matrix.begin(); it != matrix.end(); it++) {
        tempMatrix.push_back((*it)*(*it));
    }

    return sumValues(tempMatrix);
}

/*
findSumOfXDistTimesYDist function multiplies all X Distances by its cooresponding Y Distance in its given two vectors and then returns all the
values summed together
*/
float findSumOfXDistTimesYDist(std::vector<float> XMatrix, std::vector<float> YMatrix) {
    std::vector<float>::iterator XIter = XMatrix.begin();
    std::vector<float>::iterator YIter = YMatrix.begin();
    std::vector<float> tempMatrix;
    for(XIter; XIter != XMatrix.end(); XIter++) {
        tempMatrix.push_back((*XIter) * (*YIter));
        YIter++;
    }
    
    return sumValues(tempMatrix);
}

/*
solveB0B1 function takes in the meanX, meanY, sumX, and sumY and solves for b0 and b1 and returns them as a tuple to the user.
*/
std::tuple<float, float> solveB0B1(float meanX, float meanY, float sumX, float sumY) {
    float b0, b1;
    b1 = sumY/sumX;
    b0 = meanY-(b1*meanX);
    return std::make_tuple(b0, b1);
}

/*
buildMatrix function builds the matrix to specification for this assignment. This function takes in the two vectors from the local readData function
and builds a matrix based on the form x^2, x, 1, y.
*/
std::vector<float> buildMatrix(std::vector<float> independentMatrix, std::vector<float> dependentMatrix) {
    std::vector<float> returnMatrix;
    std::vector<float> tempMatrix;
    std::vector<float>::iterator XIter = independentMatrix.begin();
    std::vector<float>::iterator YIter = dependentMatrix.begin();
    std::vector<float>::iterator tempX = XIter;
    std::vector<float>::iterator tempY = YIter;
    float tempSum = 0;

    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            for(tempX; tempX != independentMatrix.end(); tempX++) {
                tempMatrix.push_back(pow(*tempX, j+i+i+i));
            }
            tempX = XIter;
            tempSum = sumValues(tempMatrix);
            returnMatrix.push_back(tempSum);
            tempMatrix.clear();
        }
        for(tempY; tempY != dependentMatrix.end(); tempY++) {
            tempMatrix.push_back(pow(*tempX, i)*(*tempY));
            tempX++;
        }
        tempX = XIter;
        tempY = YIter;
        tempSum = sumValues(tempMatrix);
        returnMatrix.push_back(tempSum);
        tempMatrix.clear();
    }
    return returnMatrix;
}

/*
solveQuadratic function takes in a list of quadratic coefficients, the dimensions, and the X value and attempts to solve the quadratic 
*/
double solveQuadratic(std::vector<float> matrix, int dimensions, float X) {
    double value;
    std::vector<float>::iterator iter = matrix.begin();
    
    for(int i = dimensions-1; i > -1; i--) {
        value += (*iter)*(pow(X, i));
        iter++;
    }
    return value;
}

/*
start of main
*/
int main(int argc, char **argv) {
    matrixMath::parseArgs(argc, argv);
    float meanX, meanY, sumX, sumY;
    float y, x, b0, b1;
    std::vector<float> independantVars, dependentVars;
    std::vector<float> XDistances, YDistances;
    std::vector<float> quadraticMatrix;
    std::vector<float> quadraticSolved;
    try
    {
        std::tie(independantVars, dependentVars) = readData("i_data.txt", "d_data.txt");
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        exit(1);
    }

    meanX = findMean(independantVars);
    meanY = findMean(dependentVars);
    XDistances = findDistance(independantVars, meanX);
    YDistances = findDistance(dependentVars, meanY);
    sumX = findSumOfXSquared(XDistances);
    sumY = findSumOfXDistTimesYDist(XDistances, YDistances);

    std::tie(b0, b1) = solveB0B1(meanX, meanY, sumX, sumY);
    std::cout << "Equaltion:\ny = " << b1 << "x + " << b0 << std::endl;
    std::cout << "15 years projected salary: $" << (b1 * 15 +b0) << std::endl;
    std::cout << std::endl;
    
    // Quadratic
    quadraticMatrix = buildMatrix(independantVars, dependentVars);
    quadraticSolved = matrixMath::runGaussian("Quadratic", quadraticMatrix, 4);
    double value = solveQuadratic(quadraticSolved, 3, 0);
    std::cout << "Quadratic\n15 years projected salary: $"<< value/1000 <<std::endl;

    return 0;
}