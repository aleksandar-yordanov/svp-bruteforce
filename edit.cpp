#include "includes.h"

// based on a solution on stackoverflow
std::vector<std::string> split(const std::string input, std::string delimiter) {
    std::vector<std::string> retVec;
    size_t startPos = 0, endPos, delimLength = delimiter.length();
    std::string curElement;
    while ((endPos = input.find(delimiter, startPos)) != std::string::npos) {
        curElement = input.substr(startPos, endPos - startPos);
        startPos = endPos + delimLength;
        retVec.push_back(curElement);
    }
    retVec.push_back(input.substr(startPos));
    return retVec;
}

void fixString(std::string& input) {
    std::erase(input, '[');
    std::erase(input, ']');
}

int main(int argc, char* argv[]) {
    std::string input = "";
    // Concatentate each argument to input
    for (int i = 1; i < argc; i++) {
        if (i != (argc - 1)) {
            input += (std::string(argv[i]) + ' ');
        } else {
            input += std::string(argv[i]);
        }
    }
    std::vector<std::string> vecs = split(input, "] [");
    std::vector<std::vector<std::string>> finalVec;
    int dim = vecs.size();
    for (auto &e : vecs) {
        // Erase any remaining [ or ] chars remaining
        // and split on " " to get each basis vector
        fixString(e);
        std::vector<std::string> tempVec = split(e, " ");
        finalVec.push_back(tempVec);
    }
    for (auto &vec : finalVec) {
        if (vec.size() != finalVec.size()) {
            std::cout << "Invalid vector sizes, Exiting..." << std::endl;
            return 1;
        }
    }
    if (finalVec.size() == 1) {
        // If 1 dimensional vector, the shortest vector will be the basis vector
        std::ofstream results("result.txt");
        results << finalVec[0][0];
        results.close();
        return 0;
    }
    std::string baseStr = "{";
    // Below loops convert input args to format compilable in C++
    // Format <std::string>: "{Vector({v1n1, v1n2}), Vector({v2n1, v2n2})};"
    // Also checks if any of the args cannot be converted to a double
    for (auto vector = finalVec.begin(); vector != finalVec.end(); ++vector) {
        std::string curBase = "Vector({";
        for (auto e = vector->begin(); e != vector->end(); ++e) {
            if (std::next(e) != vector->end()) {
                try {
                    std::stod(*e);
                } catch (std::exception) {
                    std::cout << "Cannot convert input to double" << std::endl;
                    return 1;
                }
                curBase+= *e + ", ";
            } else {
                try {
                    std::stod(*e);
                } catch (std::exception) {
                    std::cout << "Cannot convert input to double" << std::endl;
                    return 1;
                }
                curBase+= *e;
            }
        }
        if (std::next(vector) != finalVec.end()) {
            curBase += "}), ";
        } else {
            curBase += "})};";
        }
        baseStr += curBase;
    }
    // Here we write our formatted args and dimension to our header files
    std::ofstream dimfile("dim.h");
    std::ofstream datafile("data.h");
    dimfile << "#define DIM " + std::to_string(dim);
    datafile << "#define DATA " + baseStr;
    dimfile.close();
    datafile.close();
    // Recompile and run implementation with formatted args and dimension.
    system("make main");
    system("make clean");
    system("./main");
    return 0;
}
