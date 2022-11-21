//  main.cpp
//  OpGL
//  Created by Cyril Steger on 16.11.2022.

//Libraries
#include <iostream>
#include <math.h>
using namespace std;
#define GL_SILENCE_DEPRECATION //to silence these warings
#include <GLUT/glut.h>
//Classes
#include "vector.cpp"
#include "color.cpp"
// window width and height (choose an appropriate size)
int g_iWidth  = 1000;
int g_iHeight = 1000;

// global variable to tune the timer interval
int g_iTimerMSecs;
float g_iPos;        // ... position and ...
float g_iPosIncr;    // ... position increment (used in display1)
//rotation matrix
GLfloat rotX = 0.0f; //rotate x
GLfloat rotY = 0.0f; //rotate y
GLfloat rotZ = 0.0f; //rotate z

CVec2i g_vecPos;        // same as above but in vector form ...
CVec2i g_vecPosIncr;    // (used in display2)

//Set variables and vector position on startup
void init ()
{
    // init timer interval
    g_iTimerMSecs = 10;

    // init variables for display1
    g_iPos     = 0;
    g_iPosIncr = 2;

    // init variables for display2
    int aiPos    [2] = {0, 0};
    int aiPosIncr[2] = {2, 2};
    g_vecPos.setData (aiPos);
    g_vecPosIncr.setData (aiPosIncr);
}
// function to initialize the view to ortho-projection
void initGL ()
{
    glViewport (0, 0, g_iWidth, g_iHeight);    // Establish viewing area to cover entire window.

    glMatrixMode (GL_PROJECTION);            // Start modifying the projection matrix.
    glLoadIdentity ();                        // Reset project matrix.
    glOrtho (-g_iWidth/2, g_iWidth/2, -g_iHeight/2, g_iHeight/2, 0, 1);    // Map abstract coords directly to window coords.

    // tell GL that we draw to the back buffer and
    // swap buffers when image is ready to avoid flickering
    glDrawBuffer (GL_BACK);

    // tell which color to use to clear image
    glClearColor (0,0,0,1);
}
void set8Pixel(GLint x, GLint y, GLint cx, GLint cy, GLint part){
    switch (part) {
        case 0:
            glBegin(GL_POINTS);
            //glColor3f (1,0,0);
            //glVertex2i (g_iPos -x + cx, -y + cy);
            //glVertex2i (-g_iPos -y + cx, g_iPos -x + cy);
            
            glVertex2i(-x + cx, -y + cy); // -1, -2
            glVertex2i(-y + cx, -x + cy); // -2, -1
            glEnd();
        break;
        case 1:
            glBegin(GL_POINTS);
            glVertex2i(x + cx, -y + cy); //  1, -2
            glVertex2i(y + cx, -x + cy); //  2, -1
            glEnd();
        break;
        case 2:
            glBegin(GL_POINTS);
            glVertex2i(x + cx, y + cy); //  1,  2
            glVertex2i(y + cx, x + cy); //  2,  1
            glEnd();
        break;
        case 3:
            glBegin(GL_POINTS);
            glVertex2i(-x + cx, y + cy); // -1,  2
            glVertex2i(-y + cx, x + cy); // -2,  1
            glEnd();
        break;
        default:
            break;
    }
}
//Draw a circle with radius,color and coordinates on the x-axis and y-axis and which part of the circle to draw
void drawPlanet(GLint radius,Color rgb,GLint cx,GLint cy, GLint part){
    GLint x, y, d, dE, dSE;
    //Choose a color for points
    glColor3f(rgb.r, rgb.g, rgb.b);
    x = 0;
    y = radius;
    d = 1 - radius;
    dE = 3;
    dSE = 5 - 2 * radius;
    //x = 0, y = radius, cx = x-axis cy = position y-axis, part = quarter of the circle 0,1,2,3
    //Draw pixels for each part first
    set8Pixel(x, y, cx, cy, part);
    //continue
    //while painting top eighth part of circle
        while (y > x)
        {
            //move on x-axis
            if (d < 0)
            {
                x++;
                dE += 2;
                dSE += 2;
                d += dE;
            }
            //move on y-axis
            else
            {
                x++;
                y--;
                dE += 2;
                dSE += 4;
                d += dSE;
            }
            set8Pixel(x, y, cx, cy, part);
        }
}
void rotatePlanet(float radius,Color rgb,float posX,float posY){
    glPushMatrix();
    
    for (int i = 0; i<=3; i++)
        drawPlanet(radius,rgb, posX, posY,i);
    glPopMatrix();
}
// display callback function
//Use bresenham
void display1 (void)
{
    //Variables for the planets
    float radius = 100; //increases the size
    float posX = 0; float posY = 0; //position on the x-axis and y-axis where the planet is drawn
    //Angles for the planets
    float earthAngle = 0;
    float moonAngle = 0;
    //Colors for the planets
    //Yellow color, pass values in correct order!
    glClearColor(0, 0, 0, 1);
    glClear (GL_COLOR_BUFFER_BIT);
    Color sun(1,1,0);
    Color earth(0,0,1);
    Color moon(0.5,0.5,0.5);
    //Draw sun at center position
    for (int i = 0; i<=3; i++)
        drawPlanet(radius,sun, posX, posY,i);

    //Draw earth at position +300 on x-axis
    posX = 300;
    radius = 75;
    for (int i = 0; i<=3; i++)
        drawPlanet(radius,earth, posX, posY,i);
    //Draw moon at position +450 on x-axis
    posX = 450;
    radius = 50;
    rotatePlanet(radius,moon,posX,posY);
    // In double buffer mode the last
    
    // two lines should always be
    glFlush ();
    glutSwapBuffers (); // swap front and back buffer
}

