// This contains code for drawing different objects

#include <GL/glut.h>
#include "shapes.h"
#include <bits/stdc++.h>
#include "functionalities.h"

void FlatSurface::draw()     // drawing flat surface like ground
{

    start2DTexture(groundTexture);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(corners[0].x, corners[0].y, corners[0].z);
    glTexCoord2f(0.0, GRASS_TEXT_MULTIPLY);
    glVertex3f(corners[1].x, corners[1].y, corners[1].z);
    glTexCoord2f(GRASS_TEXT_MULTIPLY, GRASS_TEXT_MULTIPLY);
    glVertex3f(corners[2].x, corners[2].y, corners[2].z);
    glTexCoord2f(GRASS_TEXT_MULTIPLY, 0.0);
    glVertex3f(corners[3].x, corners[3].y, corners[3].z);
    glEnd();

    end2DTexture();

    start2DTexture(ads);
    for (int i = 0; i < 4; ++i)
    {

        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 1);
        glVertex3f(corners[i].x, corners[i].y, corners[i].z);
        glTexCoord2f(1 * 10, 1);
        glVertex3f(corners[(i + 1) % 4].x, corners[(i + 1) % 4].y, corners[(i + 1) % 4].z);
        glTexCoord2f(1 * 10, 0);
        glVertex3f(corners[(i + 1) % 4].x, corners[(i + 1) % 4].y, corners[(i + 1) % 4].z + AD_HEIGHT);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(corners[i].x, corners[i].y, corners[(i + 1) % 4].z + AD_HEIGHT);
        glEnd();
    }

    end2DTexture();
}

FlatSurface ground;

void PoleSurface::draw()        // draw pole rods
{
    glPushAttrib(GL_CURRENT_BIT);
    GLUquadric *quadric = gluNewQuadric();

    if (Type == U_POLE)
    {
        glTranslated(radius, 0.0, radius);
        glRotatef(180, 1, 0, 0);
        {
            glPushMatrix();
            glPushAttrib(GL_CURRENT_BIT);
            glColor4fv(color);
            glTranslated(-radius, 0, radius);
            glRotatef(-90, 0, 1, 0);
            gluCylinder(quadric, radius, radius, height - radius, 20, 10);
            glPopMatrix();

            glTranslated(-height - 2 * radius, 0, 0);
            glPushMatrix();
            glRotatef(90, 1, 0, 0);
            myShear();
            glRotatef(-90, 1, 0, 0);
            glRotatef(90, 0, 1, 0);
            glTranslated(-radius, 0.0, 0.0);
            glColor4fv(color);
            gluCylinder(quadric, radius, radius, 2 * radius, 20, 10);
            glPopMatrix();

            glTranslated(height, 0, 0);
            glScalef(-1.0, 1.0, 1.0);
            glPushMatrix();
            glPushAttrib(GL_CURRENT_BIT);
            glColor4fv(color);
            glTranslated(-radius, 0, radius);
            glRotatef(-90, 0, 1, 0);
            gluCylinder(quadric, radius, radius, height - radius, 20, 10);
            glPopMatrix();

            glTranslated(-height - 2 * radius, 0, 0);
            glPushMatrix();
            glRotatef(90, 1, 0, 0);
            myShear();
            glRotatef(-90, 1, 0, 0);
            glRotatef(90, 0, 1, 0);
            glTranslated(-radius, 0.0, 0.0);
            glColor4fv(color);
            gluCylinder(quadric, radius, radius, 2 * radius, 20, 10);
            glPopMatrix();
        }
    }
    else if (Type == R_POLE || Type == L_POLE)
    {
        if (Type == L_POLE)
        {
            glScalef(-1.0, 1.0, 1.0);
        }
        glRotatef(90, 0, 1, 0);
        glTranslated(0, 0, 2 * radius);
        glRotatef(180, 1, 0, 0);
        glEnd();
        {

            glPushMatrix();
            glPushAttrib(GL_CURRENT_BIT);
            glColor4fv(color);
            glTranslated(0, 0, radius);
            glRotatef(-90, 0, 1, 0);
            gluCylinder(quadric, radius, radius, height, 20, 10);
            glPopMatrix();

            glTranslated(-height - 2 * radius, 0, 0);
            glPushMatrix();
            glRotatef(90, 1, 0, 0);
            myShear();
            glRotatef(-90, 1, 0, 0);
            glRotatef(90, 0, 1, 0);
            glTranslated(-radius, 0.0, 0.0);
            glColor4fv(color);
            gluCylinder(quadric, radius, radius, 2 * radius, 20, 10);
            glPopMatrix();
        }
    }
    gluDeleteQuadric(quadric);
    glPopMatrix();
    glPopAttrib();
}

PoleSurface::PoleSurface()
{
    radius = POLE_RADIUS;
    color[0] = color[1] = color[2] = 1.0;
    color[3] = 1.0;
}

