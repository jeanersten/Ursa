#version 330

in vec3 vertex_color;
in vec2 texture_coordinates;
out vec4 fragment_color;

uniform sampler2D tex0;
uniform sampler2D tex1;

void main()
{
  fragment_color = mix(texture(tex0, texture_coordinates), texture(tex1, texture_coordinates), 0.5);
}
