#version 450

layout(location = 0) in vec4 v_Color;

layout(location = 0) out vec4 o_Color;
layout(set = 0, binding = 0) buffer Atomics { uint count; }atomics;

void main() {
	atomicAdd(atomics.count, 1);
	o_Color = v_Color;
}