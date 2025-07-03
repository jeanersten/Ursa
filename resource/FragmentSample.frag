#version 330

in vec3 color;
out vec4 fragment_color;

uniform float red;

void main()
{
  fragment_color = vec4(red, color.gb, 1.0);
}
