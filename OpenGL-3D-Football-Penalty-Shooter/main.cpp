#include <GL/glut.h>
#include <bits/stdc++.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "constants.h"
#include "functionalities.h"
#include "shapes.h"

using namespace std;

bool poleCollided[3];
bool stopEverything = false;
unsigned int Tries, Goals;
int prevGoals;
vector<float> currentTextColor = {1, 1, 1, 1};

void showScore();

void updatePos(PhysicalState &p, double t)
{
    p.timePassed += t;

    { // Collision with Pole0
        if (p.positionCurrent.z < POLE_HEIGHT && p.positionCurrent.z > 0.0)
        {
            axes t = {-POLE_LENGTH / 2 + poles[0].state.positionCurrent[0], GOAL_POST_Y, p.positionCurrent.z};
            if ((distanceBW(t, p.positionCurrent) <= BALL_RADIUS + POLE_RADIUS) && !poleCollided[0])
            {
                poleCollided[0] = true;
                double alpha, beta, theta;
                beta = atan(p.velocityCurrent.y / p.velocityCurrent.x);
                axes vec;
                for (int i = 0; i < 3; ++i)
                {
                    vec[i] = t[i] - p.positionCurrent[i];
                }
                alpha = atan(vec.y / vec.x);
                theta = PI / 2.0 - beta + 2 * alpha;
                double v = p.velocityCurrent.x * p.velocityCurrent.x + p.velocityCurrent.y * p.velocityCurrent.y;
                v = sqrt(v);
                p.velocityCurrent.y = -v * cos(theta) * p.elasticity;
                p.velocityCurrent.x = v * sin(theta) * p.elasticity;
            }
            else if ((distanceBW(t, p.positionCurrent) <= BALL_RADIUS + POLE_RADIUS) && poleCollided[0])
            {
                poleCollided[0] = false;
            }
        }
    }
    { // Collision with Pole2
        if (p.positionCurrent.z < POLE_HEIGHT && p.positionCurrent.z > 0.0)
        {
            axes t = {POLE_LENGTH / 2 + poles[2].state.positionCurrent[0], GOAL_POST_Y, p.positionCurrent.z};
            if ((distanceBW(t, p.positionCurrent) <= BALL_RADIUS + POLE_RADIUS) && !poleCollided[2])
            {
                poleCollided[2] = true;
                double alpha, beta, theta;
                beta = atan(p.velocityCurrent.y / p.velocityCurrent.x);
                axes vec;
                for (int i = 0; i < 3; ++i)
                {
                    vec[i] = t[i] - p.positionCurrent[i];
                }
                alpha = atan(vec.y / vec.x);
                theta = PI / 2.0 - beta + 2 * alpha;
                double v = p.velocityCurrent.x * p.velocityCurrent.x + p.velocityCurrent.y * p.velocityCurrent.y;
                v = sqrt(v);
                p.velocityCurrent.y = v * cos(theta) * p.elasticity;
                p.velocityCurrent.x = -v * sin(theta) * p.elasticity;
            }
            else if ((distanceBW(t, p.positionCurrent) <= BALL_RADIUS + POLE_RADIUS) && poleCollided[2])
            {
                poleCollided[2] = false;
            }
        }
    }
    { // Collision with Pole1
        if (p.positionCurrent.x < poles[1].state.positionCurrent[0] + POLE_LENGTH / 2 + POLE_RADIUS &&
            p.positionCurrent.x > poles[1].state.positionCurrent[0] - POLE_LENGTH / 2 - POLE_RADIUS)
        {
            axes t = {p.positionCurrent.x + poles[1].state.positionCurrent[0], GOAL_POST_Y, POLE_RADIUS + POLE_HEIGHT};
            if ((distanceBW(t, p.positionCurrent) <= BALL_RADIUS + POLE_RADIUS) && !poleCollided[1])
            {
                poleCollided[1] = true;
                double alpha, beta, theta;
                beta = atan(p.velocityCurrent.y / p.velocityCurrent.z);
                axes vec;
                for (int i = 0; i < 3; ++i)
                {
                    vec[i] = t[i] - p.positionCurrent[i];
                }
                alpha = atan(vec.y / vec.z);
                theta = PI / 2.0 - beta + 2 * alpha;
                double v = p.velocityCurrent.z * p.velocityCurrent.z + p.velocityCurrent.y * p.velocityCurrent.y;
                v = sqrt(v);
                p.velocityCurrent.y = v * cos(theta) * p.elasticity;
                p.velocityCurrent.z = -v * sin(theta) * p.elasticity;
            }
            else if ((distanceBW(t, p.positionCurrent) <= BALL_RADIUS + POLE_RADIUS) && poleCollided[1])
            {
                poleCollided[1] = false;
            }
        }
    }

    { // Collision with Defender
        if (p.positionCurrent.x < defender.state.positionCurrent.x + defender.width / 2.0 &&
            p.positionCurrent.x > defender.state.positionCurrent.x - defender.width / 2.0 &&
            p.positionCurrent.z < defender.height &&
            p.positionCurrent.y + BALL_RADIUS / 2.0 + DEFENDER_THICKNESS / 2.0 >= GOAL_POST_Y && !determineSphere)
        {
            p.velocityCurrent.y *= -p.elasticity;
        }
    }

    { // Gravity and ground bouncing effects
        for (int i = 0; i < 3; ++i)
        {
            p.positionCurrent[i] =
                p.velocityCurrent[i] * t + 0.5 * p.accelerationCurrent[i] * t * t + p.positionCurrent[i];
            p.velocityCurrent[i] = p.velocityCurrent[i] + p.accelerationCurrent[i] * t;
        }
        if (p.positionCurrent[2] <= 0)
        {
            p.positionCurrent[2] = 0;
            p.velocityCurrent[2] = -p.velocityCurrent[2];
            for (int i = 0; i < 3; ++i)
            {
                p.velocityCurrent[i] = p.elasticity * p.velocityCurrent[i];
            }
        }
        for (int i = 0; i < 3; ++i)
        {
            if (fabs(p.velocityCurrent[i]) <= THRESHOLD_ZERO)
            {
                p.velocityCurrent[i] = 0;
            }
        }
        if (fabs(p.positionCurrent[2]) <= THRESHOLD_ZERO)
        {
            p.positionCurrent[2] = 0;
        }
    }
    if (p.positionCurrent.y + BALL_RADIUS > 20.0 || p.positionCurrent.y - BALL_RADIUS < -20.0)
    {
        p.velocityCurrent.y = -p.velocityCurrent.y;
    }
    if (p.positionCurrent.x + BALL_RADIUS > 20.0 || p.positionCurrent.x - BALL_RADIUS < -20.0)
    {
        p.velocityCurrent.x = -p.velocityCurrent.x;
    }
    // the moving goal posts
    {
    }
}

