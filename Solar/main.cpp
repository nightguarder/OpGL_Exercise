//  main.cpp
//  OpGL
//  Created by Cyril Steger on 16.11.2022.

//Libraries
#include <iostream>
#include <math.h>
#include <complex>
using namespace std;
#define GL_SILENCE_DEPRECATION //to silence these warings
#include <GLUT/glut.h>
//Classes
#include "vector.cpp"
#include "color.cpp"
#include "point.cpp"
#include "matrix.cpp"
//Project SETUP
// window width and height (choose an appropriate size)
int g_iWidth  = 1000;
int g_iHeight = 1000;

// global variable to tune the timer interval
int g_iTimerMSecs;
float g_iPos;        // ... position and ...
float g_iPosIncr;    // ... position increment (used in display1)

//Planet object initialization
Point Sun,Earth,Moon = 0;
Point Sun_2,Earth_2,Moon_2 = 0;
//Planet Color initialization
Color sunColor(1,1,0);
Color earthColor(0,0,1);
Color moonColor(0.5,0.5,0.5);


CVec2i g_vecPos;        // same as above but in vector form ...
CVec2i g_vecPosIncr;    // (used in display2)

//Set variables and vector position on startup
void init ()
{
    // init timer interval
    g_iTimerMSecs = 10;

    g_iPos     = 0;
    g_iPosIncr = 2;
    //Draw sun at center
    Sun.x = 0;
    Sun.y = 0;
    //Draw earth at position +300 on x-axis
    Earth.x = 300;
    Earth.y = 0;
    //Draw moon at position +450 on x-axis
    Moon.x = 450;
    Moon.y = 0;
    glDrawBuffer (GL_BACK);

    // tell which color to use to clear image
    glClearColor (0,0,0,1);
}
// function to initialize the view to ortho-projection
void initGL ()
{
    glViewport (0, 0, g_iWidth, g_iHeight);    // Establish viewing area to cover entire window.
    //repeat for display 2
    Sun_2.x = 0;
    Sun_2.y = 0;
    //Draw earth at position +300 on x-axis
    Earth_2.x = 300;
    Earth_2.y = 0;
    //Draw moon at position +450 on x-axis
    Moon_2.x = 450;
    Moon_2.y = 0;
    
    glMatrixMode (GL_PROJECTION);            // Start modifying the projection matrix.
    glLoadIdentity ();                        // Reset project matrix.
    glOrtho (-g_iWidth/2, g_iWidth/2, -g_iHeight/2, g_iHeight/2, 0, 1);    // Map abstract coords directly to window coords.
    // tell GL that we draw to the back buffer and
    // swap buffers when image is ready to avoid flickering
    glDrawBuffer (GL_BACK);

    // tell which color to use to clear image
    glClearColor (0,0,0,1);
}
void setAllPixel(GLint x, GLint y, GLint cx, GLint cy){
    glBegin(GL_POINTS);
    
    glVertex2i(x + cx, y + cy);   //  1,  2
    glVertex2i(y + cx, x + cy);   //  2,  1
    glVertex2i(-x + cx, y + cy);  // -1,  2
    glVertex2i(-y + cx, x + cy);  // -2,  1
    glVertex2i(x + cx, -y + cy);  //  1, -2
    glVertex2i(y + cx, -x + cy);  //  2, -1
    glVertex2i(-x + cx, -y + cy); // -1, -2
    glVertex2i(-y + cx, -x + cy); // -2, -1
    
    glEnd();
}
//Draw a circle with radius,color and coordinates on the x-axis and y-axis
void drawPlanet(GLint radius,Color rgb,GLint cx,GLint cy){
    GLint x, y, d, dE, dSE;
    //Choose a color for points
    glColor3f(rgb.r, rgb.g, rgb.b);
    x = 0;
    y = radius;
    d = 1 - radius;
    dE = 3;
    dSE = 5 - 2 * radius;
    //x = 0, y = radius, cx = x-axis cy = position y-axis, part = quarter of the circle 0,1,2,3
    //Draw starting pixel on 1/4 part
    //There are total of 4 parts of the circle
    setAllPixel(x, y, cx, cy);
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
            setAllPixel(x, y, cx, cy);
        }
    
}
//Rotate Planet cx first around 2 Planet px
//Rotate Planet 1(p1)Earth around planet Planet 2  (p2)Sun
Point rotateAroundPlanet(Point p1, Point p2, float angle)
{
    //Formula is not right
    Point newPos(newPos.x = ((p1.x - p2.x) * cos(angle)) - ((p2.y - p1.y) * sin(angle)),
        newPos.y = p2.y - ((p2.y - p1.y) * cos(angle)) - ((p1.x - p2.x) * sin(angle)));
    
    cout<< newPos.x<<endl;
    cout << newPos.y<<endl;
    
    return newPos;
}
// Rotation around point, with Matrix coordinates
Point rotateAroundPlanet2(Point cx, Point px, float angle)
{
    Point newPos(cx.x);
    return newPos;
}
//Return new position maybe?
void rotateAround(Point cx, Point px, float angle){
    glPushMatrix();//Initialize new Matrix Dont't delete.
    Color rand(0.5,0.1,0.8);
    //3 arguments x , y ,z
    glRotatef(angle, 0, 0, 1); // First rotate then translate!
    glTranslatef(0, 5, 0);
    //glScalef(0.6, 0.6, 1);
    drawPlanet(55, rand, cx.x,cx.y);
    glPopMatrix();//free memory.Don't delete.
}
// display callback function
//Use bresenham
void display1 (void)
{
    //Variables for the planets
    GLfloat radius = 0; //increase the size of planet
    //Colors for the planets
    //Yellow color, pass values in correct order!
    glClearColor(0, 0, 0, 1);
    glClear (GL_COLOR_BUFFER_BIT);
    
    //Draw Planets with decreasing radius
    drawPlanet(radius = 100,sunColor, Sun.x, Sun.y);
    drawPlanet(radius = 75, earthColor, Earth.x, Earth.y);
    drawPlanet(radius = 50, moonColor, Moon.x, Moon.y);
    
    // In double buffer mode the last
    // two lines should always be
    glFlush ();
    glutSwapBuffers (); // swap front and back buffer
}

