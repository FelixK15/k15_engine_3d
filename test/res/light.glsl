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
	float specularExponent;
	int type;
};

float attenuationSimple(Light p_Light, float p_LightLength)
{
	float attenuation = 1.0f - pow(p_LightLength / p_Light.radius, 2); 

	return attenuation;
}

vec4 phongLightCalc(Light p_Light, vec4 p_Normal, vec4 p_PositionES)
{
	vec4 color = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	vec4 lightDirectionES = p_Light.positionES - p_PositionES;
	vec4 reflectionES = normalize(reflect(lightDirectionES, p_Normal));
	vec4 toEyeES = normalize(-p_PositionES);
	float lightLength = length(lightDirectionES);

	lightDirectionES = normalize(lightDirectionES);

	float lambert = max(dot(p_Normal, lightDirectionES), 0.0f);
	float attenuation = attenuationSimple(p_Light, lightLength);

	float specularIntensity = max(dot(toEyeES, -reflectionES), 0.0f);
	specularIntensity = pow(specularIntensity, p_Light.specularExponent);
	color = lambert * p_Light.diffuse;
	color += specularIntensity * p_Light.specular;
	color *= attenuation;

	color.w = 1.0f;

	return color;
}