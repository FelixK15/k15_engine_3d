#version 330


out gl_PerVertex {
        vec4 gl_Position;
        float gl_PointSize;
        float gl_ClipDistance[];
};

layout(location=0) in vec4 pos;
layout(location=1) in vec2 uv;

out vec2 out_uv;

void main(void)
{
	out_uv = uv;
	gl_Position = pos;
}