// display callback function
// use glMatrix();
void display2 (void)
{
    GLfloat radius = 0; //increase the size of planet
    glClear (GL_COLOR_BUFFER_BIT);
    // display your data here ...
    //Draw Planets with decreasing radius
    drawPlanet(radius = 100, sunColor, Sun.x, Sun.y);
    drawPlanet(radius = 75, earthColor, Earth_2.x, Earth_2.y);
    drawPlanet(radius = 50, moonColor, Moon_2.x, Moon_2.y);
    glFlush ();
    glutSwapBuffers (); // swap front and back buffer
}

int min (int a, int b) { return a>b? a: b; }
// timer callback function
void timer (int value)
{
    //Rotate your objects here
    //Angles for the planets
    float earthAngle = -0.0117; // Earth Rotation Speed factor
    float moonAngle = -0.0314;
    
    int size2 = min (g_iWidth, g_iHeight) / 2;
    
    //display 1
    // rotation formula
    Earth = rotateAroundPlanet(Earth, Sun, earthAngle);
    //cout << Earth.x << endl;
    //cout << Earth.y << endl;
    
    //Moon  = rotateAroundPlanet(Moon, Earth, moonAngle);
    
    //display 2
    //Earth_2 = rotateAroundPlanet2(Earth_2, Sun_2, earthAngle);
    //Moon_2 = rotateAroundPlanet2(Moon_2, Earth_2, moonAngle);
    //Refresh the screen
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
            exit(0);
            break;
    };
}

int main (int argc, char **argv){
    glutInit (&argc, argv);
    // we have to use double buffer to avoid flickering
    // TODO: lookup "double buffer", what is it for, how is it used ...
    glutInitWindowSize (g_iWidth, g_iHeight);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutCreateWindow ("OpenGL - Exercise 3");
    
    init ();    // init my variables first
    initGL ();    // init the GL (i.e. view settings, ...)
    
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

