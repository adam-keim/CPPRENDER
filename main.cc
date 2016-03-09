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
    Vec2f pScreen;
    pScreen.x = (pCamera.x/-pCamera.z);
    pScreen.y = (pCamera.y/-pCamera.z);
    if (abs(pScreen.x) >= canvasWidth/2 || abs(pScreen.y >= canvasHeight/2))
        return false;
    Vec2f pNdc;
    pNdc.x = (pScreen.x + canvasWidth/2)/canvasWidth;
    pNdc.y = (pScreen.y + canvasHeight/2)/canvasHeight;
    
    pRaster.x = floor(pNdc.x * imageWidth);
    pRaster.y = floor(pNdc.x * imageHeight);
    return true;
}
int main(){
    return 0;
}