void renderBitmapString(
    float x,
    float y,
    float z,
    void *font,
    char *string)
{

    char *c;
    glRasterPos3f(x, y, z);
    for (c = string; *c != '\0'; c++)
    {
        glutBitmapCharacter(font, *c);
    }
}

void updatePosCallBack(int _)
{
    if (currentMode != SHOOTING && currentlyWaiting)
    {
        currentMode = SHOOTING;
        currentlyWaiting = false;
    }
    float fps = 60;
    if (currentMode == SHOOTING)
    {
        updatePos(sphere, 1.0 / fps);
        glutTimerFunc(100 / fps, updatePosCallBack, 0);
    }
}

void draw()
{
    glLoadIdentity(); // Reset the drawing perspective
    cameraPosition(toLookAt, sphereCamera.distance, sphereCamera.zAngle, sphereCamera.xAngle);
    if (firstTime)
    {
        glutWarpPointer(WIDTH / 2, HEIGHT);
        firstTime = false;
    }
    GLfloat lightColor0[] = {1.0f, 1.0f, 1.0f, 0.7f};    // Color (0.5, 0.5, 0.5)
    GLfloat lightPos0[] = {0.0f, -100.0f, 100.0f, 1.0f}; // Positioned at (4, 0, 8)
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    GLfloat lightColor1[] = {0.3f, 0.3f, 0.1f, 1.0f};  // Color (0.5, 0.5, 0.5)
    GLfloat lightPos1[] = {-1.0f, -1.0f, -1.0f, 1.0f}; // Positioned at (4, 0, 8)
    glLightfv(GL_LIGHT1, GL_AMBIENT, lightColor1);

    GLfloat lightColor2[] = {0.2f, 0.2f, 0.2f, 1.0f};  // Color (0.5, 0.5, 0.5)
    GLfloat lightPos2[] = {0.0f, 100.0f, 0.10f, 1.0f}; // Positioned at (4, 0, 8)
    glLightfv(GL_LIGHT2, GL_DIFFUSE, lightColor2);
    glLightfv(GL_LIGHT2, GL_POSITION, lightPos2);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW); // Switch to the drawing perspective

    //    glColor3f(1, 0, 0);
    //    glBegin(GL_LINES);
    //    glVertex3f(0, 0, 0);
    //    glVertex3f(5, 0, 0);
    //    glColor3f(0, 1, 0);
    //    glVertex3f(0, 0, 0);
    //    glVertex3f(0, 5, 0);
    //    glColor3f(0, 0, 1);
    //    glVertex3f(0, 0, 0);
    //    glVertex3f(0, 0, 5);
    //    glEnd();

    glPushMatrix();
    glColor3f(1.0, 1.0, 0.0);
    glTranslatef(sphere.positionCurrent.x, sphere.positionCurrent.y, sphere.positionCurrent.z);
    glutSolidSphere(BALL_RADIUS, 20, 20);
    glPopMatrix();

    //    glTranslatef(0,0,0);
    //        glScalef(2.0,2.0,1.0);

    glPushMatrix();
    glPushAttrib(GL_CURRENT_BIT);
    glColor4f(1.0, 1.0, 1.0, 0.7);
    drawGoalPost();

    glPopAttrib();
    glPopMatrix();

    if (currentMode == POWERING || currentMode == AIMING || currentMode == POWERING_ACC)
    {
        axes st = aimArrow.start;
        axes en = aimArrow.finish;
        // for (int i = 0; i < 3; ++i)
        // {
        //     cout << st[i] << " " << i << " " << en[i] << endl;
        // }

        aimArrow.drawWithAngles();
    }
    drawChalkLines();

    //    renderBitmapString(0, 0, 0.5, GLUT_BITMAP_HELVETICA_18, "HELLO");

    // Draw all transparent textured objects here:

    ground.draw();
    defender.draw();
    // if (Goals < 5)
    // {

    showScore();
   
    
    // if(Tries==3){
    //                     string msg="";
    //                    if(Goals>Tries-Goals)msg="A-WIN";
    //                    else msg="D-WIN";

    //                    resultMsg(msg);
    //                    Tries=0;
    //                    Goals=0;  
    //                     //rotateMsg(0);
    //                     cout<<"fff"<<endl;
                
    //                 }
    //             else
    //                 showMsg();
    
    resultMsg();
    

    
    

