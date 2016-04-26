/*#include "cube.h"
#include "defines.h"*/
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glx.h>

 GLubyte front[] = {0,1,2,3};
 GLubyte right[] = {4,5,6,7};
 GLubyte back[] = {
        8,9,10,11};
 GLubyte left[] = {
        12,13,14,15};
 GLubyte bottom[] = {
        16,17,18,19};
 GLubyte top[] = {
        20,21,22,23};

extern Display *display;
extern Window main_window;

    float side = 1.0f;
   // float side2 = 1.0 / 2.0f;
#define side2 1.0f   
 float v[24*3] = {
        // Front
       -side2, -side2, side2,
        side2, -side2, side2,
        side2,  side2, side2,
       -side2,  side2, side2,
       // Right
        side2, -side2, side2,
        side2, -side2, -side2,
        side2,  side2, -side2,
        side2,  side2, side2,
       // Back
       -side2, -side2, -side2,
       -side2,  side2, -side2,
        side2,  side2, -side2,
        side2, -side2, -side2,
       // Left
       -side2, -side2, side2,
       -side2,  side2, side2,
       -side2,  side2, -side2,
       -side2, -side2, -side2,
       // Bottom
       -side2, -side2, side2,
       -side2, -side2, -side2,
        side2, -side2, -side2,
        side2, -side2, side2,
       // Top
       -side2,  side2, side2,
        side2,  side2, side2,
        side2,  side2, -side2,
       -side2,  side2, -side2
    };

  float c[24*3] = {
        1.0, 0.0,0.0,    //front
        1.0, 0.0,0.0,
        1.0, 0.0,0.0,
        1.0, 0.0, 0.0,
        1.0, 1.0, 0.0,    //right 
        1.0, 1.0, 0.0,
        1.0, 1.0, 0.0,
        1.0, 1.0, 0.0,
        0.0, 1.0, 0.0,     //back
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0,
        1.0, 0.0, 1.0,     //left
        1.0, 0.0, 1.0,
        1.0, 0.0, 1.0,
        1.0, 0.0, 1.0,
        0.0, 1.0, 1.0,     //bottom
        0.0, 1.0, 1.0,
        0.0, 1.0, 1.0,
        0.0, 1.0, 1.0,
        0.0, 0.0, 1.0,     //top
        0.0, 0.0, 1.0,
        0.0, 0.0, 1.0,
        0.0, 0.0, 1.0
      };



void drawCube() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);
    fprintf(stderr, "In drawCube()\n");
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0,  v);
    glColorPointer(3, GL_FLOAT, 0, c);
    glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, front);
    glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, right);
    glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, back);
    glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, left);
    glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, bottom);
    glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, top);
//    glDrawArrays(GL_QUADS, 0, 24);
    glXSwapBuffers(display, main_window);

}
