#include <stdio.h>
#include <stdlib.h>
#include <X11/Xutil.h>
#include <X11/Xlib.h>

Display *display;
int screen;

void connectX()
{
   display = XOpenDisplay(NULL);
   if (display == NULL) {fprintf(stderr, "Cannot connect to X\n");
                         exit(1);}
   screen = DefaultScreen(display);
}

void getDisplayInfo()
{
  printf("There are %d color planes\n", DefaultDepth(display, screen));
  printf("The display is %d pixels wide\n", DisplayWidth(display, screen));
  printf("The display is %d pixels high\n", DisplayHeight(display, screen));
  printf("The color depth is %d\n", DefaultDepth(display, screen));
}

void disconnectX()
{
   XCloseDisplay(display);
   exit(0);
}

int main (int argc, char** argv){
  connectX();
  getDisplayInfo();
  disconnectX();
}
