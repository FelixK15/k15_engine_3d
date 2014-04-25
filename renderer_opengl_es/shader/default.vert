attribute mediump vec4 g_Position;
attribute lowp vec2 g_TexCoord;

varying lowp vec2 ps_uv;

void main(void)
{
	ps_uv = g_TexCoord;
	gl_Position = g_Position;
}