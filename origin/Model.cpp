//
// Created by someone on 2019/12/27.
//

#include "Model.h"
#include "GlobalFunctions.h"

bool Model::readFromFile(const char *fileName) {
    std::string line;
    std::ifstream in;
    in.open(fileName, std::ios::in);
    if (!in.is_open()) {
        in.close();
        std::cerr << "can't open file " << fileName << "\n";
        return false;
    }
    while (std::getline(in, line)) {
        auto strVec = splice(line, " ");
        this->handleOutputData(strVec);
    }
    float a = 1;
}

Model::Model() {

}

Model::Model(const char *fileName) {

}

bool Model::handleOutputData(std::vector<std::string> strVec) {
    if (strVec[0] == "v") {
        this->points.push_back(vec4f(str2float(strVec[1]), str2float(strVec[2]), str2float(strVec[3]), 0.));
    } else if (strVec[0] == "f") {
//        std::cout<<strVec[1]<<"\n";
        auto strArg = splice(strVec[1], "/");
        std::vector<int> val;
        val.push_back(str2int(strArg[0]) - 1);
        val.push_back(str2int(strArg[1]) - 1);
        val.push_back(str2int(strArg[2]) - 1);
        this->faces.push_back(val);
    }
    return false;
}
