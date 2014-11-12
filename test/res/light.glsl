const int LT_POINT = 0;
const int LT_SPOT = 1;
const int LT_DIRECTION = 2;

struct Light
{
	vec4 positionES;
	vec4 diffuse;
	vec4 specular;
	vec4 direction;
	float constantAttenuation;
	float linearAttenuation;
	float quadricAttenuation;
	float radius;
	float coneAngle;
	float spotExponent;
	int type;
};

vec4 phongLightCalc(Light p_Light, vec4 p_Normal, vec4 p_PositionES)
{
	vec4 color = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	vec4 lightDirectionES = p_Light.positionES - p_PositionES;
	float lightLength = length(lightDirectionES);

	lightDirectionES = normalize(lightDirectionES);

	float lambert = max(dot(p_Normal, lightDirectionES), 0.0f);

	color = lambert * p_Light.diffuse;
	color.w = 1.0f;

	return vec4(lambert, lambert, lambert, 1.0f);
}