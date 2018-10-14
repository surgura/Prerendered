#version 130

uniform sampler2D colormap;
uniform sampler2D normalmap;
uniform vec3 lightpos;
uniform sampler2D positionmap;
uniform float scaley;
 
void main()
{
    vec3 normal, lightDir;
	vec4 diffuse, ambient;
    float ndotl;
    float depth;
    float pixelscale = 1024/8; // 1 positionmap position unit is 128 pixels
    float distfactor;

    normal = normalize(texture2D(normalmap,gl_TexCoord[0].st).rgb);
    lightDir = normalize(texture2D(positionmap,gl_TexCoord[0].st).rgb*pixelscale - lightpos);
    distfactor = 0.01*distance(texture2D(positionmap,gl_TexCoord[0].st).rgb*pixelscale, lightpos);
    distfactor = 1.0f / (distfactor*distfactor);
    ndotl = max(dot(normal, -lightDir), 0.0);
    diffuse = texture2D(colormap,gl_TexCoord[0].st);
    gl_FragColor = vec4(distfactor*ndotl*diffuse.rgb, diffuse.a);
    //gl_FragColor = 0.0001*vec4(ndotl*diffuse.rgb, diffuse.a) + vec4(texture2D(positionmap,gl_TexCoord[0].st).rgb, diffuse.a);

    depth = texture2D(positionmap, gl_TexCoord[0].st).r;
    gl_FragDepth = depth / 1000.0f;
    
}