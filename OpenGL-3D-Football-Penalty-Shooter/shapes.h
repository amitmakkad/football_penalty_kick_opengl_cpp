// This file contains all classes of objects used

#ifndef FOOTBALL_PENALTY_SHOOTER_SHAPES_H
#define FOOTBALL_PENALTY_SHOOTER_SHAPES_H
#include "functionalities.h"

enum objectType
{
    WALL,
    R_POLE,
    L_POLE,
    U_POLE
};

class RealObject
{
public:
    GLfloat color[4];
    objectType Type;
};

class FlatSurface : public RealObject
{
public:
    axes corners[4];
    void draw();
};

class PoleSurface : public RealObject
{
public:
    PoleSurface();
    axes base;
    double radius;
    double height;
    PhysicalState state;
    void draw();
};

class FlatArrow
{
public:
    double width;
    axes start;
    axes finish;
    double yAngle;
    double zAngle;
    GLfloat color[4];
    double length;
    void drawWithPoints();
    void drawWithAngles();
};

class Defender
{
public:
    double width;
    double height;
    double armRot;
    GLfloat color[4];

    PhysicalState state;
    void acceleration();
    void draw();
};

extern FlatArrow aimArrow;

extern FlatSurface ground;

extern PoleSurface poles[3];
extern RealObject allObjects[];
extern Defender defender;

#endif // FOOTBALL_PENALTY_SHOOTER_SHAPES_H
