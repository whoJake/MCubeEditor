#version 450

// layout (set=0, binding=0) uniform mat4 matrix;

layout (location=0) in vec3 in_position;

layout (location=0) out vec4 out_color;

void main()
{
  // gl_Position = matrix * vec4(in_position, 1.0);
  gl_Position = vec4(in_position, 1.0);
  out_color = vec4(in_position, 1.0);
}