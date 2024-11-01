#version 400

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in uint OuterTess;
layout(location = 3) in uint InnerTess;

out vec2 vTexCoord;
out vec4 vPosition;
out uint vOuter;
out uint vInner;

void main()
{
    vTexCoord = TexCoord;
    vPosition = vec4(VertexPosition, 1.0);
    vOuter = OuterTess;
    vInner = InnerTess;
}