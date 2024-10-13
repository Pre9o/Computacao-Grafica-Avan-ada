#version 330 core
out vec4 FragColor;

uniform vec2 u_resolution;
uniform vec2 u_center;
uniform float u_zoom;
uniform float iTime;

float mandelbrot(in vec2 c)
{
    #if 1
    {
        float c2 = dot(c, c);
        // skip computation inside M1 - https://iquilezles.org/articles/mset1bulb
        if (256.0 * c2 * c2 - 96.0 * c2 + 32.0 * c.x - 3.0 < 0.0) return 0.0;
        // skip computation inside M2 - https://iquilezles.org/articles/mset2bulb
        if (16.0 * (c2 + 2.0 * c.x + 1.0) - 1.0 < 0.0) return 0.0;
    }
    #endif

    const float B = 256.0;
    float l = 0.0;
    vec2 z = vec2(0.0);
    for (int i = 0; i < 512; i++)
    {
        z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
        if (dot(z, z) > (B * B)) break;
        l += 1.0;
    }

    if (l > 511.0) return 0.0;

    // equivalent optimized smooth iteration count
    float sl = l - log2(log2(dot(z, z))) + 4.0;

    float al = smoothstep(-0.1, 0.0, sin(0.5 * 6.2831 * iTime));
    l = mix(l, sl, al);

    return l;
}

vec3 getColor(float t)
{
    float r = 9.0 * (1.0 - t) * t * t * t;
    float g = 15.0 * (1.0 - t) * (1.0 - t) * t * t;
    float b = 8.5 * (1.0 - t) * (1.0 - t) * (1.0 - t) * t;
    return vec3(r, g, b);
}

void main()
{
    vec2 p = (gl_FragCoord.xy - u_resolution / 2.0) / u_zoom + u_center;
    float l = mandelbrot(p);
    float color = l / 512.0;
    FragColor = vec4(getColor(color), 1.0);
}