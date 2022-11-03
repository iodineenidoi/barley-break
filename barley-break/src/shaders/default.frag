#version 330 core

in vec2 UV;

out vec4 FragColor;

uniform sampler2D u_mainTex;
struct imageSplitter
{
	int texRowIndex;
	int texColumnIndex;
	int texturesInRow;
	int texturesInColumn;
};
uniform imageSplitter u_imageSplitter;

void main()
{
	vec2 uvOffset = vec2(1.0 / u_imageSplitter.texturesInRow, 1.0 / u_imageSplitter.texturesInColumn);
	
	vec2 uv = vec2(UV.x / u_imageSplitter.texturesInRow, UV.y / u_imageSplitter.texturesInColumn);
	uv.x += uvOffset.x * u_imageSplitter.texRowIndex;
	uv.y += uvOffset.y * u_imageSplitter.texColumnIndex;

	FragColor = texture(u_mainTex, uv);
}