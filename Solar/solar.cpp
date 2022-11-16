//  main.cpp
//  OpGL
//  Created by Cyril Steger on 16.11.2022.

//Libraries
#include <iostream>
using namespace std;
#define GL_SILENCE_DEPRECATION //to silence these warings
#include <GLUT/glut.h>
//Classes
#include "vector.cpp"
// window width and height (choose an appropriate size)
const int g_iWidth  = 400;
const int g_iHeight = 400;

// global variable to tune the timer interval
int g_iTimerMSecs;
float g_iPos;        // ... position and ...
float g_iPosIncr;    // ... position increment (used in display1)

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

int main (int argc, char **argv){
    glutInit (&argc, argv);
    // we have to use double buffer to avoid flickering
    // TODO: lookup "double buffer", what is it for, how is it used ...
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutCreateWindow ("OpenGL- Exercise 3");

    init ();    // init my variables first
    //initGL ();    // init the GL (i.e. view settings, ...)

    // assign callbacks
    //glutTimerFunc (10, timer, 0);
    //glutKeyboardFunc (keyboard);
    //glutDisplayFunc (display1);
    // you might want to add a resize function analog to
    // ‹bung1 using code similar to the initGL function ...

    // start main loop
    glutMainLoop ();

    return 0;
}
