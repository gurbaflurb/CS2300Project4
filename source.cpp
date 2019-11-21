/*
Sean Chen
Last modified: Nov. 21, 2019
Written for CS 2300 Sec. 002
source.cpp

Desc: 
*/

#include <iostream>
#include <vector>
#include <fstream>
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
    std::vector<float>::iterator XIter = independentMatrix.begin();
    std::vector<float>::iterator YIter = dependentMatrix.begin();

    for(XIter; XIter != independentMatrix.end(); XIter++) {
        returnMatrix.push_back((*XIter) * (*XIter));
        returnMatrix.push_back(*XIter);
        returnMatrix.push_back(1);
        returnMatrix.push_back(*YIter);
        YIter++;
    }
    return returnMatrix;
}

/*
buildQuadratic function takes in a matrix built by build matrix and returns a vector of values out of said matrix
*/
//MIGHT NOT NEED WE WILL SEE ------------------------------------------------------------------------------------
std::vector<float> buildQuadratic(std::vector<float> matrix) {
    std::vector<float> newMatrix;

    return newMatrix;
}


/*
start of main
*/
int main(int argc, char **argv) {
    float meanX, meanY, sumX, sumY;
    float y, x, b0, b1;
    std::vector<float> independantVars, dependentVars;
    std::vector<float> XDistances, YDistances;
    std::vector<float> qudraticMatrix;
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
    
    // r^2Distance between regression line values and the actual values
    // Quadratic Time
    // Make a vector filled with the points in the form of a qudratic(Ax^2 + Bx + C = Y)
    // So from the data from the files, the first thing would be A(1)^2 + B(1) + C = 5000.
    // Do this for all the values
    // The Matrix Should look like:
    /* [ 1  1  1  5000]
       [ 4  2  1  7500]
       [ 9  3  1  15000]
       [ 16  4  1  20000]
       [ 25  5  1  66000]
       [ 36  6  1  72000]
       [ 49  7  1  74500]
       [ 64  8  1  80000]
       [ 81  9  1  82000]
       [ 100  10  1  98000]
    */
    qudraticMatrix = buildMatrix(independantVars, dependentVars);
    matrixMath::printMatrix(qudraticMatrix, 3);


    return 0;
}