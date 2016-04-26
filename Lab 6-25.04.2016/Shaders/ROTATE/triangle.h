#ifndef _triangle_h_
#define _triangle_h_
#include <GL/glew.h>

#include <stdio.h>
#include <stdlib.h>
#include <X11/Xutil.h>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>


#define printOpenGLError() printOglError(__FILE__,__LINE__)


typedef enum{
    EVertexShader,
    EFragmentShader,
} ShaderType;


#endif
