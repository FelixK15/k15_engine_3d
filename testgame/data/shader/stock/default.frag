K15_Vector4 ShadeFragment(in K15_Vector2 uv : TEXCOORD1, 
						  in K15_2DSampler sampler : DATA,
						  in K15_2DTexture tex : DATA)
{
<<<<<<< HEAD
	K15_Vector4 c = sampleTex2D(sampler, tex, uv);
	return c;
=======
	K15_Vector4 color = sampleTex2D(DiffuseSampler, DiffuseTexture, uv); 
	return K15_Vector4(color.r, color.r, color.r, color.r);
>>>>>>> 3630a7e93c86249517bb3dc3fffae82c960d0f0d
}