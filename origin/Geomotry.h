//
// Created by someone on 2019/12/29.
//
# include <vector>

#ifndef IMGRENDER_GEOMOTRY_H
#define IMGRENDER_GEOMOTRY_H

struct vec4f {
    float x;
    float y;
    float z;
    float w;

    vec4f(float x, float y, float z, float w) : x(0.), y(0.), z(0.), w(0.) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }
};

struct vec2i {
    int x;
    int y;

    vec2i(int x, int y) : x(0.), y(0.) {
        this->x = x;
        this->y = y;
    }
};

#endif //IMGRENDER_GEOMOTRY_H
