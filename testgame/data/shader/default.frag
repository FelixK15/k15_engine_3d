in vec2 v_UV;
uniform sampler2D DiffuseTexture;

void main(void)
{
	gl_FragColor = texture2D(DiffuseTexture, v_UV);
}