#include <iostream>
#include "origin/tgaimage.h"
#include "origin/Model.h"
#include <math.h>

#include "origin/render.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);

const float PI = atan(1.0) * 4;

int main(int argc, char **argv) {

    TGAImage image(1000, 1000, TGAImage::RGBA);
//    initZBuffer();
//    drawMdTriWithZ(image);

//    drawMdTriWithG(image);
//    image.set(558,819,TGAColor(0,0,0,0));
//    drawZbufferImg(image);
//    drawMdTriangle(image);
//    drawCircleTriangle(image);

//    TGAImage tex;
//    tex.read_tga_file("Resource/diablo3_pose_diffuse.tga");
//    for(int i=0;i<WIDTH;i++){
//        for(int j=0;j<HEIGHT;j++){
//            image.set(i,j,tex.get(i,j));
//        }
//    }
    renderer rd;
    rd.init(image);

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output/output3.tga");
    std::system("open output/output3.tga");
    return 0;
}