//    loadTextureFile("");
   
    drawHUD();
   

    glutSwapBuffers();
    glutPostRedisplay();
}

void showScore()
{
    glPushMatrix();
    glTranslatef(0, GOAL_POST_Y, POLE_HEIGHT + BALL_RADIUS);

    glDisable(GL_LIGHTING);

    glPushMatrix();
    glTranslatef(-POLE_LENGTH / 2.0 - BALL_RADIUS / 2.0, 0.0001, 0);
    glColor4f(125 / 255.0, 178 / 255.0, 209 / 255.0, 0.5);
    glScalef(POLE_LENGTH + BALL_RADIUS, 1, 1);
    glBegin(GL_QUADS);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1);
    glVertex3f(1, 0, 1);
    glVertex3f(1, 0, 0);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-POLE_LENGTH / 2.0 - BALL_RADIUS / 2.0, 0.0001, 1);
    //    glColor4f(133/255.0, 193/255.0, 162/255.0,0.8);
    glColor4f(178 / 255.0, 255 / 255.0, 215 / 255.0, 0.5);
    glScalef(POLE_LENGTH + BALL_RADIUS, 1, 1);
    glBegin(GL_QUADS);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1);
    glVertex3f(1, 0, 1);
    glVertex3f(1, 0, 0);
    glEnd();
    glPopMatrix();

    glEnable(GL_LIGHTING);

    glPushMatrix();
    currentTextColor = {24 / 255.0, 163 / 255.0, 24 / 255.0, 1.0};
    glTranslatef(-POLE_LENGTH / 2, 0, 1);
    //    glScalef(FONT_SIZE,FONT_SIZE,1.0);
    writeText("Attacker ", font, LEFT);
    glPopMatrix();

    glPushMatrix();
    currentTextColor = {32 / 255.0, 140 / 255.0, 32 / 255.0, 1.0};
    glTranslatef(POLE_LENGTH / 2, 0, 1);
//    glScalef(FONT_SIZE,FONT_SIZE,FONT_SIZE);
int y=Goals-prevGoals;
if(y<0) y=0;
y=y%5;
    writeText(to_string(y), font, RIGHT);
    glPopMatrix();

    glPushMatrix();
    currentTextColor = {27 / 255.0, 92 / 255.0, 145 / 255.0, 1.0};
    glTranslatef(-POLE_LENGTH / 2, 0, 0);
    //    glScalef(FONT_SIZE,FONT_SIZE,1.0);
    writeText("Defender", font, LEFT);
    glPopMatrix();

    glPushMatrix();
    currentTextColor = {0.1, 0.1, 1.0, 1.0};
    glTranslatef(POLE_LENGTH / 2, 0, 0);
