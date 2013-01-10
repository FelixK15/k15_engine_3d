cbuffer PerObject : register(b0)
{
	float4 Material_Ambient;				//16
	float4 Material_Diffuse;				//32
	float4 Material_Specular;				//48

#ifdef HAS_COLOR
	float4 Color;							//64
#endif

#ifdef HAS_TEXTURE

#endif

#ifdef HAS_TEXTURE
	int Num_Textures;						//68
	Texture2D Textures[MAX_TEXTURES];		
#endif




};

#ifdef RECEIVES_LIGHT
tbuffer LightBuffer : register(b1)
{
	int Num_Lights;
	Light Lights[MAX_LIGHTS];
}
#endif

cbuffer PerFrame : register(b1)
{
	float4x4 WorldViewProjection;
}

cbuffer States : register(b2)
{
	SamplerState Sampler;
	
}