#version 120 

attribute  vec3 VertexPosition;
attribute  vec3 VertexColor;

uniform mat4 RotationMatrix;

varying vec3 Color;
 
void main(void)
{
  gl_Position =  RotationMatrix *  vec4(VertexPosition, 1.0);
  Color = VertexColor;
}
