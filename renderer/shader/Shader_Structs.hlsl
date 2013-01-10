struct Light
{
	float4 Ambient;		//16
	float4 Diffuse;		//32
	float4 Specular;	//48

	int Type;			//52

	float3 Position;	//64
	float Range;		//68

	float3 Attenuation;	//80
	float3 Direction;	//92
	float pad;			//96
};