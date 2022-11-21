//  main.cpp
//  OpGL
//  Created by Cyril Steger on 30.10.2022.

#include <iostream>
using namespace std;
#define GL_SILENCE_DEPRECATION //to silence these warings
//For MacOSX use 'GLUT'
//Run using Xcode and setup Frameworks in settings
#include <GLUT/glut.h>
//Classes
#include "point.cpp"
#include "color.cpp"

#define TEX_RES_X 60
#define TEX_RES_Y 60
// Anzahl der Pixel der Textur
#define TEX_RES TEX_RES_X*TEX_RES_Y
// Achsenl�nge der Textur (Achsen sind asymmetrisch von -TexRes#/2 bis TesRes#/2-1)
#define TEX_HALF_X TEX_RES_X/2
#define TEX_HALF_Y TEX_RES_Y/2
// Konvertiert Indices von (x,y) Koordinaten in ein lineares Array
#define TO_LINEAR(x, y) (((x)) + TEX_RES_X*((y)))
#define sign(x) ((x > 0) ? 1 : ((x < 0) ? -1 : 0))
// globaler Speicher f�r Texturdaten
char g_Buffer[3*TEX_RES];
// Textur ID Name
GLuint g_TexID = 0;

// Change the window size
int g_WinWidth = 1000;
int g_WinHeight = 1000;