//    glScalef(FONT_SIZE,FONT_SIZE,FONT_SIZE);
    int x=Tries%5-y;
    if(Tries%5==0&&Tries!=0){
        x=5-y;
    }
    if(x<0) x=0;
    x=x%5;
    writeText(to_string(x), font, RIGHT);
    glPopMatrix();

    glPopMatrix();
}

void incrementPowerMeter(int _)
{
    static int up = 1;
    if (powerMeter > 1.0 || powerMeter < 0.0)
    {
        up *= -1;
    }
    if (!currentlyWaiting && currentMode == POWERING)
    {
        powerMeter += up * 0.015;
        glutTimerFunc(1000 * 1 / 60.0, incrementPowerMeter, 0);
    }
}
void incrementPowerMeter2(int _)
{
    static int up = 1;
    if (powerMeter2 > 1.0 || powerMeter2 < 0.0)
    {
        up *= -1;
    }
    if (!currentlyWaiting && currentMode == POWERING_ACC)
    {
        powerMeter2 += up * 0.015;
        glutTimerFunc(1000 * 1 / 60.0, incrementPowerMeter2, 0);
    }
}
bool powering_set = false;
// void handleKeypress(unsigned char key, // The key that was pressed
//                     int x, int y)
// { // The current mouse coordinates
    
//     if (currentMode != HELP)
//     {
//         switch (key)
//         {
//         case '+':
//             sphereCamera.distance -= 0.1f;
//             sphereCamera.distance += (sphereCamera.distance < MIN_SPHERE_CAMERA_DISTANCE ? 0.1f : 0);
//             //            cout<<sphereCamera.distance<<endl;
//         }
//     if(currentMode==CHOOSE){
//         if(key=='1'){
//             currentLevel=HUMAN;
//             cout<<"human"<<endl;
//         }
//         else if(key=='2'){
//             currentLevel=EASY;


//           defender.state.velocityInitial[0] = defender.state.velocityCurrent[0] = DEFENDER_SPEED_EASY;
//         defender.state.velocityInitial[2]=defender.state.velocityCurrent[2]=DEFENDER_SPEED_VERTICAL;
//         defender.state.velocityInitial.x = defender.state.velocityCurrent.x = DEFENDER_SPEED_EASY;
//         defender.state.velocityInitial.z=defender.state.velocityCurrent.z=DEFENDER_SPEED_VERTICAL;
//         // defender.state.accelerationCurrent[2] = -9.8;
//             cout<<"easy"<<endl;
//         }
//         else if(key=='3'){
//             currentLevel=MEDIUM;
//             defender.state.velocityInitial[0] = defender.state.velocityCurrent[0] = DEFENDER_SPEED_MEDIUM;
//         defender.state.velocityInitial[2]=defender.state.velocityCurrent[2]=DEFENDER_SPEED_VERTICAL;
//         defender.state.velocityInitial.x = defender.state.velocityCurrent.x = DEFENDER_SPEED_MEDIUM;
//         defender.state.velocityInitial.z=defender.state.velocityCurrent.z=DEFENDER_SPEED_VERTICAL;
//         // defender.state.accelerationCurrent[2] = -19.8;
//         }
//         else if(key=='4'){
//             currentLevel=HARD;
//             defender.state.velocityInitial[0] = defender.state.velocityCurrent[0] = DEFENDER_SPEED_HARD;
//         defender.state.velocityInitial[2]=defender.state.velocityCurrent[2]=DEFENDER_SPEED_VERTICAL;
//         defender.state.velocityInitial.x = defender.state.velocityCurrent.x = DEFENDER_SPEED_HARD;
//         defender.state.velocityInitial.z=defender.state.velocityCurrent.z=DEFENDER_SPEED_VERTICAL;
//         // defender.state.accelerationCurrent[2] = -29.8;
//         }
//         currentMode=ADJUSTING;
        
//     }
//     }
    

//     if (currentMode != HELP) {
//         switch (key) {
//             case '+':
//                 sphereCamera.distance -= 0.1f;
//                 sphereCamera.distance += (sphereCamera.distance < MIN_SPHERE_CAMERA_DISTANCE ? 0.1f : 0);
// //            cout<<sphereCamera.distance<<endl;

