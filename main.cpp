#include <iostream>
#include "origin/tgaimage.h"
#include "origin/Model.h"
#include <math.h>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);

const vec4f LIGHTDIR = vec4f(0,1,1,0).getNor();
const float LIGHT_INST = 2.0;

//DEFAULT
const vec4f CENTER = vec4f(0,0,0,0);
const vec4f EYE = vec4f(0,0.,1.0,0);
const vec4f UP = vec4f(0.,1.,0,0);

//CENTER = vec4f(0,0,0,0);
//EYE = vec4f(0.,0.,1.0,0);
//UP = vec4f(0.,1.,0,0);

const float PI = atan(1.0) * 4;

const int WIDTH = 1000;
const int HEIGHT = 1000;

#define REG(x) (x/180.*PI)
#define SCREENVEC2(_p) (vec2i((_p.x + 1.) * (WIDTH / 2.), (_p.y + 1.) * (HEIGHT / 2.)))

float zBuffer[WIDTH*HEIGHT];

void initZBuffer(){
    for(int i=0;i<WIDTH*HEIGHT;i++){
        zBuffer[i]=-std::numeric_limits<float>::max();
    }
}

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
    vec2i p = SCREENVEC2(_p);
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

vec4f getBaryCoord(vec2i* points, vec2i &endp){
    vec2i a = points[0]-points[2];
    vec2i b = points[1]-points[2];
    vec2i c = endp-points[2];

    float x = (c.x*b.y-c.y*b.x)/(float)(a.x*b.y-a.y*b.x);
    float y = (c.x*a.y-c.y*a.x)/(float)(b.x*a.y-b.y*a.x);
    vec4f result(x,y,1.0-x-y,0);
    if ((result.x>=0.&&result.x<=1.)&&(result.y>=0.&&result.y<=1.)&&(result.z>=0.&&result.z<=1.)){
        return result;
    } else{
        return vec4f(-1.,-1.,-1.,0);
    }
}

vec4f getBaryCoord(vec4f* points, vec4f &endp){
    vec4f a = points[0]-points[2];
    vec4f b = points[1]-points[2];
    vec4f c = endp-points[2];

    float x = (c.x*b.y-c.y*b.x)/(float)(a.x*b.y-a.y*b.x);
    float y = (c.x*a.y-c.y*a.x)/(float)(b.x*a.y-b.y*a.x);
    vec4f result(x,y,1.0-x-y,0);
    if ((result.x>=0.&&result.x<=1.)&&(result.y>=0.&&result.y<=1.)&&(result.z>=0.&&result.z<=1.)){
        return result;
    } else{
        return vec4f(-1.,-1.,-1.,0);
    }
}

void drawTriangle(TGAImage &image,vec2i* points,TGAColor color){
    int xyMinMax[4] = {points[0].x,points[0].x,points[0].y,points[0].y};
    for(int i=0;i<4;i++){
        for(int j=0;j<3;j++){
            int tval = i <= 1 ? points[j].x : points[j].y;
            bool isUpdate = (i % 2 == 0) ? xyMinMax[i] > tval : xyMinMax[i] < tval;
            xyMinMax[i] = isUpdate ? tval : xyMinMax[i];
        }
    }

    for(int x=xyMinMax[0];x<xyMinMax[1];x+=1){
        for(int y=xyMinMax[2];y<xyMinMax[3];y+=1) {
            auto end = vec2i(x,y);
            auto result = getBaryCoord(points, end);
            if(result.x>=0.&&result.y>=0.&&result.z>=0.){
                image.set(x,y,color);
            }
        }
    }
}

void drawCircleTriangle(TGAImage &image) {
    float length = 100;
    float reg =  0.;
    const int AMOUNT = 5;
    for(int j=0;j<AMOUNT;j++) {
        reg = 360./AMOUNT*(j+1);
        vec2i points[3]={};
        for (int i = 0; i < 3; i++) {
            points[i] = vec2i(cos(REG(reg))*length,sin(REG(reg))*length)+vec2i(WIDTH/2,HEIGHT/2);
            reg+=120;
        }
        drawTriangle(image,points,red);
    }
}


void drawMdTriangle(TGAImage &image) {
    Model md;
    md.readFromFile("Resource/diablo3_pose.obj");
    for (int i = md.getFacesCount()-1; i >=0 ; i--) {
        vec4f normal = vec4f(
                (md.getFaceVecs(i, 1) - md.getFaceVecs(i, 2)) ^ (md.getFaceVecs(i, 0) - md.getFaceVecs(i, 2))).getNor();
        float instensty = vec4f(0,0,2.,0)*normal;
        vec2i points[3] = {getScreenVec(md, i, 0),getScreenVec(md, i, 1),getScreenVec(md, i, 2)};
        if(instensty>0.) {
            drawTriangle(image, points, TGAColor(255 * instensty, 255 * instensty, 255 * instensty, 255));
        }
    }
}