void manageTexture ()
{
    glEnable        (GL_TEXTURE_2D);
    if (g_TexID==0) glGenTextures (1, &g_TexID);
    glBindTexture   (GL_TEXTURE_2D, g_TexID);
    glTexEnvf       (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D    (GL_TEXTURE_2D, 0, GL_RGB, TEX_RES_X, TEX_RES_Y, 0, GL_RGB, GL_UNSIGNED_BYTE, g_Buffer);
    glBindTexture   (GL_TEXTURE_2D, 0);
    glDisable       (GL_TEXTURE_2D);
}

void reshape(int w, int h)
{

    g_WinWidth = w;
    g_WinHeight = h;
    glViewport(0, 0, w, h);                    // Establish viewing area to cover entire window.

    glMatrixMode(GL_PROJECTION);            // Start modifying the projection matrix.
    glLoadIdentity();                        // Reset project matrix.
    glOrtho(-w/2, w/2, -h/2, h/2, 0, 1);    // Map abstract coords directly to window coords.

    glutPostRedisplay ();
}
// Ohne Farbangabe ist der Standard Wei�
void clearImage (Color c=Color()) {
    for (int i=0; i<TEX_RES; i++) {
        g_Buffer[3*i  ] = 255.0*c.r;
        g_Buffer[3*i+1] = 255.0*c.g;
        g_Buffer[3*i+2] = 255.0*c.b;
    }
}
// Funktion malt einen Punkt an die angegebenen Koordinaten
// Usage z.B.: setPoint (Point(10, 5), Color (1,0,0))
// malt einen Punkt an die Koordinate (10, 5)
// Ohne Farbangabe ist die Standard-Malfarbe Schwarz
// Nutzen Sie diese Funktion ...
void setPoint (Point p, Color c=Color(0,0,0)) {
    int x = p.x + TEX_HALF_X;
    int y = p.y + TEX_HALF_Y;
    if (x < 0 || y < 0 || x >= TEX_RES_X || y >= TEX_RES_Y) {
        cerr << "Illegal point co-ordinates (" << p.x << ", " << p.y << ")\n" << flush;
        return;
    }
    //also draw a color
    g_Buffer[3*TO_LINEAR (x, y)  ] = 255.0*c.r;
    g_Buffer[3*TO_LINEAR (x, y)+1] = 255.0*c.g;
    g_Buffer[3*TO_LINEAR (x, y)+2] = 255.0*c.b;
}
// Diese Funktion soll eine Gerade zwischen den Punkten
// p1 und p2 in der Farbe c malen. Benutzen Sie die Funktion
// setPoint um die individuellen Punkte zu zeichnen.
void bhamLine (Point p1, Point p2, Color c) {
    //New instance of Point
    Point line;
    //Draw initial points
    setPoint(Point(p1),c);
    setPoint(Point(p2),c);
    int dx, dy, x, y, d, s1, s2, temp = 0;
    bool swap = 0;
    //Calculate differences between points
    dx = abs(p2.x - p1.x);
    dy = abs(p2.y - p1.y);
    //Calculate sign value between points
    s1 = sign(p2.x - p1.x);
    s2 = sign(p2.y - p1.y);
    /* if dy has a greater range than dx swap dx and dy */
    if (dy > dx)
    {
        temp = dx;
        dx = dy;
        dy = temp;
        swap = 1;
    }
    //Set initial points (0,0)
    x = p1.x; y = p1.y;
    //Set decision parameter;
    d = 2 * dy - dx;
    //dx is the end point here
    for (int i = 0; i <= dx; i++)
    {
        line.x = x; line.y = y;
        setPoint(line,c); //drawn with default color
        while (d >= 0){
            if (swap)
                x = x + s1;
            else
            {
                y = y + s2;
                d = d - 2 * dx;
            }
        }
        if (swap)
            y = y + s2;
        else
            x = x + s1;
        
        d = d + 2 * dy;
    }
}
void CircleDraw(Point circle,int x, int y,int mr, int my)
{
    //mr, my = the Mittlepunkt coordinates
    //for each point add the mr and my coordinates
    
    //Divided the circle into 1/8 section
    //top section 1 is 1/8
    circle.x = x + mr ; circle.y = y + my;
    setPoint(circle);
    //flip top section 1 to the other side
    circle.x = -x + mr; circle.y = y + my;
    setPoint(circle);
    
    //bottom section 2 is 1/8
    circle.x = x + mr; circle.y = -y + my;
    setPoint(circle);
    //flip bottom section 2 to the other side
    circle.x = -x + mr; circle.y = -y + my;
    setPoint(circle);
    
    //right section 3
    circle.x = y + mr; circle.y = x + my;
    setPoint(circle);
    //flip right section 3 to the other side
    circle.x = -y + mr;circle.y = x + my;
    setPoint(circle);
    
    //left section 4
    circle.x = y + mr; circle.y = -x + my;
    setPoint(circle);
    //flip section 4 to the other side
    circle.x = -y + mr; circle.y = -x + my;
    setPoint(circle);
}
void bhamCircle (Point p, int r, Color c) {
    Point circle;
    // Draw Mittelpunkt
    setPoint(p,c);
    float x, y, d,dE,dSE = 0;
    
    y = r;
    d = 1 - r;
    //draw East
    dE = 3;
    //general formula
    dSE = 5 - 2 * r;
    //while radius(each step) is not bigger than x
    //while x gets incremented after each step
    //TODO: Draw pixel for each part
    
    while (y > x) {
        if(d<0){
            x++;
            //move (right) East
            dE += 2;
            //move (up) SE
            dSE += 2;
            //increment position
            d += dE;
        }
        else {
            x++;
            y--;
            dE += 2;
            dSE += 4;
            d += dSE;
        }
        circle.x = x; circle.y = y;
        //draw until each section is complete, and pass original mittelpunkt coordinates to shift the line (circle)
        CircleDraw(circle,x,y,p.x,p.y);
    }
}

void display (void)
{
    //1. Create a Background
    Color background(0.1,0.10,0.5);    // change this to any RGB color
    Color cRed(1,0,0);                    // Es soll eine rote Gerade sein ...
    Color cBlue(0,0,1);
    Color cGreen(0,1,0);
    clearImage(background);                // create a RGB colored window

    //Bresenham Line Draw
    //2. Define starting and ending point
    Point p1  (-5,  10);
    Point p2  (10, -5);
    //3. Draw a line with red color
    bhamLine  (p1, p2, cRed);
    //4. Draw second line with blue color
    Point p3 (5,-10);
    Point p4 (-10, 5);
    bhamLine(p3, p4, cBlue);
    //5. Draw a third line with green color
    Point p5 (-15,0);
    Point p6 (0,-15);
    bhamLine(p5, p6, cGreen);
    
    //clearImage(background);
    //Bresenham Circle Draw
    //1. Create a middle point
    Point p(-3, -5);
    //2. Define Radius of the circle
    int r = 17;
    //3. Draw a circle
    bhamCircle (p, r, cGreen);

    manageTexture ();

        glClear      (GL_COLOR_BUFFER_BIT);
        glBindTexture(GL_TEXTURE_2D, g_TexID);
        glEnable     (GL_TEXTURE_2D);

        glBegin     (GL_QUADS);
        glColor3f   (1, 0, 0);
        glTexCoord2f(0, 0);
        glVertex2f  (-g_WinWidth/2, -g_WinHeight/2);
        glTexCoord2f(1, 0);
        glVertex2f  (g_WinWidth/2, -g_WinHeight/2);
        glTexCoord2f(1, 1);
        glVertex2f  (g_WinWidth/2, g_WinHeight/2);
        glTexCoord2f(0, 1);
        glVertex2f  (-g_WinWidth/2, g_WinHeight/2);
        glEnd       ();

        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable    (GL_TEXTURE_2D);

        glFlush ();
}

// Die Main-Funktion
int main (int argc, char **argv) {

    glutInit (&argc, argv);
    //set window size
    glutInitWindowSize (g_WinWidth, g_WinHeight);
    glutCreateWindow ("OpenGL - Exercise1"); //name this anything you want

    glutReshapeFunc (reshape);
    glutDisplayFunc (display);
    
    glutMainLoop ();

    glDeleteTextures (1, &g_TexID);

    return 0;
}
