#version 120

attribute vec3 VertexPosition;   //in for OpenGL 4.0
attribute vec3 VertexColor;

//uniform mat4 RotationMatrix;

varying vec3 Color;    //out for OpenGL 4.0
 
void main(void)
{
  gl_Position =  vec4(VertexPosition, 1.0);
  Color = VertexColor; 
}
