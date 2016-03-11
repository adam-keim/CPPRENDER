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
    float xscale = 1;
    float yscale = 1;

    enum GateMethod {
        kFill = 0, kOverscan
    };
    GateMethod fitFilm = kFill;
    switch (fitFilm) {
        default:
        case kFill:
            if (filmAspectRatio > deviceAspectRatio) { xscale = deviceAspectRatio / filmAspectRatio; } else { yscale = filmAspectRatio / deviceAspectRatio; }
            break;
        case kOverscan:
            if (filmAspectRatio > deviceAspectRatio) { yscale = filmAspectRatio / deviceAspectRatio; } else { xscale = deviceAspectRatio / filmAspectRatio; }
            break;
    }

    right *= xscale;
    top *= yscale;
    left = -right;
    bottom = -top;
    printf("bottom, left top, right %f %f %f %f", bottom, left, top, right);
    //This matrix was outputted using the blender console
    Matrix44f cameraToWorld(0.871214, 0, -0.490904, 0, -0.192902, 0.919559, -0.342346, 0, 0.451415, 0.392953, 0.801132, 0, 14.777467, 29.361945, 27.993464, 1);
    Matrix44f WtC = cameraToWorld.inverse();
    ofstream ofs;
    ofs.open("./output.svg");
    float canvasWidth = 2, canvasHeight = 2;
    ofs << "<svg version=\"1.1\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns=\"http://www.w3.org/2000/svg\" height=\"512\" width=\"512\">" << endl;
    for (uint32_t i = 0; i < numTris; ++i) {
        const Vec3f &world0 = verts[tris[i * 3]];
        const Vec3f &world1 = verts[tris[i * 3 + 1]];
        const Vec3f &world2 = verts[tris[i * 3 + 2]];
        Vec2i rc0, rc1, rc2;
        computePixelCoordinates(world0, WtC, left, right, top, bottom, nearClippingPlane, imageWidth, imageHeight, rc0);
        computePixelCoordinates(world1, WtC, left, right, top, bottom, nearClippingPlane, imageWidth, imageHeight, rc1);
        computePixelCoordinates(world2, WtC, left, right, top, bottom, nearClippingPlane, imageWidth, imageHeight, rc2);
        //cout << rc0 << rc1 << rc2;
        ofs << "<line x1=\"" << rc0.x << "\" y1=\"" << rc0.y << "\" x2=\"" << rc1.x << "\" y2=\"" << rc1.y << "\" style=\"stroke:rgba(255,0,0,.5);stroke-width:1\" />\n";
        ofs << "<line x1=\"" << rc1.x << "\" y1=\"" << rc1.y << "\" x2=\"" << rc2.x << "\" y2=\"" << rc2.y << "\" style=\"stroke:rgba(0,255,0,.5);stroke-width:1\" />\n";
        ofs << "<line x1=\"" << rc2.x << "\" y1=\"" << rc2.y << "\" x2=\"" << rc0.x << "\" y2=\"" << rc0.y << "\" style=\"stroke:rgba(0,0,255,.5);stroke-width:1\" />\n";
    }
    ofs << "</svg>";
    ofs.close();

    return 0;
}