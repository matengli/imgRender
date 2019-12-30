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

