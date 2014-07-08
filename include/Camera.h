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

public:
    //constructor
    Camera();
    //get mouse
    Vec2 getWMouse();
    Vec2 getSMouse();
    //projection
    const Mat4& getProjection();
    const Vec2& getViewport();
    const AABox2& getBoxViewport();
    void updateProjection();
    void updateProjection(const Vec2& argViewport);
    void updateViewport(const Vec2& argViewport);
};

};

#endif