//
// Created by someone on 2019/12/29.
//
# include <vector>
# include <math.h>

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

    inline float getLength(){return sqrt(x*x+y*y+z*z);}
    inline vec4f gerNor(){float l = getLength();return vec4f(x/l,y/l,z/l,w);}
    inline vec4f operator +(const vec4f &v) { return vec4f(x+v.x,y+v.y,z+v.z,w+v.w);}
    inline vec4f operator -(const vec4f &v) { return vec4f(x-v.x,y-v.y,z-v.z,w-v.w);}
    inline float operator *(const vec4f &v) { return x*v.x+y*v.y+z*v.z+w*v.w;}
    inline vec4f operator *(float v) { return vec4f(x*v,y*v,z*v,w*v);}
    inline vec4f operator ^(const vec4f &v) { return vec4f(y*v.z-z*v.y,z*v.x-x*v.z,x*v.y-y*v.x,0.);}

};

struct vec2i {
    int x;
    int y;

    vec2i() : x(0.), y(0.) {

    }

    vec2i(int x, int y) : x(0.), y(0.) {
        this->x = x;
        this->y = y;
    }

    inline vec2i operator -(const vec2i &v) {
        return vec2i(x-v.x,y-v.y);
    }

    inline vec2i operator +(const vec2i &v) { return vec2i(x+v.x,y+v.y);}

};

#endif //IMGRENDER_GEOMOTRY_H
