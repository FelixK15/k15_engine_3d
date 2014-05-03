precision mediump float;

attribute vec4 g_Position;
attribute vec2 g_TexCoord;

varying vec2 texcoord;

uniform mat4 g_ProjMatrix;
uniform mat4 g_ViewMatrix;
uniform mat4 g_ModelMatrix;

void main(void)
{
	mat4 mvp = g_ProjMatrix * (g_ViewMatrix * g_ModelMatrix);
	texcoord = g_TexCoord;
	gl_Position = mvp * g_Position;
}