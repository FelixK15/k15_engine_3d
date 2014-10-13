in vec3 a_Position;
in vec3 a_Normal;
in vec2 a_TexCoord;

uniform mat4 g_ModelMatrix;
uniform mat4 g_ProjMatrix;
uniform mat4 g_ViewMatrix;
uniform mat4 g_NormalMatrix;

out vec2 v_UV;
out vec4 v_Position;
out vec4 v_Normal;
out vec4 v_ESPosition;

void main(void)
{
	mat4 mv = g_ModelMatrix * g_ViewMatrix;
	mat4 mvp = g_ProjMatrix * g_ViewMatrix * g_ModelMatrix;
	mat4 normalMat = transpose(inverse(mv));
	vec4 transformedPosition = mvp * vec4(a_Position, 1.0);
	v_UV = a_TexCoord;
	v_Normal = normalize(mv * vec4(a_Normal, 0.0));
	v_Position = transformedPosition;
	v_ESPosition = mv * vec4(a_Position, 1.0);
	gl_Position = transformedPosition;
}