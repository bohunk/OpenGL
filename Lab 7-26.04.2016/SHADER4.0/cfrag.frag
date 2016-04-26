#version 410

in vec3 Color;

out vec4 FragColor;

void main(void) {
 FragColor = vec4(Color,1.0);
}

