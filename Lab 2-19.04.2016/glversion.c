#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include <stdio.h>
#include <stdlib.h>
#include <X11/Xutil.h>
#include <X11/Xlib.h>
#include <GL/glx.h>


char WINDOW_NAME[] = "Graphics Window";
char ICON_NAME[] = "Icon";
Display *display;
int screen;
Window main_window;
unsigned long foreground, background;

static int snglBuf[] = {GLX_RGBA, GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1, GLX_BLUE_SIZE, 1, GLX_DEPTH_SIZE, 12, None};
static int dblBuf[] = {GLX_RGBA, GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1, GLX_BLUE_SIZE, 1, GLX_DEPTH_SIZE, 12,GLX_DOUBLEBUFFER, None};

Bool doubleBuffer = True;
XVisualInfo *vi;
GLXContext cx;
Colormap cmap;
XSetWindowAttributes swa;
Bool recalcModelView = True;
int dummy;

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
   else printf(stderr, "We have a rendering context\n");

  cmap = XCreateColormap(display, RootWindow(display, vi->screen),
                         vi->visual, AllocNone);
  if (cmap == NULL){
      fprintf(stderr, "Color map is not available\n");
      exit(1);
     }
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


void init(void)
{
  const GLubyte *renderer = glGetString(GL_RENDERER);
  const GLubyte *vendor = glGetString(GL_VENDOR);
  const GLubyte *version = glGetString(GL_VERSION);
  const GLubyte *glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION);
  GLint major, minor;

  //glGetIntegerv(GL_MAJOR_VERSION, &major);
  //glGetIntegerv(GL_MINOR_VERSION, &minor);
  printf("GL_VENDOR           : %s\n", vendor);
  printf("GL_RENDERER         : %s\n", renderer);
  printf("GL_VERSION (string) : %s\n", version);
  //printf("GL_VERSION (int)    : %d.%d\n", major, minor);
  printf("GLSL Version        : %s\n", glslversion);

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
 switch(key_buffer[0]){
    case 'q':
      exit(0);
      break;
   }
}


void redraw(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  glShadeModel(GL_SMOOTH);
  glBegin(GL_POLYGON);
    glColor3f(1.0,0.0,0.0);
    glVertex2f(-0.9,-0.9);
    glColor3f(0.0,1.0,0.0);
    glVertex2f(0.9,-0.9);
    glColor3f(0.0,0.0,1.0);
    glVertex2f(0.0,0.9);
  glEnd();

  if (doubleBuffer) glXSwapBuffers(display,main_window); else
   glFlush();
//   fprintf(stderr, "redraw called\n");
}


void doButtonPressEvent(XButtonEvent *pEvent)
{
   fprintf(stderr, "Mouse button pressed\n");
}

int main (int argc, char** argv){
  XEvent event;
  connectX();
  main_window = openWindow(10,20,500,400,5, argc, argv);
  glXMakeCurrent(display, main_window, cx);
  XMapWindow(display, main_window);

  glClear(GL_COLOR_BUFFER_BIT);
  init();
  
 /*glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glFrustum(-1.0,1.0, -1.0, 1.0,0.0,10.0);*/
while(1){
    do{
    XNextEvent(display, &event);
    switch (event.type){
      case KeyPress:
        doKeyPressEvent(&event);
        break;
      case ConfigureNotify:
        glViewport(0,0, event.xconfigure.width, event.xconfigure.height);
      case Expose:
          redraw();
          break;
       case ButtonPress:
          doButtonPressEvent(&event);
          break; 
 }
}
   while (True); //XPending(display));
  }
}