void drawTriangle(TGAImage &image,vec4f* oriPoints,TGAColor color){
    vec2i points[3] = {SCREENVEC2(oriPoints[0]),SCREENVEC2(oriPoints[1]),SCREENVEC2(oriPoints[2])};
    int xyMinMax[4] = {points[0].x,points[0].x,points[0].y,points[0].y};
    for(int i=0;i<4;i++){
        for(int j=0;j<3;j++){
            int tval = i <= 1 ? points[j].x : points[j].y;
            bool isUpdate = (i % 2 == 0) ? xyMinMax[i] > tval : xyMinMax[i] < tval;
            xyMinMax[i] = isUpdate ? tval : xyMinMax[i];
        }
    }

    for(int x=xyMinMax[0];x<xyMinMax[1];x+=1){
        for(int y=xyMinMax[2];y<xyMinMax[3];y+=1) {
            auto end = vec2i(x,y);
            auto result = getBaryCoord(points, end);
            if(result.x>=0.&&result.y>=0.&&result.z>=0.){
                float zbuf = zBuffer[y*WIDTH+x];
                float z = result*vec4f(oriPoints[0].z,oriPoints[1].z,oriPoints[2].z,0.);
                if(z>zbuf){
                    zBuffer[y*WIDTH+x] = z;
                    image.set(x,y,color);
                }
            }
        }
    }
}

void drawTriangle(TGAImage &image,vec4f* oriPoints,vec4f* uvs,Model &md){
    vec2i points[3] = {SCREENVEC2(oriPoints[0]),SCREENVEC2(oriPoints[1]),SCREENVEC2(oriPoints[2])};
    int xyMinMax[4] = {points[0].x,points[0].x,points[0].y,points[0].y};
    for(int i=0;i<4;i++){
        for(int j=0;j<3;j++){
            int tval = i <= 1 ? points[j].x : points[j].y;
            bool isUpdate = (i % 2 == 0) ? xyMinMax[i] > tval : xyMinMax[i] < tval;
            xyMinMax[i] = isUpdate ? tval : xyMinMax[i];
        }
    }

    for(int x=xyMinMax[0];x<xyMinMax[1];x+=1){
        for(int y=xyMinMax[2];y<xyMinMax[3];y+=1) {
            auto end = vec2i(x,y);
            auto result = getBaryCoord(points, end);
            if(result.x>=0.&&result.y>=0.&&result.z>=0.){
                float zbuf = zBuffer[y*WIDTH+x];
                float z = result*vec4f(oriPoints[0].z,oriPoints[1].z,oriPoints[2].z,0.);
                vec4f uv = uvs[0]*result.x+uvs[1]*result.y+uvs[2]*result.z;
                TGAColor c = md.getDiffuseColorByUv(uv);

                if(z>zbuf){
                    zBuffer[y*WIDTH+x] = z;
                    image.set(x,y,c);
                }
            }
        }
    }
}

void drawTriangle(TGAImage &image,vec4f* oriPoints,vec4f* uvs,Model &md,float ins){
    vec2i points[3] = {SCREENVEC2(oriPoints[0]),SCREENVEC2(oriPoints[1]),SCREENVEC2(oriPoints[2])};
    int xyMinMax[4] = {points[0].x,points[0].x,points[0].y,points[0].y};
    for(int i=0;i<4;i++){
        for(int j=0;j<3;j++){
            int tval = i <= 1 ? points[j].x : points[j].y;
            bool isUpdate = (i % 2 == 0) ? xyMinMax[i] > tval : xyMinMax[i] < tval;
            xyMinMax[i] = isUpdate ? tval : xyMinMax[i];
        }
    }

    if(ins>1.0){
        float a = 11;
    }

    for(int x=xyMinMax[0];x<xyMinMax[1];x+=1){
        for(int y=xyMinMax[2];y<xyMinMax[3];y+=1) {
            auto end = vec2i(x,y);
            auto result = getBaryCoord(points, end);
            if(result.x>=0.&&result.y>=0.&&result.z>=0.){
                float zbuf = zBuffer[y*WIDTH+x];
                float z = result*vec4f(oriPoints[0].z,oriPoints[1].z,oriPoints[2].z,0.);
                vec4f uv = uvs[0]*result.x+uvs[1]*result.y+uvs[2]*result.z;
                TGAColor c = md.getDiffuseColorByUv(uv)*ins;

                if(z>zbuf){
                    zBuffer[y*WIDTH+x] = z;
                    image.set(x,y,c);
                }
            }
        }
    }
}

void drawMdTriWithZ(TGAImage& image){
    Model md;
    md.readFromFile("Resource/diablo3_pose.obj");
    md.readDiffTextureFromFile("Resource/diablo3_pose_diffuse.tga");

    for (int i = 0; i <md.getFacesCount() ; i++) {
        vec4f points[3] = {md.getFaceVecs(i,0),md.getFaceVecs(i,1),md.getFaceVecs(i,2)};
        TGAColor colors[3] = {md.getDiffuseColor(i,0),md.getDiffuseColor(i,1),md.getDiffuseColor(i,2)};
        vec4f uvs[3] = {md.getUvVecs(i,0),md.getUvVecs(i,1),md.getUvVecs(i,2)};

        vec4f normal = ((points[0]-points[1])^(points[0]-points[2]));
        normal = normal.getNor();
        float instensty = vec4f(0,0,1.0,0)*normal;
//        instensty =1.0;
        if(instensty>0.1){
            drawTriangle(image, points, uvs,md,instensty);
        }
    }
}

