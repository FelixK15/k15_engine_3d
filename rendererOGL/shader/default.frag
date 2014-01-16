#version 330

struct Vertex
{
	vec4 position;
	vec4 color;
};

in Vertex in_vertex;

void main(void)
{
	gl_FragColor = in_vertex.color;
}
