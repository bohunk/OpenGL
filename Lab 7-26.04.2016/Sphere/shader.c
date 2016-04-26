#include "shader.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GLhandleARB programObject = 0;
GLhandleARB VertexShaderObject = 0;
GLhandleARB FragmentShaderObject = 0;
char infoLog[1024];
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
   GLuint verShader, fragShader, programHandle;
   GLhandleARB vHandle, fHandle;
   GLint verCompiled, fragCompiled, progLinked;
   GLint linked;
   const char *vShader, *fShader; 
   fprintf(stderr, "Attempting to create shaders\n");
   verShader = glCreateShader(GL_VERTEX_SHADER);
   fragShader = glCreateShader(GL_FRAGMENT_SHADER);
   fprintf(stderr, "Created Shaders\n");
   vShader = readShader("Verts.vert", EVertexShader);
   fprintf(stderr, "Printing Vertex Shader:\n");
   fprintf(stderr, "%s\n", vShader);
   if (!vShader)
   {
     glDeleteShader(verShader);
     glDeleteShader(fragShader);
     fprintf(stderr, "Vertex Shader not found...exiting\n");
     exit(1);

   }
   fShader = readShader("Frags.frag", EFragmentShader);
   fprintf(stderr, "Printing Fragment Shader:\n");
   fprintf(stderr, "%s\n", fShader);
   if (!fShader)
   {
     glDeleteShader(verShader);
     glDeleteShader(fragShader);
     fprintf(stderr, "Fragment Shader not found...exiting\n");
     exit(1);

   }
   fprintf(stderr, "\n\nSetting sources\n");
   glShaderSource(verShader, 1, &vShader, NULL);
   fprintf(stderr, "Set the vertex shader source\n");
   glShaderSource(fragShader, 1, &fShader, NULL);
   fprintf(stderr, "Set the fragmwent shader source\n");
   fprintf(stderr, "Compiling Vertex Shader\n"); 
   glCompileShader(verShader);
   glGetShaderiv(verShader, GL_COMPILE_STATUS, &verCompiled);
   if (verCompiled == GL_FALSE){
      fprintf(stderr, "Vertex Shader not compiled\n");
      glGetShaderInfoLog(verShader,1024, NULL, infoLog);
      fprintf(stderr, "%s\n", infoLog);
      //printInfoLog(verShader);
      glDeleteShader(verShader);
      glDeleteShader(fragShader);
      return 0; 
      }
   fprintf(stderr, "Compiling Fragment Shader\n");
   glCompileShader(fragShader);
   //printOpenGLError();
   glGetShaderiv(fragShader, GL_COMPILE_STATUS, &fragCompiled);
   if (fragCompiled == GL_FALSE){
     glGetShaderInfoLog(fragShader, 1024, NULL, infoLog);
     fprintf(stderr, "%s\n", infoLog);
     // printInfoLog(fragShader);
      return 0; 
      }
  programHandle = glCreateProgram();
  if (programHandle == 0) fprintf(stderr, "No shader program\n");
  glAttachShader(programHandle, verShader);
  glAttachShader(programHandle, fragShader);
  glBindAttribLocation(programHandle, 0, "VertexPosition");
  glBindAttribLocation(programHandle, 1, "VertexNormal");
  glLinkProgram(programHandle);
  glGetProgramiv(programHandle, GL_LINK_STATUS, &progLinked);
  if (progLinked == GL_FALSE){
    char infoLink[1024];
    glGetProgramInfoLog(programHandle, 1024, NULL, infoLink);
    fprintf(stderr, "Program does not link with error %s\n", infoLink);
    glDeleteProgram(programHandle);
    return (GLuint)NULL;
  }
    glUseProgram(programHandle);
    return programHandle;
}



