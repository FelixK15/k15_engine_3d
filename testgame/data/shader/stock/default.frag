K15_Vector4 ShadeFragment(in K15_Vector2 uv : TEXCOORD1, 
						  in K15_2DSampler DiffuseSampler : DATA,
						  in K15_2DTexture DiffuseTexture : DATA)
{
	return sampleTex2D(DiffuseSampler, DiffuseTexture, uv);
}