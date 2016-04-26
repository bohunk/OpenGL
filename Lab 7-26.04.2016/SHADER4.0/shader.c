#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shader.h"
#define __gl_h_
#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
//#include <glm/glm.hpp>
#include <OpenGL/glu.h>
//#include <GL/glew.h>
//using glm::mat4;
//using glm::vec3;
//using glm::vec4;
//using glm::mat3;
//#include <glm/gtc/matrix_transform.hpp>
GLuint prog = 0;     


char * readShader(char *fileName, ShaderType shaderType)
{
    FILE *fp;
    char name[20];
    char *vcontent = NULL;
    char *fcontent = NULL;
    int count = 0;

    if (fileName == NULL) return NULL;
    else
    {
       fp = fopen(fileName, "rt");
       if (fp != NULL){
         fseek(fp, 0, SEEK_END);
         count = ftell(fp);
         rewind(fp);
         if (count > 0){
         switch (shaderType)
         {
           case EVertexShader:
             vcontent = (char *)malloc(sizeof(char)*(count+1));
             count = fread(vcontent, sizeof(char), count, fp);
             vcontent[count] = '\0';
             return vcontent;
            case EFragmentShader:
             fcontent = (char *)malloc(sizeof(char)*(count+1));
             count = fread(fcontent, sizeof(char), count, fp);
             fcontent[count] = '\0';
             return fcontent;
         }
         fclose(fp);
     }
   }
}
return NULL;
}

void printShader(char *buffer)
{
    printf("%s\n", buffer);
}

int installShaders()
{
   GLuint verShader, fragShader;
   GLhandleARB vHandle, fHandle;
   GLint verCompiled,fragCompiled;
   GLint linked;
   const char *vertexShader,  *fragmentShader;  
   fprintf(stderr, "Attempting to create shaders\n");
   verShader = glCreateShader(GL_VERTEX_SHADER);
   fragShader = glCreateShader(GL_FRAGMENT_SHADER);
   fprintf(stderr, "Created Shaders\n");
  // vertexShader = readShader("Verts.vert", EVertexShader);
   vertexShader = readShader("cvert.vert", EVertexShader);
   fprintf(stderr, "Printing Vertex Shader:\n");
   fprintf(stderr, "%s\n", vertexShader);
   if (!vertexShader)
   {
     glDeleteShader(verShader);
     glDeleteShader(fragShader);
     fprintf(stderr, "Vertex Shader not found...exiting\n");
     exit(1);
   }

   //fragmentShader = readShader("Frags.frag", EFragmentShader);
   fragmentShader = readShader("cfrag.frag", EFragmentShader);
   fprintf(stderr, "Printing Fragment Shader:\n");
   fprintf(stderr, "%s\n", fragmentShader);
   if (!fragmentShader)
   {
     glDeleteShader(verShader);
     glDeleteShader(fragShader);
     fprintf(stderr, "Fragment Shader not found...exiting\n");
     exit(1);

   }


   fprintf(stderr, "\nSetting Vertex source\n");
   glShaderSource(verShader, 1, &vertexShader, NULL);
   fprintf(stderr, "\nSetting the Fragment  source\n");
   glShaderSource(fragShader, 1, &fragmentShader, NULL);
  
   fprintf(stderr, "Compiling Vertex Shader\n");
   glCompileShader(verShader);
   glGetShaderiv(verShader, GL_COMPILE_STATUS, &verCompiled);
   if (verCompiled == GL_FALSE){
      fprintf(stderr, "Vertex Shader not compiled\n");
      glDeleteShader(verShader);
      glDeleteShader(fragShader);
      return 0; 
      }

   fprintf(stderr, "Compiling Fragment Shader\n");
   glCompileShader(fragShader);
   glGetShaderiv(fragShader, GL_COMPILE_STATUS, &fragCompiled);
   if (fragCompiled == GL_FALSE){
      fprintf(stderr, "Fragment Shader not compiled\n");
      glDeleteShader(verShader);
      glDeleteShader(fragShader);
      return 0; 
      }

    
    prog = glCreateProgram();
    glAttachShader(prog, verShader);
    glAttachShader(prog, fragShader);
    //glBindAttribLocation(prog, 0, "VertexPosition");
    //glBindAttribLocation(prog, 1, "VertexColor");
    glLinkProgram(prog);
    glDeleteShader(verShader);
    glDeleteShader(fragShader);
    glGetProgramiv(prog, GL_LINK_STATUS, &linked);
    if (linked == GL_FALSE){
       fprintf(stderr, "Problems with linking\n");
      // glGetShaderInfoLog(prog, 1024, NULL, infoLog);
      // fprintf(stderr, "%s\n", infoLog);
       glDeleteProgram(prog);
       return 0;
       }
     else fprintf(stderr, "Linking prog was successful\n");
    fprintf(stderr, "Using Vertex and Frag Shaders\n");
    //glUseProgram(prog);
    //Now output vertices to see control points
    return 1;
}

