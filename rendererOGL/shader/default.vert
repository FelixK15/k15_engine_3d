#version 330

struct Vertex
{
	vec4 position;
	vec4 color;
};

in vec4 pos;
int vec4 color;
out Vertex out_vertex;

void main(void)
{
	out_vertex.position = pos;
	out_vertex.color = color;
	gl_Position = out_vertex.position;
}