#include <stdio.h>
#include <X11/Xutil.h>
#include <X11/Xlib.h>

char WINDOW_NAME[] = "Graphics Window";
char ICON_NAME[] = "Icon";
Display *display;
int screen;
Window main_window;
GC gc;
unsigned long foreground, background;

void connectX()
{
   display = XOpenDisplay(NULL);
   if (display == NULL) {fprintf(stderr, "Cannot connect to X\n");
                         exit(1);}
   screen = DefaultScreen(display);
   foreground = BlackPixel(display, screen);
   background = WhitePixel(display, screen);
}

Window openWindow(int x, int y, int width, int height,
                  int border_width, int argc, char** argv)
{
    Window new_window;
    XSizeHints size_hints;
    new_window = XCreateSimpleWindow(display, DefaultRootWindow(display),
                 x, y, width, height, border_width, foreground,
                 background);
   size_hints.x = x;
   size_hints.y = y;
   size_hints.width = width;
   size_hints.height = height;
   size_hints.flags = PPosition | PSize;
   XSetStandardProperties(display, new_window, WINDOW_NAME, ICON_NAME,
                          None, argv, argc, &size_hints);
   XSelectInput(display, new_window, (ButtonPressMask | KeyPressMask |
                                      ExposureMask | PointerMotionMask));
   return (new_window);
}

GC getGC()
{
    GC gc;
    XGCValues gcValues;
    gc = XCreateGC(display, main_window, (unsigned long)0, &gcValues);
    XSetBackground(display, gc, background);
   XSetForeground(display, gc, foreground);
    return (gc);
}

void disconnectX()
{
   XCloseDisplay(display);
   exit(0);
}


 void draw()
{
   XDrawImageString(display, main_window, gc, 10, 10, "Hello, World",
                    strlen("Hello, World"));
   XDrawArc(display, main_window, gc, 30, 30, 100, 100, 0, 360*64);
   XFlush(display);
}

void doButtonPressEvent(XButtonEvent *pEvent)
{
  int x,y;
  x = pEvent->x;
  y = pEvent->y;
  XDrawImageString(display, main_window, gc, x, y, "You hit me",
                   strlen("You hit me"));
  printf("Mouse clicked at %d,%d\n", x,y);
  /* disconnectX();*/
  printf("Button pressed\n");
}

void doMotionNotifyEvent(XMotionEvent *pEvent)
{
  int x,y;
  char hitLoc[20];
  x = pEvent->x;
  y = pEvent->y;
 /* XClearArea(display, main_window, 50, 10, 100, 30);*/
  sprintf(hitLoc, "Pixel: %d,%d", x,y);
  XDrawImageString(display, main_window,gc,10,180, hitLoc, strlen(hitLoc));
 /* printf("At %d %d\n", x,y);*/

}
void doExposeEvent(XExposeEvent *pEvent)
{
  XDrawImageString(display, main_window, gc, 10, 260,
                  "You just caused an expose event", 
                  strlen("You just caused an expose event"));
  printf("Expose Event occurred\n");
  draw();
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
   else printf("You pressed %c\n", key_buffer[0]);
}


int main (int argc, char** argv){
  XEvent event;
  connectX();
  main_window = openWindow(10,20,500,400,5, argc, argv);
  gc = getGC();
  XMapWindow(display, main_window);
  draw();
  XFlush(display);
  while (True){
    XNextEvent(display, &event);
   switch(event.type){
    case Expose:
      printf("Expose event\n");
      doExposeEvent (&event);
      break;
    case ButtonPress:
      printf("Button Pressed\n");
      doButtonPressEvent(&event);
      break;
    case KeyPress:
     printf("Key pressed\n");
     doKeyPressEvent(&event);
     break;
    case MotionNotify:
     doMotionNotifyEvent(&event);
     /*printf("motion notify\n");*/
     break;
   
   }
  }   
}
