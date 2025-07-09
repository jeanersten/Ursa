#version 330

layout (location = 0) in vec3 in_vertex_position;
layout (location = 1) in vec3 in_vertex_color;
layout (location = 2) in vec2 in_texture_coordinates;

out vec3 vertex_color;
out vec2 texture_coordinates;

uniform mat4 transform;

void main()
{
  gl_Position = transform * vec4(in_vertex_position, 1.0);
  vertex_color = in_vertex_color;
  texture_coordinates = in_texture_coordinates;
}
