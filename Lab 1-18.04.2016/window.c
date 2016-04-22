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
   XSelectInput(display, new_window, (ButtonPressMask | ButtonReleaseMask |
                                      ExposureMask));
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
   XEvent event;
   XSelectInput(display, main_window, ExposureMask);
   XNextEvent(display, &event);
   XDrawImageString(display, main_window, gc, 10, 10, "Hello, World",
                    strlen("Hello, World"));
   XDrawArc(display, main_window, gc, 30, 30, 100, 100, 0, 360*64);
   XFlush(display);
}

void doButtonPressEvent(XButtonEvent pEvent)
{
   disconnectX();
}

int main (int argc, char** argv){
  connectX();
  main_window = openWindow(10,20,500,400,5, argc, argv);
  gc = getGC();
  XMapWindow(display, main_window);
   draw();
   sleep(20);
  disconnectX();
}
