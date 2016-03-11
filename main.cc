#include <iostream> 
#include <cstdlib> 
#include <cstdio> 
#include <fstream> 
#include <cmath> 

#include "geometry.h"
#include "pcoords.h"
#include "object.h"
using namespace std;
//Camera Parameters
static const float inchesToMm = 25.4;
float focalLength = 12;
// 35mm Full Aperture
float filmApertureWidth = 0.980;
float filmApertureHeight = 0.735;
static const float inchToMm = 25.4;
float nearClippingPlane = 0.1;
float farClipingPlane = 1000;

int main() {
    uint32_t imageWidth = 512, imageHeight = 512;
    float deviceAspectRatio = imageWidth / imageHeight;
    //Calculate clipping vertices in all 4 directions
    float top = ((filmApertureHeight * inchToMm / 2) / focalLength) * nearClippingPlane;
    float bottom = -top;
    float filmAspectRatio = filmApertureWidth / filmApertureHeight;
    float right = bottom * filmAspectRatio;
    float left = -right;
    float xScale = 1;
    float yScale = 1;

    enum GateMethod {
        kFill = 0, kOverscan
    };
    GateMethod fitFilm = kFill;
    switch (fitFilm) {
        default:
        case kFill:
            if (filmAspectRatio > deviceAspectRatio) { xScale = deviceAspectRatio / filmAspectRatio; } else { yScale = filmAspectRatio / deviceAspectRatio; }
            break;
        case kOverscan:
            if (filmAspectRatio > deviceAspectRatio) { yScale = filmAspectRatio / deviceAspectRatio; } else { xScale = deviceAspectRatio / filmAspectRatio; }
            break;
    }

    right *= xScale;
    top *= yScale;
    left = -right;
    bottom = -top;
    printf("bottom, left top, right %f %f %f %f", bottom, left, top, right);
    //This matrix was outputted using the blender console
    Matrix44f cameraToWorld(0.871214, 0, -0.490904, 0, -0.192902, 0.919559, -0.342346, 0, 0.451415, 0.392953, 0.801132, 0, 14.777467, 29.361945, 27.993464, 1);
    Matrix44f worldToCamera = cameraToWorld.inverse();
    ofstream ofs;
    ofs.open("./output.svg");
    float canvasWidth = 2, canvasHeight = 2;
    ofs << "<svg version=\"1.1\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns=\"http://www.w3.org/2000/svg\" height=\"512\" width=\"512\">" << endl;
    //Render Loop!
    for (uint32_t i = 0; i < numTris; ++i) {
        const Vec3f &world0 = verts[tris[i * 3]];
        const Vec3f &world1 = verts[tris[i * 3 + 1]];
        const Vec3f &world2 = verts[tris[i * 3 + 2]];
        Vec2i rasterCoords0, rasterCoords1, rasterCoords2;
        computePixelCoordinates(world0, worldToCamera, left, right, top, bottom, nearClippingPlane, imageWidth, imageHeight, rasterCoords0);
        computePixelCoordinates(world1, worldToCamera, left, right, top, bottom, nearClippingPlane, imageWidth, imageHeight, rasterCoords1);
        computePixelCoordinates(world2, worldToCamera, left, right, top, bottom, nearClippingPlane, imageWidth, imageHeight, rasterCoords2);
        //cout << rc0 << rc1 << rc2;
        ofs << "<line x1=\"" << rasterCoords0.x << "\" y1=\"" << rasterCoords0.y << "\" x2=\"" << rasterCoords1.x << "\" y2=\"" << rasterCoords1.y << "\" style=\"stroke:rgba(255,0,0,.5);stroke-width:1\" />\n";
        ofs << "<line x1=\"" << rasterCoords1.x << "\" y1=\"" << rasterCoords1.y << "\" x2=\"" << rasterCoords2.x << "\" y2=\"" << rasterCoords2.y << "\" style=\"stroke:rgba(0,255,0,.5);stroke-width:1\" />\n";
        ofs << "<line x1=\"" << rasterCoords2.x << "\" y1=\"" << rasterCoords2.y << "\" x2=\"" << rasterCoords0.x << "\" y2=\"" << rasterCoords0.y << "\" style=\"stroke:rgba(0,0,255,.5);stroke-width:1\" />\n";
    }
    ofs << "</svg>";
    ofs.close();

    return 0;
}