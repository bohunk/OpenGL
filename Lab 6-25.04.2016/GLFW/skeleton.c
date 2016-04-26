#include <GL/glew.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
//#include <OpenGL/glut.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <stdio.h>
int main(int argc,char **argv)
{
  if (!glfwInit()){
    fprintf(stderr,"ERROR: cannot start initialization GLFW3\n");
    return 1;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
  GLFWwindow* window  = glfwCreateWindow(640,480, "Hello Window",
                                         NULL,NULL);
  if (!window){
    fprintf(stderr,"ERROR: could not open window with GLFW\n");
    glfwTerminate();
    return 1;
  } 
  glfwMakeContextCurrent(window);
  glewExperimental = GL_TRUE;
  glewInit();
  const GLubyte* renderer = glGetString(GL_RENDERER);
  fprintf(stderr, "Able to call OpenGL");
  const GLubyte* version = glGetString(GL_VERSION);
  printf("Renderer: %s\n", renderer);
  printf("OpenGL version supported: %s\n", version);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  /* Fill in Open GL calls here */

  glfwTerminate();
  return 0;

}
