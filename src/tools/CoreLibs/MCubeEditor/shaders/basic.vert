#version 450

layout ( push_constant ) uniform constants
{
  mat4 proj;
  mat4 view;
  mat4 model;
} pc_matrices;

layout (location=0) in vec3 in_position;
layout (location=1) in vec3 in_colour;

layout (location=0) out vec4 out_color;

void main()
{
  gl_Position = (pc_matrices.proj * pc_matrices.view * pc_matrices.model) * vec4(in_position, 1.0);
  out_color = vec4(in_colour, 1.0);
}