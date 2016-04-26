//#include "triangle.h"
#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xutil.h>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <GL/gl.h>

extern int installShaders();
extern GLuint prog;
/* bullshit stuff that does not work*/
//#define cmath math.h
#include <glm/glm.hpp>
using glm::mat4;
using glm::vec3;
#include <glm/gtc/matrix_transform.hpp>


GLfloat Angle = 45.0;
mat4 rotationMatrix = glm::rotate(mat4(1.0f), Angle, vec3(0.0f,0.0f,1.0f));
GLuint vaoHandle;
char WINDOW_NAME[] = "Graphics Window";
char ICON_NAME[] = "Icon";
Display *display;
int screen;
Window main_window;
unsigned long foreground, background;
static int snglBuf[] = {GLX_RGBA, GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1, GLX_BLUE_SIZE, 1, GLX_DEPTH_SIZE, 12, None};
static int dblBuf[] = {GLX_RGBA, GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1, GLX_BLUE_SIZE, 1, GLX_DEPTH_SIZE, 12,GLX_DOUBLEBUFFER, None};


//GLfloat Vertices[] = {{-0.9,-0.9}, {0.9,-0.9}, {0.0,0.9}};
//GLfloat Colors[] = {{1.0,0.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0}};



Bool doubleBuffer = True;
XVisualInfo *vi;
GLXContext cx;
Colormap cmap;
XSetWindowAttributes swa;
Bool recalcModelView = True;
int dummy;
Bool needRedraw = True;

void connectX()
{
   display = XOpenDisplay(NULL);
   if (display == NULL) {fprintf(stderr, "Cannot connect to X\n");
                         exit(1);}
   screen = DefaultScreen(display);
   if (!glXQueryExtension(display, &dummy, &dummy)){
         fprintf(stderr, "X server has no OpenGL GLX Extension\n");
         exit(1);}
   vi = glXChooseVisual(display, screen, dblBuf);
   if (vi == NULL){
      fprintf(stderr, "Double Buffering not available\n");
      vi = glXChooseVisual(display, screen, snglBuf);
      if (vi == NULL) fprintf(stderr, "No RGB Visual with depth buffer\n");
      doubleBuffer = False;
   }
  if (doubleBuffer == True) fprintf(stderr, "We have double buffering\n");
  /*if (vi->class != TrueColor){
            fprintf(stderr, "Need a TrueColor visual\n");
            exit(1);
          }*/
  cx = glXCreateContext(display, vi , None, True);
  if (cx == NULL){
       fprintf(stderr, "No rendering context\n");
       exit(1);
      }
  cmap = XCreateColormap(display, RootWindow(display, vi->screen),
                         vi->visual, AllocNone);
  /*if (cmap == NULL){
      fprintf(stderr, "Color map is not available\n");
      exit(1);
     }*/
  swa.colormap = cmap;
  swa.border_pixel = 0;
  swa.event_mask = ExposureMask | ButtonPressMask | StructureNotifyMask |
                   KeyPressMask;
}

Window openWindow(int x, int y, int width, int height,
                  int border_width, int argc, char** argv)
{
    XSizeHints size_hints;
    Window new_window;
    new_window = XCreateWindow(display, RootWindow(display, vi->screen),
                 x,y, width, height, border_width, vi->depth, InputOutput,
                 vi->visual, CWBorderPixel | CWColormap | CWEventMask,
                 &swa); 
   size_hints.x = x;
   size_hints.y = y;
   size_hints.width = width;
   size_hints.height = height;
   size_hints.flags = PPosition | PSize;
   XSetStandardProperties(display, new_window, WINDOW_NAME, ICON_NAME,
                          None, argv, argc, &size_hints);
   XSelectInput(display, new_window, (ButtonPressMask | KeyPressMask |
                                       StructureNotifyMask | ExposureMask));
   return (new_window);
}




void disconnectX()
{
   XCloseDisplay(display);
   exit(0);
}

void doKeyPressEvent(XKeyEvent *pEvent)
{
 int key_buffer_size = 10;
 char key_buffer[9];
 XComposeStatus compose_status;
 KeySym key_sym;
 XLookupString(pEvent, key_buffer, key_buffer_size,
               &key_sym, &compose_status);
 if (key_buffer[0] == 'q') disconnectX();
 else fprintf(stderr, "You pressed %c\n", key_buffer[0]);
}


