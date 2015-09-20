K15_Vector4 TransformVertex(in K15_Vector4 pos : POSITION,
							in K15_Vector2 uv : TEXCOORD1,
							out K15_Vector2 out_uv : TEXCOORD1,
							in K15_Matrix4 modelMatrix : MODELMATRIX,
							in K15_Matrix4 projMatrix : PROJECTIONMATRIX,
							in K15_Matrix4 viewMatrix : VIEWMATRIX)
{
	K15_Matrix4 ModelViewProj = projMatrix * (viewMatrix * modelMatrix);
	K15_Vector4 transformedPosition = projMatrix * pos;
	out_uv = uv;

	return transformedPosition;
}