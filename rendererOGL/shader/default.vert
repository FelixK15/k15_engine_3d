#version 330


out gl_PerVertex {
        vec4 gl_Position;
        float gl_PointSize;
        float gl_ClipDistance[];
};

layout(location=0) in vec4 pos;
layout(location=1) in vec4 color;

out vec4 out_color;

void main(void)
{
	out_color = color;
	gl_Position = pos;
}