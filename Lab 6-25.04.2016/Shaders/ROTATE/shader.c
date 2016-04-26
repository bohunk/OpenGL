#include "triangle.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char infoLog[1024];
GLuint prog = 0;

int printOglError(char *file, int line)
{
    //
    // Returns 1 if an OpenGL error occurred, 0 otherwise.
    //
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    /*while (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
        retCode = 1;
        glErr = glGetError();
    } */
    return 1; //retCodee;
}



static
void printInfoLog(GLint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    GLchar *infoLog;

    printOpenGLError();  // Check for OpenGL errors

    glGetObjectParameterivARB(obj, GL_INFO_LOG_LENGTH,
                                         &infologLength);

   printOpenGLError();  // Check for OpenGL errors

    if (infologLength > 0)
    {
        infoLog = (GLcharARB*)malloc(infologLength);
        if (infoLog == NULL)
        {
            printf("ERROR: Could not allocate InfoLog buffer\n");
            exit(1);
        }
        glGetInfoLogARB(obj, infologLength, &charsWritten, infoLog);
        printf("InfoLog:\n%s\n\n", infoLog);
        free(infoLog);
    }
    printOpenGLError();  // Check for OpenGL errors
}


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
   GLint verCompiled, fragCompiled;
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
  
   glCompileShader(verShader);
   printOpenGLError();
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
   glCompileShader(fragShader);
   //printOpenGLError();
   glGetShaderiv(fragShader, GL_COMPILE_STATUS, &fragCompiled);
   if (fragCompiled == GL_FALSE){
     glGetShaderInfoLog(fragShader, 1024, NULL, infoLog);
     fprintf(stderr, "%s\n", infoLog);
     // printInfoLog(fragShader);
      return 0; 
      }
    prog = glCreateProgram();
    glAttachShader(prog, verShader);
    glAttachShader(prog, fragShader);
    glBindAttribLocation(prog, 0, "VertexPosition");
    glBindAttribLocation(prog, 1, "VertexColor");
    glLinkProgram(prog);
   // printOpenGLError();
    glDeleteShader(verShader);
    glDeleteShader(fragShader);
    glGetProgramiv(prog, GL_LINK_STATUS, &linked);
    if (linked == GL_FALSE){
       fprintf(stderr, "Problems with linking\n");
       glGetShaderInfoLog(prog, 1024, NULL, infoLog);
       fprintf(stderr, "%s\n", infoLog); 
       glDeleteProgram(prog);
       return 0;
       }
     else fprintf(stderr, "Linking was successful\n");
    fprintf(stderr, "Using Vertex and Frag Shaders\n");
    glUseProgram(prog);
    return 1;
}



