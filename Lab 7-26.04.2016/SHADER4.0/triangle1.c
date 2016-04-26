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

  GLuint vbo = 0;
  glGenBuffers (1,&vbo);

  //Fill the Vertex Buffer

  glBindBuffer (GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points),points, GL_STATIC_DRAW);


  //Set up the VAO
  GLuint vao = 0;  
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 0, NULL);
 
//  glBindVertexArray(vaoHandle); 

//The Vertex Shader  

const char* vertex_shader=
"#version 410\n"
"in vec3 vp;"
"void main(){"
"  gl_Position = vec4 (vp,1.0);"
"}";

//The Fragment Shader

const char* fragment_shader=
"#version 410\n"
"out vec4 frag_color;"
"void main() {"
" frag_color = vec4(0.5,0.5,0.0,1.0);"
"}";

//Compile both shaders

  fprintf(stderr, "Compiling shaders...\n");
  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  if (vs == 0) fprintf(stderr, "Cannot create vertex shader\n");
  glShaderSource(vs, 1, &vertex_shader,NULL);
  glCompileShader(vs);
  GLint result;
  glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE) fprintf(stderr, "Vertex compilation failed\n");
  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
  if (fs == 0) fprintf(stderr, "Cannot create fragment shader\n");
  glShaderSource(fs,1,&fragment_shader, NULL);
  glCompileShader(fs);
  glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE) fprintf(stderr, "Vertex compilation failed\n");
  

//Link them into a single shader program
 
  fprintf(stderr, "Linking...\n");
  GLuint shader_program = glCreateProgram();

  glAttachShader (shader_program, fs);
  glAttachShader(shader_program, vs);
  glLinkProgram(shader_program);

//Now draw the triangle using the shaders
 
  fprintf(stderr, "Drawing...\n");
  while(!glfwWindowShouldClose(window)){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shader_program);
    glBindVertexArray(vao); 
    glDrawArrays(GL_TRIANGLES,0,3);
    glfwPollEvents();
    glfwSwapBuffers(window);  
   }
  glfwTerminate();
  return 0;

}
