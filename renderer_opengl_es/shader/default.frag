varying lowp vec2 ps_uv;
uniform lowp sampler2D g_Diffuse;

void main(void)
{
	gl_FragColor = texture2D(g_Diffuse,ps_uv);
}
