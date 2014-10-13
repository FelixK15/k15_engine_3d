const int LT_POINT = 0;
const int LT_SPOT = 1;
const int LT_DIRECTION = 2;

struct Light
{
	vec4 position;
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

vec4 phongLightCalc(Light light, vec4 normal, vec4 eyeSpacePosition)
{
	vec4 color = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 diffuseColor = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 specularColor = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 eyeVec = normalize(vec4(0.0, 0.0, 0.0, 1.0) - eyeSpacePosition);

	vec4 lightDirection = light.position - eyeSpacePosition;
	vec4 reflection = normalize(reflect(-lightDirection, normal));
	float lightDistance = length(lightDirection);
	float attenuation = 1 - pow((lightDistance/light.radius) , 2);
	lightDirection.w = 1.0;

	lightDirection = normalize(lightDirection);

	float lightIntensity = max(0.0, dot(lightDirection, normal));
	float specularIntensity = max(0.0, dot(reflection, eyeVec));

	if(specularIntensity > 0.0)
	{
		specularIntensity = pow(specularIntensity, 25.0);
		specularColor = specularIntensity * light.specular;
	}
	
	diffuseColor = lightIntensity * light.diffuse;
	color = attenuation * (diffuseColor + specularColor);

	if(light.type == LT_SPOT)
	{
		float spot = max(0.0, dot(-lightDirection, light.direction));
		float angle = acos(spot);
		if(angle > light.coneAngle)
		{
			spot = 0.0;
		}
		else
		{
			spot = 1.0;
		}

		color = spot * color;
	}

	return color;
}