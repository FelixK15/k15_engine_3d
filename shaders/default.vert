in vec4 a_Position;
in vec2 a_TexCoord;

uniform mat4 g_ModelMatrix;
uniform mat4 g_ProjMatrix;
uniform mat4 g_ViewMatrix;

out vec2 v_UV;

void main(void)
{
	mat4 mvp = g_ProjMatrix * (g_ViewMatrix * g_ModelMatrix);
	v_UV = a_TexCoord;
	gl_Position = mvp * a_Position;
}