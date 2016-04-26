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

  fprintf(stderr, "Set up buffers\n");
  
  /* Fill in Open GL calls here */
  GLfloat points[] = {
    0.0f,0.8f,0.0f,
    0.8f, -0.8f,0.0f,
    -0.8f, -0.8f,0.0f
  };

  GLfloat colors[]={
    1.0,0.0,0.0,
    0.0,1.0,0.0,
    0.0,0.0,1.0
  };

  GLuint vaoHandle;
  GLuint vboHandles[2];
  glGenBuffers (2,vboHandles);
  GLuint vertexBufferHandle = vboHandles[0];
  GLuint colorBufferHandle = vboHandles[1];

  //Fill the Vertex Buffer

  glBindBuffer (GL_ARRAY_BUFFER, vertexBufferHandle);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points),points, GL_STATIC_DRAW);

  //Fill the Color Buffer

  glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

  //Set up the VAO
  
  glGenVertexArrays(1, &vaoHandle);
  glBindVertexArray(vaoHandle);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
  glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 0, NULL);

  glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
  glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, 0, NULL);
 
//  glBindVertexArray(vaoHandle); 


//Now draw the triangle using the shaders
 
  fprintf(stderr, "Drawing...\n");
  while(!glfwWindowShouldClose(window)){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(vaoHandle); 
    glDrawArrays(GL_TRIANGLES,0,3);
    glfwPollEvents();
    glfwSwapBuffers(window);  
   }
  glfwTerminate();
  return 0;

}
