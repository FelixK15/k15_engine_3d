#include "light.glsl"

in vec3 a_Position;
in vec3 a_Normal;
in vec2 a_TexCoord;

uniform mat4 g_ModelMatrix;
uniform mat4 g_ProjMatrix;
uniform mat4 g_ViewMatrix;
uniform mat4 g_NormalMatrix;

uniform Light g_Lights[8];
uniform int g_LightCount;

out vec2 v_UV;
out vec4 v_Position;
out vec4 v_Normal;
out vec4 v_PositionES;
out vec4 v_Color;

void main(void)
{
	mat4 mv = g_ModelMatrix * g_ViewMatrix;
	mat4 mvp = g_ProjMatrix * g_ViewMatrix * g_ModelMatrix;
	vec4 transformedPosition = mvp * vec4(a_Position, 1.0);
	v_UV = a_TexCoord;
	v_Normal = normalize(mv * vec4(a_Normal, 0.0));
	v_Position = transformedPosition;
	v_PositionES = mv * vec4(a_Position, 1.0);

	v_Color = phongLightCalc(g_Lights[0], v_Normal, v_PositionES);

	gl_Position = transformedPosition;
}