//                 break;
//             case '-':
//                 sphereCamera.distance += 0.1f;
//                 sphereCamera.distance -= (sphereCamera.distance > MAX_SPHERE_CAMERA_DISTANCE ? 0.1f : 0);
// //            cout<<sphereCamera.distance<<endl;
//                 break;
//         }
//     } 
//     else {
//         if (key == 27){
//             currentMode = CHOOSE;
//         } 
        
//     }

//     downKeys[key] = true;
//     if (currentMode == ADJUSTING)
//     {
//         switch (key)
//         {
//         case '\r':
//             currentMode = AIMING;
//             break;
//         case EXIT_KEY: // Escape key
//             exit(0);   // Exit the program
//         }
//     }
//     if (currentMode == AIMING)
//     {
//         switch (key)
//         {
//         case 112:
//             currentMode = POWERING_ACC;
//             glutTimerFunc(1000 * 1 / 60.0, incrementPowerMeter2, 0);
//             break;
//         case 27: // Escape key
//             currentMode = ADJUSTING;
//         }
//     }
//     if(currentLevel==HUMAN){
//         if(key==97){
//             //left
        
//             defender.state.velocityInitial.x = -DEFENDER_SPEED;
//             defender.state.velocityCurrent.x = -DEFENDER_SPEED;
//             defender.state.accelerationCurrent[0] = 10;
//         }
//         if(key==100){
//             //right
//             defender.state.velocityInitial.x = DEFENDER_SPEED;
//             defender.state.velocityCurrent.x = DEFENDER_SPEED;
//             defender.state.accelerationCurrent[0] = -10;
//         }
//         if(key==119){
//             //up
//             if(defender.state.positionCurrent[2]==0){
//                 defender.state.velocityInitial[2]=DEFENDER_SPEED_VERTICAL;
//                 defender.state.velocityCurrent[2]=DEFENDER_SPEED_VERTICAL;
//                 defender.state.accelerationCurrent[2] = -9.8;
//             }
        
//         }
//     }
    

    

// }


