bool computePixelCoordinates(
        const Vec3f &pWorld,
        const Matrix44f &WtC,
        const float &l,
        const float &r,
        const float &t,
        const float &b,
        const float &near,
        const int &imageWidth,
        const int &imageHeight,
        Vec2i &pRaster) {
    Vec3f pCamera;
    //Compute world to camera matrix
    WtC.multVecMatrix(pWorld, pCamera);
    //Convert to screen space
    Vec2f pScreen;
    pScreen.x = pCamera.x / -pCamera.z * near;
    pScreen.y = pCamera.y / -pCamera.z * near;
    //Check if on screen

    //Normalize
    Vec2f pNDC;
    pNDC.x = (pScreen.x + r) / (2 * r);
    pNDC.y = (pScreen.y + t) / (2 * t);
    pRaster.x = (pNDC.x * imageWidth);
    pRaster.y = ((1 - pNDC.y) * imageHeight);
    //Convert to Raster
    bool visible = true;
    if (pScreen.x < l || pScreen.x > r || pScreen.y < b || pScreen.y > t)
        visible = false;
    return visible;
}