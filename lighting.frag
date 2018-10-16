#version 130

uniform sampler2D colormap;
uniform sampler2D normalmap;
uniform sampler2D positionmap;
uniform vec3 position;
uniform vec3 lightpos;
 
void main()
{
    vec3 normal, lightDir, relativepos, absolutepos;
	vec4 diffuse, ambient;
    float ndotl;
    float depth;
    float pixelscale = 1024/8; // 1 positionmap position unit is 128 pixels
    float distfactor;

    relativepos = texture2D(positionmap,gl_TexCoord[0].st).xyz;
    absolutepos = position + relativepos*pixelscale;

    normal = normalize(texture2D(normalmap,gl_TexCoord[0].st).rgb);
    lightDir = normalize(absolutepos - lightpos);
    distfactor = 0.005*distance(absolutepos, lightpos);
    distfactor = min(1, 0.5*1.0f / (distfactor*distfactor));
    ndotl = max(dot(normal, -lightDir), 0.0);
    diffuse = texture2D(colormap,gl_TexCoord[0].st);
    gl_FragColor = vec4(diffuse.rgb*0.0f+distfactor*ndotl*diffuse.rgb, diffuse.a);

    //realpos = normalize(texture2D(positionmap,gl_TexCoord[0].st);
    // depth = -y + tan(cam_angle)*rendered_z*distfactor
    //depth = texture2D(positionmap, gl_TexCoord[0].st).r;
    //depth = position
    //gl_FragDepth = depth / 1000.0f;
    //depth = realpos
    //gl_FragDepth = 
    absolutepos /= 10000.0f;
    depth = 0.5f;
    depth += 10*(absolutepos.x*-0.70710678118f + absolutepos.y*0.70710678118f); // why does this work????
    depth += absolutepos.z;

    if (diffuse.a < 0.1f)
        depth = 1;
    gl_FragDepth = depth;

    /*
        

    depth = absolutepos.x*-0.70710678118f + absolutepos.y*0.70710678118f;
    depth *= 0.5f;
    depth += absolutepos.z;
    depth = 0.5 - depth;
        */


    //gl_FragColor = vec4(absolutepos.x+0.5f, absolutepos.y+0.5f, absolutepos.z+0.5f, gl_FragColor.a);
    //gl_FragColor = vec4(position.z / 1000.0f, position.z / 1000.0f, position.z / 1000.0f, gl_FragColor.a);
}