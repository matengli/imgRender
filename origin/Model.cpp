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
    while (!in.eof()) {
        std::getline(in, line);
        std::stringstream str(line);
        char trash;
        if(!line.compare(0,2,"v ")){
            float x,y,z;
            str>>trash>>x>>y>>z;
            points.push_back(vec4f(x,y,z,0.));
        } else if (!line.compare(0,2,"f ")){
            str>>trash;

            std::vector<int> val;
            int ival, itrash;
            char ctrash;
            while (str>>ival>>ctrash>>itrash>>ctrash>>itrash) {
                val.push_back(ival-1);
            }
            faces.push_back(val);
        }
    }
}

Model::Model() {

}

Model::Model(const char *fileName) {

}

bool Model::handleOutputData(std::vector<std::string> strVec) {
//    if (strVec[0] == "v") {
//        this->points.push_back(vec4f(str2float(strVec[1]), str2float(strVec[2]), str2float(strVec[3]), 0.));
//    } else if (strVec[0] == "f") {
//
//        std::vector<int> val;
//
//        for (int i = 0; i < 3; i++) {
//            std::istringstream iss(strVec[i+1]);
//            int ival, itrash;
//            char ctrash;
//            while (iss>>ival>>ctrash>>itrash>>ctrash>>itrash) {
//                val.push_back(ival-1);
//            }
//        }
//        this->faces.push_back(val);
//    }
//    return false;
}
