#version 400

layout(triangles) in;  
layout(triangle_strip, max_vertices = 3) out;  

in vec4 tePosition[];
in vec2 teTexCoord[];

out vec4 gPosition;
out vec2 gTexCoord;
out vec3 gNormal;

void main() {
    vec3 edge1 = vec3(tePosition[1]) - vec3(tePosition[0]);
    vec3 edge2 = vec3(tePosition[2]) - vec3(tePosition[0]);
    
    vec3 normal = normalize(cross(edge1, edge2));

    for (int i = 0; i < 3; i++) {
        gTexCoord = teTexCoord[i];
        gPosition = tePosition[i];
        gl_Position = gl_in[i].gl_Position;
        gNormal = normal;
        EmitVertex();
    }
    EndPrimitive();
}
