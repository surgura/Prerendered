uniform sampler2D colormap;
uniform sampler2D normalmap;
uniform vec3 lightpos;
 
void main()
{
    //vec4 color = texture2D(colormap,gl_TexCoord[0].st);
    //gl_FragColor = color;



    vec3 normal, lightDir;
	vec4 diffuse, ambient;
    float NdotL;

    normal = normalize(texture2D(normalmap,gl_TexCoord[0].st));
    lightDir = normalize(lightpos);
    NdotL = max(dot(normal, lightDir), 0.0);
    diffuse = texture2D(colormap,gl_TexCoord[0].st);
    ambient = texture2D(colormap,gl_TexCoord[0].st) * 0;
    gl_FragColor =  NdotL * diffuse + ambient;

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