//Gouraund shader
void drawTriangle(TGAImage &image,vec4f* oriPoints,vec4f* uvs,Model &md,vec4f* normals){
    vec2i points[3] = {SCREENVEC2(oriPoints[0]),SCREENVEC2(oriPoints[1]),SCREENVEC2(oriPoints[2])};
    int xyMinMax[4] = {points[0].x,points[0].x,points[0].y,points[0].y};
    for(int i=0;i<4;i++){
        for(int j=0;j<3;j++){
            int tval = i <= 1 ? points[j].x : points[j].y;
            bool isUpdate = (i % 2 == 0) ? xyMinMax[i] > tval : xyMinMax[i] < tval;
            xyMinMax[i] = isUpdate ? tval : xyMinMax[i];
        }
    }

    float ins = 1.;

    for(int i=0;i<4;i++){
        if(xyMinMax[i]<0||xyMinMax[i]>=WIDTH){
            return;
        }
    }

    for(int x=xyMinMax[0];x<xyMinMax[1];x+=1){
        for(int y=xyMinMax[2];y<xyMinMax[3];y+=1) {
            auto end = vec2i(x,y);
            auto result = getBaryCoord(points, end);
            if(result.x>=0.&&result.y>=0.&&result.z>=0.){
                float zbuf = zBuffer[y*WIDTH+x];
                float z = result*vec4f(oriPoints[0].z,oriPoints[1].z,oriPoints[2].z,0.);
                vec4f uv = uvs[0]*result.x+uvs[1]*result.y+uvs[2]*result.z;

//                vec4f normal = normals[0]*result.x+normals[1]*result.y+normals[2]*result.z;
                vec4f normal =  md.getNorFromMapByUv(uv);
                ins = normal*LIGHTDIR*LIGHT_INST;

                if(ins<0.2){
                    ins = 0.2;
                }

                TGAColor c = md.getDiffuseColorByUv(uv)*ins;

                if(z>zbuf&&ins>0.){
                    zBuffer[y*WIDTH+x] = z;
                    image.set(x,y,c);
                }
            }
        }
    }
}

//Gouraund shader
void drawMdTriWithG(TGAImage& image){
    Model md;
    md.readFromFile("Resource/diablo3_pose.obj");
    md.readDiffTextureFromFile("Resource/diablo3_pose_diffuse.tga");
    md.readNormalFromFile("Resource/diablo3_pose_nm.tga");

    Matrix mv = Matrix::lookAt(CENTER,EYE,UP);
//    Matrix mv = Matrix::identity(4);
    Matrix proj = Matrix::identity(4);
    proj[3][2] = -0.5;
    Matrix transform = proj*mv;
    for (int i = 0; i <md.getFacesCount() ; i++) {
        vec4f points[3] = {md.getFaceVecs(i,0),md.getFaceVecs(i,1),md.getFaceVecs(i,2)};
        for(int j=0;j<3;j++){
            points[j] = (transform*Matrix(points[j])).getVec4f();
        }
        TGAColor colors[3] = {md.getDiffuseColor(i,0),md.getDiffuseColor(i,1),md.getDiffuseColor(i,2)};
        vec4f uvs[3] = {md.getUvVecs(i,0),md.getUvVecs(i,1),md.getUvVecs(i,2)};

//        normal from .obj
//        vec4f normals[3] = {md.getFaceNors(i,0),md.getFaceNors(i,1),md.getFaceNors(i,2)};

//        normal form normal texture
        vec4f normals[3] = {md.getFaceNors(i,0),md.getFaceNors(i,1),md.getFaceNors(i,2)};

        drawTriangle(image, points, uvs,md,normals);
    }
}

void drawZbufferImg(TGAImage& image){
    float min = std::numeric_limits<float>::max();
    float max = -std::numeric_limits<float>::max();

    for(int i=0;i<WIDTH*HEIGHT;i++){
        float factor = zBuffer[i];
        if(factor==-std::numeric_limits<float>::max()){
            continue;
        }
        min = min>factor?factor:min;
        max = max<factor?factor:max;
    }

    for(int x=0;x<WIDTH;x++){
        for(int y=0;y<HEIGHT;y++){
            float factor = zBuffer[x+y*WIDTH];
            if(factor>-std::numeric_limits<float>::max()){
//                factor*=255.;
                factor = (factor-min)/(max-min)*255.;
                image.set(x,y,TGAColor(factor,factor,factor,255));
            }
        }
    }
}

int main(int argc, char **argv) {

    TGAImage image(WIDTH, HEIGHT, TGAImage::RGBA);
    initZBuffer();
//    drawMdTriWithZ(image);

    drawMdTriWithG(image);
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

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output/output3.tga");
    std::system("open output/output3.tga");
    return 0;
}
