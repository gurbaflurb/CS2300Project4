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
std::vector<float> buildMatrix(std::vector<float> independentMatrix, std::vector<float> dependentMatrix, int numSize) {
    std::vector<float> returnMatrix;
    std::vector<float>::iterator XIter = independentMatrix.begin();
    std::vector<float>::iterator YIter = dependentMatrix.begin();

    for(XIter; XIter != independentMatrix.end(); XIter++) {
        for(int i = numSize-1; i >0; i--) {
            returnMatrix.push_back(pow((*XIter), i));
        }
        returnMatrix.push_back(pow(*XIter, 0));
        returnMatrix.push_back(*YIter);
        YIter++;
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
        std::cout << *iter << "*(" << X << ")^" << i<< " + ";
        iter++;
    }
    
    std::cout << std::endl;

    return value;
}

/*
c = { [ Σ (x^2 *y) * Σ xx ] - [Σ xy * Σ xx^2 ] } /  { [ Σ xx * Σ x^2x^2] - [Σ xx^2 ]^2 }
solveC function takes in a matrix of the known x values, and of known y values. It then performs a set of equations to solve for the variable c in 
the quadratic formula
*/
float solveC(std::vector<float> XMatrix, std::vector<float> YMatrix) {
    float c;
    std::vector<float>::iterator XIter = XMatrix.begin(), YIter = YMatrix.begin();
    std::vector<float> temp, temp2, temp3, temp4, temp5, temp6, temp7;
    //[ Σ (x^2 *y) * Σ xx ]
    for(std::vector<float>::iterator xit = XIter, yit = YIter; xit != XMatrix.end(); xit++){
        temp.push_back(pow(*xit, 2)*(*yit));
        yit++;
    }
    for(std::vector<float>::iterator xit = XIter; xit != XMatrix.end(); xit++){
        temp2.push_back((*xit)*(*xit));
    }

    // [Σ xy * Σ xx^2 ]
    for(std::vector<float>::iterator xit = XIter, yit = YIter; xit != XMatrix.end(); xit++){
        temp3.push_back((*xit)*(*yit));
        yit++;
    }
    for(std::vector<float>::iterator xit = XIter; xit != XMatrix.end(); xit++){
        temp4.push_back(*xit*pow(*xit, 2));
    }

    // [ Σ xx * Σ x^2x^2]
    for(std::vector<float>::iterator xit = XIter; xit != XMatrix.end(); xit++){
        temp5.push_back((*xit)*(*xit));
    }
    for(std::vector<float>::iterator xit = XIter; xit != XMatrix.end(); xit++){
        temp6.push_back(pow(*xit, 2)*pow(*xit, 2));
    }

    // [Σ xx^2 ]^2
    for(std::vector<float>::iterator xit = XIter; xit != XMatrix.end(); xit++){
        temp7.push_back((*xit)*pow(*xit, 2));
    }
    // c = { [ Σ (x^2 *y) * Σ xx ] - [Σ xy * Σ xx^2 ] } /  { [ Σ xx * Σ x^2x^2] - [Σ xx^2 ]^2 }
    c = ((sumValues(temp)*sumValues(temp2)) - (sumValues(temp3)*sumValues(temp4)))/((sumValues(temp5)*sumValues(temp6))-(pow(sumValues(temp7), 2)));
    return c;
}

/*
b = { [ Σ xy * Σ x^2x^2 ] - [Σ x^2y * Σ xx^2 ] } /  { [ Σ xx * Σ x^2x^2] - [Σ xx^2 ]^2 }
solveB function takes in a matrix with all the known x values and a matrix with all known y values, and performs a set of equations to solve for the
b variable.
*/
float solveB(std::vector<float> XMatrix, std::vector<float> YMatrix) {
    float b;

    return b;
}

/*
a =  [ Σ y / n ] - { b *  [ Σ x / n ] } -  { a * [ Σ x^2  / n ]  }
solveA function takes in a matrix with all the known values of x, and a matrix with all the known y values, the number of elements total, the B variable

*/

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
    
    // Quadratic Time
    // What the formula should be in the end: -505.682 X^2 +16792.803 X -20891.667
    quadraticMatrix = buildMatrix(independantVars, dependentVars, 10);
    std::cout << "Quadratic\n15 years projected salary: $" << solveC(independantVars, dependentVars) << std::endl;

    return 0;
}