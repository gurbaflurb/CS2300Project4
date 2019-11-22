/*
This is the headerfile for matrix.cpp which was copied from my CS2300Project3 source file and modified slightly to work as a library instead of being
a source file.
*/
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <cmath>
#include <tuple>

namespace matrixMath {
    std::vector<float> makeRow(std::vector<float>::iterator firstColumn, std::vector<float>::iterator secondColumn, int dimensions, int passThrough);
    std::tuple<std::vector<float>, int> readData(std::string fileName, std::string productFileName);
    std::vector<float> reduce(std::vector<float> matrix, int dimensions, int passThrough);
    std::vector<float> gaussianElimination(std::vector<float> matrix, int dimensions);
    std::vector<float> solveMatrix(std::vector<float> matrix, int dimensions);

    std::vector<float> runGaussian(std::string matrixRun, std::vector<float> matrix, int dimensions);
    void printMatrix(std::vector<float> matrix, int dimensions);
    void pivot(std::vector<float> &matrix, int dimensions);
    void parseArgs(int argc, char **argv);
    void printUsage();
}
