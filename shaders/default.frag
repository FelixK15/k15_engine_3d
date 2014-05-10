in vec2 v_UV;
uniform sampler2D g_Texture_1;

void main(void)
{
	gl_FragColor = texture2D(g_Texture_1,v_UV);
}
