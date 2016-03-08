#include <iostream> 
#include <cstdlib> 
#include <cstdio> 
#include <fstream> 
#include <cmath> 

#include "geometry.h" 

bool computePixelCoordinates(
        const Vec3f &pWorld,
        const Matrix44f &cameraToWorld,
        const float &canvasWidth,
        const float &canvasHeight,
        const int &imageWidth,
        const int &imageHeight,
        Vec2i &pRaster) {
    Vec3f pCamera;
    Matrix44f WtC = cameraToWorld.inverse();
    WtC.multVecMatrix(pWorld, pCamera);

    return true;
}