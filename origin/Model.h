//
// Created by someone on 2019/12/27.
//

#ifndef IMGRENDER_MODEL_H
#define IMGRENDER_MODEL_H

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "Geomotry.h"

class Model {
public:
    Model();

    Model(const char *fileName);

    bool readFromFile(const char *fileName);

    inline vec4f getFaceVecs(int faceIndex, int index) {
        return points[faces[faceIndex][index]];
    }

    inline int getFacesCount() {
        return faces.size();
    }

    inline int getPointsCount() {
        return points.size();
    }

private:
    std::vector<vec4f> points;

    bool handleOutputData(std::vector<std::string>);

    std::vector<std::vector<int>> faces;
};


#endif //IMGRENDER_MODEL_H
