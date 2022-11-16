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
int g_iWidth  = 1000;
int g_iHeight = 1000;

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
// display callback function
void display1 (void)
{

    glClear (GL_COLOR_BUFFER_BIT);

    ///////
    // display your data here ...
    //

    glBegin (GL_TRIANGLES);
        glColor3f (1,0,0);
        glVertex2i (g_iPos, 0);
        glColor3f (0,1,0);
        glVertex2i (-g_iPos, g_iPos);
        glColor3f (0,0,1);
        glVertex2i (-g_iPos, -g_iPos);
    glEnd ();

    //
    ///////

    // In double buffer mode the last
    // two lines should alsways be
    glFlush ();
    glutSwapBuffers (); // swap front and back buffer
}

// display callback function
void display2 (void)
{
    glClear (GL_COLOR_BUFFER_BIT);

    ///////
    // display your data here ...
    //

    glBegin (GL_QUADS);
        glColor3f (1,0,0);
        glVertex2i (-g_vecPos(1), -g_vecPos(2));
        glColor3f (0,1,0);
        glVertex2i (g_vecPos(1), -g_vecPos(2));
        glColor3f (0,0,1);
        glVertex2i (g_vecPos(1), g_vecPos(2));
        glColor3f (1,1,0);
        glVertex2i (-g_vecPos(1), g_vecPos(2));
    glEnd ();

    //
    ///////

    // In double buffer mode the last
    // two lines should alsways be
    glFlush ();
    glutSwapBuffers (); // swap front and back buffer
}

int min (int a, int b) { return a>b? a: b; }
// timer callback function
void timer (int value)
{
    ///////
    // update your variables here ...
    //

    int size2 = min (g_iWidth, g_iHeight) / 2;

    // variables for display1 ...
    if (g_iPos<=-size2 || g_iPos>=size2) g_iPosIncr = -g_iPosIncr;
    g_iPos += g_iPosIncr;

    // variables for display2 ...
    if (g_vecPos(1)<=-size2 || g_vecPos(1)>=size2) g_vecPosIncr = -g_vecPosIncr;
    g_vecPos += g_vecPosIncr;

    //
    ///////

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
    glutTimerFunc (10, timer, 0);
    glutKeyboardFunc (keyboard);
    glutDisplayFunc (display1);
    // you might want to add a resize function analog to
    // ‹bung1 using code similar to the initGL function ...

    // start main loop
    glutMainLoop ();

    return 0;
}