void handleKeypress(unsigned char key, // The key that was pressed
                    int x, int y)
{ // The current mouse coordinates

    if(currentMode==CHOOSE){
        if(key=='1'){
            currentLevel=HUMAN;
            cout<<"human"<<endl;
        }
        else if(key=='2'){
            currentLevel=EASY;


          defender.state.velocityInitial[0] = defender.state.velocityCurrent[0] = DEFENDER_SPEED_EASY;
        defender.state.velocityInitial[2]=defender.state.velocityCurrent[2]=DEFENDER_SPEED_VERTICAL;
        defender.state.velocityInitial.x = defender.state.velocityCurrent.x = DEFENDER_SPEED_EASY;
        defender.state.velocityInitial.z=defender.state.velocityCurrent.z=DEFENDER_SPEED_VERTICAL;
        // defender.state.accelerationCurrent[2] = -9.8;
            cout<<"easy"<<endl;
        }
        else if(key=='3'){
            currentLevel=MEDIUM;
            defender.state.velocityInitial[0] = defender.state.velocityCurrent[0] = DEFENDER_SPEED_MEDIUM;
        defender.state.velocityInitial[2]=defender.state.velocityCurrent[2]=DEFENDER_SPEED_VERTICAL;
        defender.state.velocityInitial.x = defender.state.velocityCurrent.x = DEFENDER_SPEED_MEDIUM;
        defender.state.velocityInitial.z=defender.state.velocityCurrent.z=DEFENDER_SPEED_VERTICAL;
        // defender.state.accelerationCurrent[2] = -19.8;
        }
        else if(key=='4'){
            currentLevel=HARD;
            defender.state.velocityInitial[0] = defender.state.velocityCurrent[0] = DEFENDER_SPEED_HARD;
        defender.state.velocityInitial[2]=defender.state.velocityCurrent[2]=DEFENDER_SPEED_VERTICAL;
        defender.state.velocityInitial.x = defender.state.velocityCurrent.x = DEFENDER_SPEED_HARD;
        defender.state.velocityInitial.z=defender.state.velocityCurrent.z=DEFENDER_SPEED_VERTICAL;
        // defender.state.accelerationCurrent[2] = -29.8;
        }
        currentMode=ADJUSTING;
        
    }

    if (currentMode != HELP)
    {
        switch (key)
        {
        case '+':
            sphereCamera.distance -= 0.1f;
            sphereCamera.distance += (sphereCamera.distance < MIN_SPHERE_CAMERA_DISTANCE ? 0.1f : 0);
            //            cout<<sphereCamera.distance<<endl;

            break;
        case '-':
            sphereCamera.distance += 0.1f;
            sphereCamera.distance -= (sphereCamera.distance > MAX_SPHERE_CAMERA_DISTANCE ? 0.1f : 0);
            //            cout<<sphereCamera.distance<<endl;
            break;
        }
    }
    else
    {
        if (key == 27)
        {
            currentMode = CHOOSE;
        }
    }

    downKeys[key] = true;
    if (currentMode == ADJUSTING)
    {
        switch (key)
        {
        case '\r':
            currentMode = AIMING;
            break;
        case EXIT_KEY: // Escape key
            exit(0);   // Exit the program
        }
    }
    if (currentMode == AIMING)
    {
        switch (key)
        {
        case 112:
            currentMode = POWERING_ACC;
            glutTimerFunc(1000 * 1 / 60.0, incrementPowerMeter2, 0);
            break;
        case 27: // Escape key
            currentMode = ADJUSTING;
        }
    }
    if (currentMode == POWERING_ACC)
    {
        cout << "was here in power ACC key press.\n";
        switch (key)
        {
        case 112:
            currentMode = POWERING_ACC;
            // glutTimerFunc(1000 * 1 / 60.0, incrementPowerMeter, 0);
            break;
        case 27: // Escape key
            currentMode = AIMING;
            // case ' ':
            //     currentMode = POWERING;
            //     glutTimerFunc(1000 * 1 / 60.0, incrementPowerMeter, 0);
        }
    }
    if (currentMode == POWERING)
    {
        powering_set = true;
        cout << "was here in powering key press.\n";
        switch (key)
        {
        case ' ':
            currentMode = POWERING;
            // glutTimerFunc(1000 * 1 / 60.0, incrementPowerMeter, 0);
            break;
        case 27: // Escape key
            currentMode = POWERING_ACC;
        }
    }
    if (currentMode == POWERING_IDLE)
    {
        switch (key)
        {
        case ' ':
            currentMode = POWERING;
            glutTimerFunc(1000 * 1 / 60.0, incrementPowerMeter, 0);
            break;
        case 27:
            currentMode = POWERING_ACC;
            break;
        }
    }
    if (key == 97)
    {
        // left

        defender.state.velocityInitial.x = -DEFENDER_SPEED;
        defender.state.velocityCurrent.x = -DEFENDER_SPEED;

        // defender.state.accelerationCurrent[0] = 10;
    }
    if (key == 100)
    {
        // right
        defender.state.velocityInitial.x = DEFENDER_SPEED;
        defender.state.velocityCurrent.x = DEFENDER_SPEED;
        // defender.state.accelerationCurrent[0] = -10;
    }
    if (key == 119)
    {
        // up
        if (defender.state.positionCurrent[2] == 0)
        {
            //              defender.state.velocityInitial.y = DEFENDER_SPEED_VERTICAL;
            // defender.state.velocityCurrent.y = DEFENDER_SPEED_VERTICAL;

            defender.state.velocityInitial[2] = DEFENDER_SPEED_VERTICAL;
            defender.state.velocityCurrent[2] = DEFENDER_SPEED_VERTICAL;
            defender.state.accelerationCurrent[2] = -9.8;
        }
    }
}

double sq(double x)
{
    return x * x;
}

