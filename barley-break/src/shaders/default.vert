#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

out vec2 UV;

struct rect
{
	vec2 resolution;
    vec2 size;
    vec2 pos;
};
uniform rect u_rect;

void main()
{
	vec2 pixelSize = vec2(2.0 / u_rect.resolution.x, 2.0 / u_rect.resolution.y);
	vec2 rectScale = vec2(u_rect.size.x / u_rect.resolution.x, u_rect.size.y / u_rect.resolution.y);

	vec2 scaledPos = aPos.xy * rectScale;
	vec2 result = scaledPos + pixelSize * u_rect.pos;

	gl_Position = vec4(result, 0.0, 1.0);
	UV = aUV;
}