// display callback function
// use glMatrix();
void display2 (void)
{
    glClear (GL_COLOR_BUFFER_BIT);
    // display your data here ...
    
    // In double buffer mode the last
    // two lines should alsways be
    glFlush ();
    glutSwapBuffers (); // swap front and back buffer
}

int min (int a, int b) { return a>b? a: b; }
// timer callback function
void timer (int value)
{
    //Rotate your objects here
    //Angles for the planets
    float earthAngle = 0;
    float moonAngle = 0;
    //rotatePlanet(earthAngle);
    int size2 = min (g_iWidth, g_iHeight) / 2;

    // variables for display1 ...
    if (g_iPos<=-size2 || g_iPos>=size2) g_iPosIncr = -g_iPosIncr;
    g_iPos += g_iPosIncr;

    // variables for display2 ...
    if (g_vecPos(1)<=-size2 || g_vecPos(1)>=size2) g_vecPosIncr = -g_vecPosIncr;
    g_vecPos += g_vecPosIncr;

    // the last two lines should always be
    glutPostRedisplay ();
    glutTimerFunc (g_iTimerMSecs, timer, 0);    // call timer for next iteration
}
void keyboard (unsigned char key, int x, int y)
{
    switch (key) {
        case 'q':
        case 'Q':
            exit (0); // quit program
            break;
        case '1':
            glutDisplayFunc (display1);
            //glutPostRedisplay ();    // not needed since timer triggers redisplay
            break;
        case '2':
            glutDisplayFunc (display2);
            //glutPostRedisplay ();    // not needed since timer triggers redisplay
            break;
        default:
            cout << "Not a valid command" <<endl;
            break;
    };
}

int main (int argc, char **argv){
    glutInit (&argc, argv);
    // we have to use double buffer to avoid flickering
    // TODO: lookup "double buffer", what is it for, how is it used ...
    glutInitWindowSize (g_iWidth, g_iHeight);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutCreateWindow ("OpenGL- Exercise 3");
    init ();    // init my variables first
    initGL ();    // init the GL (i.e. view settings, ...)

    // assign callbacks
    //causes to refresh the timer()
    glutTimerFunc (10, timer, 0);
    glutKeyboardFunc (keyboard);
    glutDisplayFunc (display1);
    // you might want to add a resize function analog to
    // ‹bung1 using code similar to the initGL function ...

    // start main loop
    glutMainLoop ();

    return 0;
}

