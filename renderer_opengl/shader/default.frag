#version 330

in vec2 uv;
uniform sampler2D g_Texture_1;

void main(void)
{
	gl_FragColor = texture2D(g_Texture_1,uv);
}