void idle()
{
    if (!currentlyWaiting)
    {
        if (currentMode == POWERING_ACC && !downKeys[112])
        {
            cout << "was here in P lifting ACC.\n";
            currentMode = POWERING_IDLE;
            /// powering_set = true;
            // if (downKeys[' '])
            // {

            //     glutTimerFunc(1000 * 1 / 60.0, incrementPowerMeter, 0);
            // }
            double powerMeter_half = powerMeter2 - 0.5;
            cout << "value of powerMeter2: " << powerMeter2 << endl;
            // sphere.accelerationCurrent.x = -20.0 * (powerMeter_half);
            if (powerMeter2 >= 1.0)
                powerMeter2 = 1.0;
            // currentlyWaiting = true;
        }
        if (currentMode == POWERING && !downKeys[' '] && powering_set)
        {
            cout << "was here in space lifting.\n";
            sphere.velocityCurrent[0] = sphere.velocityInitial[0] =
                cos(DEG2GRAD(aimArrow.zAngle)) * sin(DEG2GRAD(aimArrow.yAngle)) * BALL_MAX_SPEED * powerMeter;
            sphere.velocityCurrent[1] = sphere.velocityInitial[1] =
                cos(DEG2GRAD(aimArrow.zAngle)) * cos(DEG2GRAD(aimArrow.yAngle)) * BALL_MAX_SPEED * powerMeter;
            sphere.velocityCurrent[2] = sphere.velocityInitial[2] =
                sin(DEG2GRAD(aimArrow.zAngle)) * BALL_MAX_SPEED * powerMeter + BALL_MIN_SPEED;
            cout << "Value of the initial velocity is: " << sphere.velocityInitial[0] << " " << sphere.velocityInitial[1] << "\n";
            // magnus effect x dir acceleration
            // sqrt(sq(sphere.velocityCurrent[0]) + sq(sphere.velocityCurrent[1]))
            double sgn1 = 1, sgn2 = 1;
            if (sphere.velocityInitial[0] <= 0)
            {
                sgn1 = -0.5;
                sgn2 = -0.5;
            }
            sphere.accelerationCurrent.x = -0.06 * sphere.velocityCurrent[0] - 1.4 * sgn1 * powerMeter2 * sphere.velocityCurrent[1];
            // magnus effect y dir acceleration
            // sqrt(sq(sphere.velocityCurrent[0]) + sq(sphere.velocityCurrent[1]))
            sphere.accelerationCurrent.y = -0.06 * sphere.velocityCurrent[1] + 1.4 * sgn2 * powerMeter2 * sphere.velocityCurrent[0];
            if (powerMeter >= 1.0)
                powerMeter = 1.0;
            glutTimerFunc(10, updatePosCallBack, 0);
            currentlyWaiting = true;
            powering_set = false;
        }
        if (currentMode == POWERING && downKeys[27])
        {
            currentMode = AIMING;
            powerMeter = 0.0;
        }
        if (currentMode == SHOOTING)
        {
            if (sphere.positionCurrent.y <= GOAL_POST_Y)
                toLookAt = sphere.positionCurrent;
        }
        if (currentMode == SHOOTING)
        {
            if (sphere.positionCurrent.y > GOAL_POST_Y || sphere.velocityCurrent.y <= 0)
            {
                if (!determineSphere && !stopEverything)
                {
                    determineSphere = new PhysicalState;
                    *determineSphere = sphere;
                    //                    cout << *determineSphere;
                    scoredGoal = isItGoal(*determineSphere);
                    if (scoredGoal)
                    {
                        Goals++;
                        // system("paplay resources/goal.wav&");
                    }

                    
                    glutTimerFunc(1000 * RESET_TIME, initialiseEverythingCallback, 0);
                    Tries++;
                    cout<<Tries<<endl;
                    if(Tries%5==0){
                       
                      // resultMsg(msg); 
                
        
    
                       
                        rotateMsg(0);
                        
                    }
                    
                }
            }
        }
        if (currentMode == POWERING && downKeys[27])
        {
            currentMode = POWERING_ACC;
            powerMeter2 = 0.0;
        }
    }

    glutPostRedisplay();
}

axes toLookAt;

void handleUpKeypress(unsigned char key, int x, int y)
{
    downKeys[key] = false;
}

void handleSpecialKeypress(int key, int x, int y)
{
    //    if (currentMode == ADJUSTING || currentMode == REPLAY) {
    //        switch (key) {
    //            case GLUT_KEY_UP:
    //                sphereCamera.zAngle += 1.0f;
    //                break;
    //            case GLUT_KEY_DOWN:
    //                sphereCamera.zAngle -= 1.0f;
    //                break;
    //            case GLUT_KEY_LEFT:
    //                sphereCamera.xAngle -= 1.0f;
    //                break;
    //            case GLUT_KEY_RIGHT:
    //                sphereCamera.xAngle += 1.0f;
    //                break;
    //        }
    //    }
    if (currentMode == AIMING)
    {
        //    glTranslatef(36.0, 0, 0);
        //    glScalef(-1.0, 1.0, 1.0);
        //    glColor4f(0.1, 0.1, 0.1, 1.0);
        //    glBegin(GL_QUADS);
        //    glVertex2f(-10.0, -0.2);
        //    glVertex2f(5.0, -0.2);
        //    glVertex2f(5.0, 0.2);
        //    glVertex2f(-10.0, 0.2);
        //    glEnd();
        //    glBegin(GL_TRIANGLES);
        //
        //    glVertex2f(5.0, -0.4);
        //    glVertex2f(8.0, 0.0);
        //    glVertex2f(5.0, 0.4);
        //    glEnd();
        //
        //    glColor3f(0.3, 0.3, 1.0);
        //    glTranslatef(18, 0, 0);
        //    glBegin(GL_LINES);
        //    glVertex2f(-10, 0);
        //    glVertex2f(10.0, 0);
        //    glEnd();
        const float increment = 2.0f;
        switch (key)
        {
        case GLUT_KEY_UP:
            aimArrow.zAngle += (aimArrow.zAngle > 50 ? 0 : increment);
            break;
        case GLUT_KEY_DOWN:
            aimArrow.zAngle -= (aimArrow.zAngle < 0.01 ? 0 : increment);
            break;
        case GLUT_KEY_LEFT:
            aimArrow.yAngle -= (aimArrow.yAngle < -60 ? 0 : increment);
            break;
        case GLUT_KEY_RIGHT:
            aimArrow.yAngle += (aimArrow.yAngle > 60 ? 0 : increment);
            break;
        }
    }
}

