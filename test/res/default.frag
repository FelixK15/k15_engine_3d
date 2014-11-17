#include "light.glsl"

in vec2 v_UV;
in vec4 v_Position;
in vec4 v_Normal;
in vec4 v_PositionES;
in vec4 v_Color;

uniform Light g_Lights[8];
uniform vec4 g_AmbientColor;
uniform int g_LightCount;

void main(void)
{
	vec4 color = vec4(0.0, 0.0, 0.0, 1.0);
	/*for(int i = 0; i < g_LightCount; ++i)
	{
		Light currentLight = g_Lights[i];
		vec4 lightColor = phongLightCalc(currentLight, v_Normal, v_PositionES);
		color += lightColor;
	}

	gl_FragColor = color + g_AmbientColor;*/


	/*color = v_Normal;
	color += vec4(1.0, 1.0, 1.0, 0.0);
	color /= 2.f;
	color.a = 1.0;

	gl_FragColor = color;*/

	gl_FragColor = v_Color;
}
