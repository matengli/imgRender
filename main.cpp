#include <iostream>
#include "origin/tgaimage.h"
#include "origin/Model.h"
#include <math.h>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

const float PI = atan(1.0) * 4;

const int WIDTH = 1000;
const int HEIGHT = 1000;

void drawLine(TGAImage &image, int x1, int y1, int x2, int y2) {
    bool isSteep = false;
    if (std::abs(x1 - x2) < std::abs(y1 - y2)) {
        isSteep = true;
    }
    if (!isSteep) {
        if (x1 >= x2) {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }
    } else {
        if (y1 >= y2) {
            std::swap(y1, y2);
            std::swap(x1, x2);
        }
    }

    int start = isSteep ? y1 : x1;
    int end = isSteep ? y2 : x2;

    int _start = isSteep ? x1 : y1;
    int _end = isSteep ? x2 : y2;

    int pos[2];
    for (int i = start; i <= end; i++) {
        float t = (i - start) / (float) (end - start);
        int other = end - start > 0 ? _start + (_end - _start) * t : _start;
        pos[0] = isSteep ? other : i;
        pos[1] = isSteep ? i : other;
        image.set(pos[0], pos[1], red);
    }
}

void drawLine(TGAImage &image, vec2i p1, vec2i p2) {
    drawLine(image, p1.x, p1.y, p2.x, p2.y);
}

vec2i getScreenVec(Model &md, int faceIndex, int index) {
    vec4f _p = md.getFaceVecs(faceIndex, index);
    vec2i p = vec2i((_p.x + 1.) * (WIDTH / 2.), (_p.y + 1.) * (HEIGHT / 2.));
    return p;
}

void drawMdFrame(TGAImage &image) {
    Model md;
    md.readFromFile("Resource/diablo3_pose.obj");
    for (int i = 0; i < md.getFacesCount(); i++) {
        drawLine(image, getScreenVec(md, i, 0), getScreenVec(md, i, 1));
        drawLine(image, getScreenVec(md, i, 1), getScreenVec(md, i, 2));
        drawLine(image, getScreenVec(md, i, 2), getScreenVec(md, i, 0));
    }
}

void drawCircleLine(TGAImage &image) {
    float length = 300.;
    for (int i = 0; i <= 360; i += 1) {
        float deg = i / 180. * PI;
        vec2i endP = vec2i(WIDTH / 2. + length * cos(deg), HEIGHT / 2. + length * sin(deg));
        drawLine(image, vec2i(WIDTH / 2, HEIGHT / 2), endP);
    }
}

int main(int argc, char **argv) {

    TGAImage image(WIDTH, HEIGHT, TGAImage::RGB);

    drawMdFrame(image);
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output/output.tga");
    std::system("open output/output.tga");
    return 0;
}