template <typename T>
int sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}
int last_pos_x = -1, last_pos_y = -1;
void move_mouse(int x, int y)
{
    int curr_pos_x = x;
    int curr_pos_y = y;

    if (last_pos_x == -1 && last_pos_y == -1)
    {
        last_pos_x = curr_pos_x, last_pos_y = curr_pos_y;
    }

    // float angle = (atan2(curr_pos_y, curr_pos_x) - atan2(last_pos_y, last_pos_x));
    // rotate(angle);
    if (currentMode != HELP)
    {
        sphereCamera.xAngle = -90 + (curr_pos_x - last_pos_x - WIDTH / 2) * 120 / WIDTH;
        sphereCamera.zAngle = 45 + -1 * (curr_pos_x - curr_pos_y) * 60 / HEIGHT;
    }
    glutPostRedisplay();
    last_pos_x = curr_pos_x, last_pos_y = curr_pos_y;
}
void mouseButton(int button, int state, int x, int y)
{

    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        last_pos_x = -1;
        last_pos_y = -1;
    }
}
void handlePassiveMouse(int x, int y)
{
    //    if (currentMode == ADJUSTING) {
    if (currentMode != HELP)
    {
        sphereCamera.xAngle = -90 + (x - WIDTH / 2) * 120 / WIDTH;
        sphereCamera.zAngle = 45 + -1 * (y)*60 / HEIGHT;
    }
}

void myInit(void)
{
    glClearColor(137 / 255.0, 206 / 255.0, 255 / 255.0, 0);
    //    glOrtho(0, WIDTH, 0, HEIGHT, 0, 500);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glEnable(GL_LIGHTING); // Enable lighting

    glEnable(GL_LIGHT0);    // Enable light #0
    glEnable(GL_LIGHT1);    // Enable light #1
    glEnable(GL_LIGHT2);    // Enable light #2
    glEnable(GL_NORMALIZE); // Automatically normalize normals
    glShadeModel(GL_SMOOTH);
    backgroundMusicPlayer(0);
    updateDefenderPosition(0);
    updateGoalPostPosition(0);
    glutSetCursor(GLUT_CURSOR_NONE);
    glEnable(GL_MULTISAMPLE);
}

int main(int argc, char *argv[])
{
    // convertToTexture("resources/texture.txt", "resources/texture.texture");
    initialiseEverything();
    currentMode = HELP;
    currentLevel=NIL;


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow(WINDOW_NAME);
    glutFullScreen();
    glutReshapeFunc(handleResize);
    
    glutKeyboardFunc(handleKeypress);
    glutKeyboardUpFunc(handleUpKeypress);
    glutSpecialFunc(handleSpecialKeypress);
    glutPassiveMotionFunc(handlePassiveMouse);
    //    groundTexture = LoadBMP("resources/grass.bmp");
    groundTexture = convertAndLoadTexture("resources/grass1.txt");
    defenderTexture = convertAndLoadTexture("resources/defender1.txt");
    font = convertAndLoadTexture("resources/fonts/Ubuntu Mono Nerd Font Complete Mono.txt");
    ads = convertAndLoadTexture("resources/ads.txt");
    leftArm = convertAndLoadTexture("resources/left_arm.txt");
    rightArm = convertAndLoadTexture("resources/right_arm.txt");
    glutMouseFunc(NULL);
    // glutMotionFunc(move_mouse);
    glutDisplayFunc(draw);
    glutIdleFunc(idle);
    
    myInit();
    glutMainLoop();

    return 0;
}