void FlatArrow::drawWithAngles()  // drawing arrow of ball
{

    axes vector;
    for (int i = 0; i < 3; ++i)
    {
        vector[i] = finish[i] - start[i];
    }

    double rot_y = yAngle;
    glPushAttrib(GL_CURRENT_BIT);
    glPushMatrix();

    glTranslated(0, -10, 0);
    glRotatef(-rot_y, 0, 0, 1);

    glPushMatrix();

    double rot_z = zAngle;
    glRotatef(rot_z, 1, 0, 0);

    glColor4fv(color);
    glBegin(GL_QUADS);
    glVertex3f(-width / 2, start.y, 0);
    glVertex3f(width / 2, start.y, 0);
    glVertex3f(width / 2, length - 2 * width / vector.y, 0);
    glVertex3f(-width / 2, length - 2 * width / vector.y, 0);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(-width, length - 2 * width / vector.y, 0);
    glVertex3f(width, length - 2 * width / vector.y, 0);
    glVertex3f(0, length, 0);
    glEnd();
    glPopMatrix();
    glPopMatrix();
    glPopAttrib();

    glPushAttrib(GL_CURRENT_BIT);
    glPushMatrix();

    glTranslatef(0, -10, 0);
    glRotatef(-rot_y, 0, 0, 1);
    glColor4f(0.0, 0.5, 0.0, 1.0);
    glBegin(GL_QUADS);
    glVertex3f(-width / 2, start.y, -BALL_RADIUS + 0.001f);
    glVertex3f(width / 2, start.y, -BALL_RADIUS + 0.001f);
    glVertex3f(width / 2, length - 2 * width / vector.y - length * sin(DEG2GRAD(zAngle)), -BALL_RADIUS + 0.001f);
    glVertex3f(-width / 2, length - 2 * width / vector.y - length * sin(DEG2GRAD(zAngle)), -BALL_RADIUS + 0.001f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(-width, length - 2 * width / vector.y - length * sin(DEG2GRAD(zAngle)), -BALL_RADIUS + 0.001f);
    glVertex3f(width, length - 2 * width / vector.y - length * sin(DEG2GRAD(zAngle)), -BALL_RADIUS + 0.001f);
    glVertex3f(0, length - length * sin(DEG2GRAD(zAngle)), -BALL_RADIUS + 0.001f);
    glEnd();

    glPopMatrix();
    glPopAttrib();
}

void Defender::acceleration()       //collision of defender with poles
{
    if (currentLevel == MOVE_POST)
    {

        if (GOAL_POST_X + POLE_LENGTH / 2 - POLE_RADIUS + poles[2].state.positionCurrent[0] - this->state.positionCurrent.x <= 2 ||
            this->state.positionCurrent.x - (GOAL_POST_X - POLE_LENGTH / 2 + POLE_RADIUS + poles[0].state.positionCurrent[0]) <= 2)
        {
            this->state.velocityCurrent.x *= -1;
            if (this->state.positionCurrent.x - (GOAL_POST_X - POLE_LENGTH / 2 + POLE_RADIUS + poles[0].state.positionCurrent[0]) <= 2)
            {

                this->state.positionCurrent[0] += 0.2;
            }
            else
            {
                this->state.positionCurrent[0] -= 0.2;
            }
        }
    }
    else
    {
        if (this->state.positionCurrent.x >= POLE_LENGTH / 2.0 - this->width / 2 - POLE_RADIUS ||
            this->state.positionCurrent.x <= -POLE_LENGTH / 2.0 + this->width / 2 + POLE_RADIUS)
        {
            this->state.velocityCurrent.x *= -1;
        }
    }
}

void Defender::draw()           // drawing defender
{
    start2DTexture(defenderTexture);

    glTranslatef(defender.state.positionCurrent.x, GOAL_POST_Y, (this->height) / 2 - BALL_RADIUS + defender.state.positionCurrent.z);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 1);
    glVertex3f(-this->width / 2, 0, -this->height / 2);
    glTexCoord2f(1, 1);
    glVertex3f(this->width / 2, 0, -this->height / 2);
    glTexCoord2f(1, 0.0);
    glVertex3f(this->width / 2, 0, this->height / 2);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-this->width / 2, 0, this->height / 2);
    glEnd();



    glEnd();

    end2DTexture();

    start2DTexture(leftArm);

    float displacex = -0.3;
    float displacey = 0.63;
    glTranslatef(defender.state.positionCurrent.x, GOAL_POST_Y, (this->height) / 2 - BALL_RADIUS + defender.state.positionCurrent.z);
    glTranslatef(this->width * displacex / 2, 0, this->width * displacey / 2);
    glRotatef(armRot, 0, 1, 0);
    glTranslatef(-this->width * displacex / 2, 0, -this->width * displacey / 2);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 1);
    glVertex3f(-this->width / 2, 0, -this->height * 0.22 / 2);
    glTexCoord2f(1, 1);
    glVertex3f(0 - this->width * 0.15 / 2, 0, -this->height * 0.2 / 2);
    glTexCoord2f(1, 0.0);
    glVertex3f(0 - this->width * 0.15 / 2, 0, this->height * 0.60 / 2);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-this->width / 2, 0, this->height * 0.60 / 2);
    glEnd();

    end2DTexture();

    start2DTexture(leftArm);

    glTranslatef(defender.state.positionCurrent.x, GOAL_POST_Y, (this->height) / 2 - BALL_RADIUS + defender.state.positionCurrent.z);
    glScalef(-1, 1, 1);
    glTranslatef(this->width * displacex / 2, 0, this->width * displacey / 2);
    glRotatef(armRot, 0, 1, 0);
    glTranslatef(-this->width * displacex / 2, 0, -this->width * displacey / 2);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 1);
    glVertex3f(-this->width / 2, 0, -this->height * 0.22 / 2);
    glTexCoord2f(1, 1);
    glVertex3f(0 - this->width * 0.15 / 2, 0, -this->height * 0.2 / 2);
    glTexCoord2f(1, 0.0);
    glVertex3f(0 - this->width * 0.15 / 2, 0, this->height * 0.60 / 2);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-this->width / 2, 0, this->height * 0.60 / 2);
    glEnd();

    end2DTexture();

}

FlatArrow aimArrow;

PoleSurface poles[3];

Defender defender;

RealObject allObjects[] = {poles[0], poles[1], poles[2]};