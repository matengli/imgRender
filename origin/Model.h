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
#include "tgaimage.h"

class Model {
public:
    Model();

    Model(const char *fileName);

    bool readFromFile(const char *fileName);

    inline vec4f getFaceVecs(int faceIndex, int index) {
        return verts[faces[faceIndex][index]];
    }

    inline vec4f getUvVecs(int uvIndex, int index) {
        return uvverts[uvs[uvIndex][index]];
    }

    inline int getFacesCount() {
        return faces.size();
    }

    inline int getPointsCount() {
        return verts.size();
    }

    TGAColor getDiffuseColor(int uvIndex, int index){
        auto uv = getUvVecs(uvIndex, index);
        int width = diffuseTexture.get_width();
        int height = diffuseTexture.get_height();
        int x = (int)(uv.x*width);
        int y = (int)(uv.y*height);
        return diffuseTexture.get(x,y);
    }

    inline TGAColor getDiffuseColorByUv(vec4f uv){
        int width = diffuseTexture.get_width();
        int height = diffuseTexture.get_height();
        int x = (int)(uv.x*width);
        int y = (int)(uv.y*height);
        return diffuseTexture.get(x,y);
    }

    bool readDiffTextureFromFile(const char *fileName){
        diffuseTexture.read_tga_file(fileName);
        diffuseTexture.flip_vertically();
        return true;
    }

private:
    TGAImage diffuseTexture;
    std::vector<vec4f> verts;
    std::vector<vec4f> uvverts;

    bool handleOutputData(std::vector<std::string>);

    std::vector<std::vector<int>> faces;
    std::vector<std::vector<int>> uvs;
};


#endif //IMGRENDER_MODEL_H
