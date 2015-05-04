in vec3 a_Position;
in vec4 a_Color;

//uniform mat4 g_ModelMatrix;
uniform mat4 g_ProjMatrix;
uniform mat4 g_ViewMatrix;

out vec4 v_Color;

void main(void)
{
	mat4 mvp = g_ProjMatrix * g_ViewMatrix;// * g_ModelMatrix;

	v_Color = a_Color;

	gl_Position = mvp * vec4(a_Position, 1.0);
}