#include "includes.h"
#include "dim.h"
#include "data.h"

struct Vector{
    std::array<double, DIM> points;
    explicit Vector(std::array<double, DIM> inlist) : points(inlist) {}
    explicit Vector(std::initializer_list<double> vals) {
        std::copy(vals.begin(), vals.end(), points.begin());
    }
    Vector() {
        points.fill(0);
    }
    inline int dimension() const {
        return points.size();
    }
    double euclidNorm() const {
        double sum = 0;
        // Same speed as standard for loops when using -Ofast
        for (const double& i : points) {
            sum += i*i;
        }
        return std::sqrt(sum);
    }
    bool isCenterPoint() const {
        for (int i = 0; i < DIM; i++) {
            if (this->points[i] != 0) {
                return false;
            }
        }
        return true;
    }
    inline double operator[](const int& idx) const {
        return points[idx];
    }
    friend std::ostream& operator<<(std::ostream& os, const Vector& vec) {
        os << '(';
        for (const double& dim : vec.points) {
            os << dim << ',';
        }
        os << ')';
        return os;
    }
};

std::array<double, DIM * DIM> flattenAndInvertVector(std::array<Vector, DIM> vecs) {
    // flattens and swaps rows and columns of basis matrix
    std::array<double, DIM * DIM> retArr;
    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            retArr[i * DIM + j] = vecs[j][i];
        }
    }
    return retArr;
}

void nextLoop(const std::array<double,DIM * DIM>& basisVectors,
    std::array<int,DIM>& counters, int index,
    int radius, double& minDist) {
    if (index == DIM) {
        Vector closePoint;
        for (int curDim = 0; curDim < DIM; curDim++) {
            for (int coefficient = 0; coefficient < DIM; coefficient++) {
                // Calculates one part of the linear combination and adds it.
                int cf = counters[coefficient];
                double vecval = basisVectors[curDim * DIM + coefficient];
                closePoint.points[curDim] += cf * vecval;
            }
        }
        if (!closePoint.isCenterPoint()) {
            // Calculates euclidian norm and sets minDist if its smaller
            double dist = closePoint.euclidNorm();
            if (dist < minDist) {
                minDist = dist;
            }
        }
        return;
    }
    for (counters[index] = -radius; counters[index] <= radius; ++counters[index]) {
        // Go to next dimension and calculate linear combinations
        // Only take values less than counters[0] to limit to diagonal
        if (counters[index] <= counters[0]) {
            nextLoop(basisVectors, counters, index + 1, radius, minDist);
        }
    }
}

double getShortestVector(const std::array<double,DIM * DIM>& basisVectors,
    int radius) {
    // Creating array of counters for n nested loops depending on dim.
    // Exponential time complexity based of this alone
    std::array<int, DIM> counters;
    counters.fill(0);
    double minDist = std::numeric_limits<double>::max();
    nextLoop(basisVectors, counters, 0, radius, minDist);
    return minDist;
}

int main() {
    // Compiled with data values made in edit.cpp {#DEFINE DATA ..}
    std::array<Vector, DIM> preProc = DATA
    std::array<double, DIM * DIM> basisVectors = flattenAndInvertVector(preProc);
    int radius = 1;
    // optimised radius values based on execution time under 90~ seconds
    switch (DIM) {
        case 10:
            radius = 5;
            break;
        case 9:
            radius = 6;
            break;
        case 8:
            radius = 8;
            break;
        case 7:
            radius = 11;
            break;
        case 6:
            radius = 20;
            break;
        case 5:
            radius = 45;
            break;
        case 4:
            radius = 150;
            break;
        case 3:
            radius = 1400;
            break;
        case 2:
            radius = 60000;
            break;
        default:
            radius = 2;
            break;
    }
    double shortestVec = getShortestVector(basisVectors, radius);
    // Outputs and writes to file
    std::cout << shortestVec << std::endl;
    std::ofstream file("result.txt");
    file << shortestVec;
    file.close();
    return 0;
}