void init()
{
  fprintf(stderr, "Initializing\n");
  //Step1: Define the graphic data
  float Vertices[] = {-0.9,-0.9, 0.9,-0.9, 0.0,0.9};
  float Colors[] = {1.0,0.0,0.0, 0.0,1.0,0.0, 0.0,0.0,1.0};
  //Step2: Create the buffer objects
  GLuint vboHandles[2];
  glGenBuffers(2, vboHandles);
  GLuint positionBufferHandle = vboHandles[0];
  GLuint colorBufferHandle = vboHandles[1];
  glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), Vertices, GL_STATIC_DRAW); //copy data
  glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
  glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), Colors, GL_STATIC_DRAW); //copy data
  //Step3: Create and bind to a Vertex Array Object
  glGenVertexArrays(1, &vaoHandle);
  glBindVertexArray(vaoHandle);
  glEnableVertexAttribArray(0); //enable vertices
  glEnableVertexAttribArray(1); //enable colors
  glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
  //Attribute 0 will get its data from positionBufferHandle
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
  //Analogous for Attribute 1
  glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
  glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
  glBindVertexArray(0);
  //Set up the uniforma variable location
  GLuint location = glGetUniformLocation(prog, "RotationMatrix");
  if (location >= 0 )
  {
    fprintf(stderr, "location %d\n", location);
    //glUniformMatrix4fv(location, 1, GL_FALSE, &rotationMatrix[0][0]);
    glUniformMatrix4fv(location, 1, GL_FALSE, &rotationMatrix[0][0]);
  }
}

 void draw()
{
}

 void redraw()
{
/* draw a Gouraud triangle
  fprintf(stderr, "Calling redraw()\n");*/
  glClear(GL_COLOR_BUFFER_BIT);
  glShadeModel(GL_SMOOTH);
  glBindVertexArray(vaoHandle);
  glDrawArrays(GL_TRIANGLES, 0,3);
  if (doubleBuffer) glXSwapBuffers(display,main_window); else glFlush();
}

void doButtonPressEvent(XButtonEvent pEvent)
{
   disconnectX();
}

int main (int argc, char** argv){
  XEvent event;
  GLboolean needRedraw = GL_FALSE, recalcModelView = GL_TRUE;
  const unsigned char *version;
  GLint *params;
  int success;
  connectX();
  main_window = openWindow(10,20,500,400,5, argc, argv);
  glXMakeCurrent(display, main_window, cx);
  XMapWindow(display, main_window);
  version = glGetString(GL_VERSION);
  if (version == NULL) {fprintf(stderr, "Version not supported\n");
                        exit(1);}
  fprintf(stderr, "The Version is: %s\n", version);
  glewExperimental = GL_TRUE;
  fprintf(stderr, "Setting up GLEW\n");
  glewInit();
  if (glewGetExtension("GL_ARB_fragment_shader")      != GL_TRUE ||
        glewGetExtension("GL_ARB_vertex_shader")        != GL_TRUE ||
        glewGetExtension("GL_ARB_shader_objects")       != GL_TRUE ||
        glewGetExtension("GL_ARB_shading_language_100") != GL_TRUE)
    {
        fprintf(stderr, "Driver does not support OpenGL Shading Language\n");
        exit(1);
    }

  success = installShaders();
  if (success) fprintf(stderr, "We have shaders!\n");
  else fprintf(stderr, "Yes, we have no shaders today\n");
fprintf(stderr, "Supporting Version %s of GLSL\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  init();
/* glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glFrustum(-1.0,1.0, -1.0, 1.0,1.0,10.0);
glClear(GL_COLOR_BUFFER_BIT);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
glOrtho(-1,-1,0,1,0,1);
glMatrixMode(GL_MODELVIEW);*/
while(1){
    do {
    XNextEvent(display, &event);
    switch (event.type){
      case ButtonPress:
        recalcModelView = GL_TRUE;
        Angle += 10; break;  //passed to Vertex Shader
      case KeyPress:
        doKeyPressEvent((XKeyEvent *)&event);
        break;
      case ConfigureNotify:
         glViewport(0,0, event.xconfigure.width,  event.xconfigure.height);
         fprintf(stderr, "In ConfigureNotify\n");
      case Expose:
        /*fprintf(stderr, "In Expose\n");*/
        redraw();
        break;
  } /*switch*/
 } while (XPending(display));
   if (recalcModelView){
  /*  glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(Angle, 0.0,0.0,1.0);*/
    recalcModelView =  GL_FALSE;
    needRedraw = GL_TRUE;
    } /*if*/
    if (needRedraw){
      redraw();
      needRedraw = GL_FALSE;
    }
}
}
