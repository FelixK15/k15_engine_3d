K15_Vector4 ShadeFragment(in K15_Vector2 uv : TEXCOORD1, 
						  in K15_2DSampler sampler : DATA,
						  in K15_2DTexture tex : DATA)
{
	K15_Vector4 color = sampleTex2D(sampler, tex, uv);
	return K15_Vector4(color.r, 0.f, 0.f, color.r);
}