#ifndef CAMERA_H
#define CAMERA_H

#include <Config.h>
#include <Object.h>


namespace Easy2D
{

class Camera : public Object
{

    AABox2 vpBox;
    Vec2 viewport;
    Mat4 projection;
    float zoom;

public:
    //constructor
    Camera();
    //get mouse
    Vec2 getWMouse(const Vec2& mouse);
    Vec2 getSMouse(const Vec2& mouse);
    Vec2 getWMouse();
    Vec2 getSMouse();
    //projection
    void setZoom(float zoom);
    float getZoom();
    const Mat4& getProjection();
    const Vec2& getViewport();
    const AABox2& getBoxViewport();
    void updateProjection();
    void updateProjection(const Vec2& argViewport);
    void updateViewport(const Vec2& argViewport);
};

};

#endif