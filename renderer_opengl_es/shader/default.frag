precision mediump float;

uniform sampler2D g_Texture_1;
varying vec2 texcoord;

void main(void)
{
	gl_FragColor = texture2D(g_Texture_1,texcoord);
}
