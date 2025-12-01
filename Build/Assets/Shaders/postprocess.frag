#version 430

#define GRAYSCALE  (1 << 0)
#define COLORTINT  (1 << 1)
#define SCANLINE   (1 << 2)
#define GRAIN      (1 << 3)
#define INVERT     (1 << 4)

in vec2 v_texcoord;

out vec4 f_color;

// post process controls
uniform uint u_parameters = 0;
uniform vec3 u_colorTint = vec3(1, 1, 1);

uniform float u_time = 0;
uniform float u_intensity = 1.0;

uniform sampler2D u_baseMap;

// pseudo-random grain function
float random(vec2 st)
{
    return fract(sin(dot(st, vec2(12.9898, 78.233))) * 43758.5453123);
}

void main()
{
    vec4 color = texture(u_baseMap, v_texcoord);
    vec4 postprocess = color;

    // GRAYSCALE
    if ((u_parameters & GRAYSCALE) != 0u)
    {
        float gray = (color.r + color.g + color.b) / 3.0;
        postprocess = vec4(gray, gray, gray, color.a);
    }

    // COLOR TINT
    if ((u_parameters & COLORTINT) != 0u)
    {
        postprocess.rgb *= u_colorTint;
    }

    // SCANLINE (every third line is visible)
    if ((u_parameters & SCANLINE) != 0u)
    {
        postprocess = (int(gl_FragCoord.y) % 3 != 0)
                      ? vec4(0, 0, 0, 1)
                      : postprocess;
    }

    // GRAIN / NOISE
    if ((u_parameters & GRAIN) != 0u)
    {
        float n = random(gl_FragCoord.xy + u_time);
        postprocess.rgb *= mix(1.0, n, u_intensity);   // intensity controls grain strength
    }

    // INVERT COLOR
    if ((u_parameters & INVERT) != 0u)
    {
        postprocess.rgb = vec3(1.0) - postprocess.rgb;
    }

    f_color = postprocess;
}