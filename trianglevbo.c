#include <stdio.h>
#include <stdlib.h>
#include <X11/Xutil.h>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#define GL_ARRAY_BUFFER 0x8892
#define GL_STATIC_DRAW 0x88E4

char WINDOW_NAME[] = "Graphics Window";
char ICON_NAME[] = "Icon";
Display *display;
int screen;
Window main_window;
unsigned long foreground, background;
GLfloat Angle = 10.0;
static int snglBuf[] = {GLX_RGBA, GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1, GLX_BLUE_SIZE, 1, GLX_DEPTH_SIZE, 12, None};
static int dblBuf[] = {GLX_RGBA, GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1, GLX_BLUE_SIZE, 1, GLX_DEPTH_SIZE, 12,GLX_DOUBLEBUFFER, None};
// int /*GLsizeptr*/ vertex_size = 3*2*sizeof(GLfloat); /* 3 vertices with 2 coordinates*/
//static const GLsizeptr color_size = 3*3*sizeof(GLfloat); /* 3 colors with 3 coordinates each*/
/*glBufferData(GL_ARRAY_BUFFER, vertex_size+color_size, 0, GL_STATIC_DRAW);*/
/*Data will not be changed*/
/*Now put the data into the buffer*/
GLuint *buffers;
GLuint bufObjects[2];
const GLfloat Vertices[] = {-0.9,-0.9,0.9,-0.9,0.0,0.9};
const GLfloat Colors[] = {1.0,0.0,0.0,0.0, 1.0,0.0,0.0,0.0,1.0};
//static const int BufferSize=2;
//static GLuint BufferName[BufferSize];
static const GLsizei VertexCount = 3;
//enum {POSITION_OBJECT=0,COLOR_OBJECT=1};

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
  if (vi->class != TrueColor){
            fprintf(stderr, "Need a TrueColor visual\n");
            exit(1);
          }
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

 void draw()
{
}

 void redraw()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glShadeModel(GL_SMOOTH);
  fprintf(stderr, "Executing Buffer Objects\n");
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glDrawArrays(GL_TRIANGLES, 0, VertexCount);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
  if (doubleBuffer) glXSwapBuffers(display,main_window); else glFlush();
}


void doButtonPressEvent(XButtonEvent pEvent)
{
   disconnectX();
}

int main (int argc, char** argv){
  XEvent event;
  GLboolean needRedraw = GL_FALSE, recalcModelView = GL_TRUE;
  connectX();
  main_window = openWindow(10,20,500,400,5, argc, argv);
  glXMakeCurrent(display, main_window, cx);
  XMapWindow(display, main_window);
 /*glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glFrustum(-1.0,1.0, -1.0, 1.0,1.0,10.0);*/

 /*glBindBuffer(GL_ARRAY_BUFFER, BufferName[COLOR_OBJECT]);
 glBufferData(GL_ARRAY_BUFFER, ColorSize, Colors, GL_STREAM_DRAW);
 glColorPointer(3, GL_FLOAT, 0,0);

 glBindBuffer(GL_ARRAY_BUFFER, BufferName[POSITION_OBJECT]);
 glBufferData(GL_ARRAY_BUFFER,vertex_size, Vertices,GL_STREAM_DRAW);
 glVertexPointer(2, GL_FLOAT,0,0);*/

static GLuint bufferID[2];
//static const GLsizei VertexCount = 3;
glGenBuffers(2, &bufferID);
glBindBuffer(GL_ARRAY_BUFFER, bufferID[0]);
glBufferData(GL_ARRAY_BUFFER, 2*3*sizeof(GLfloat), Vertices, GL_STATIC_DRAW);
glVertexPointer(2,GL_FLOAT,0,(GLvoid*)((char*)NULL));

glBindBuffer(GL_ARRAY_BUFFER, bufferID[1]);
glBufferData(GL_ARRAY_BUFFER, 3*3*sizeof(GLfloat), Colors, GL_STATIC_DRAW);
glColorPointer(3,GL_FLOAT,0,(GLvoid*)((char*)NULL));


glClear(GL_COLOR_BUFFER_BIT);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
glOrtho(-1,-1,0,1,0,1);
glMatrixMode(GL_MODELVIEW);
while(1){
    do {
    XNextEvent(display, &event);
    switch (event.type){
      case ButtonPress:
        recalcModelView = GL_TRUE;
        Angle += 10; break;
      case KeyPress:
        doKeyPressEvent(&event);
        break;
      case ConfigureNotify:
        /* fprintf(stderr, "In ConfigureNotify\n");*/
      case Expose:
        /*fprintf(stderr, "In Expose\n");*/
        redraw();
        break;
  } /*switch*/
 } while (XPending(display));
   if (recalcModelView){
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(Angle, 0.0,0.0,1.0);
    recalcModelView =  GL_FALSE;
    needRedraw = GL_TRUE;
    } /*if*/
    if (needRedraw){
      redraw();
      needRedraw = GL_FALSE;
    }
}
}
