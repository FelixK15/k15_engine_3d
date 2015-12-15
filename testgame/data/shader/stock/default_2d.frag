K15_Vector4 ShadeFragment(in K15_Vector2 uv : TEXCOORD1, 
						  in K15_Vector2 texDim : TEXCOORD_DIMENSION1,
						  in K15_2DSampler sampler : DATA,
						  in K15_2DTexture tex : DATA)
{
	uv.x += 0.5f/texDim.x;
	uv.y += 0.5f/texDim.y;

	K15_Vector4 c = sampleTex2D(sampler, tex, uv);
	return K15_Vector4(c.r, c.g, c.b, 1.0f);
}