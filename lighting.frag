uniform sampler2D colormap;
uniform sampler2D normalmap;
uniform vec3 lightpos;
uniform sampler2D depthmap;
 
void main()
{
    //vec4 color = texture2D(colormap,gl_TexCoord[0].st);
    //gl_FragColor = color;



    vec3 normal, lightDir;
	vec4 diffuse, ambient;
    float NdotL;
    float depth;

    normal = normalize(texture2D(normalmap,gl_TexCoord[0].st)).rgb;
    depth = texture2D(depthmap, gl_TexCoord[0].st).r;
    lightDir = normalize(lightpos);
    NdotL = max(dot(normal, lightDir), 0.0) + 0.0001*depth;
    diffuse = texture2D(colormap,gl_TexCoord[0].st);
    ambient = texture2D(colormap,gl_TexCoord[0].st) * 0;
    gl_FragColor = vec4((NdotL * diffuse).rgb, diffuse.a) + ambient;
    gl_FragDepth = depth / 1000.0f;

/*
    vec3 normal, lightDir;
	vec4 diffuse, ambient, globalAmbient;
	float NdotL;

	normal = normalize(gl_NormalMatrix * gl_Normal);
	lightDir = normalize(vec3(gl_LightSource[0].position));
	NdotL = max(dot(normal, lightDir), 0.0);
	diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;

	ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
	globalAmbient = gl_LightModel.ambient * gl_FrontMaterial.ambient;
	gl_FrontColor =  NdotL * diffuse + globalAmbient + ambient;*/
}