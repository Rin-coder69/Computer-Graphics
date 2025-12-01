#version 460 core

#define MAX_LIGHTS 5
#define POINT 0
#define DIRECTIONAL 1
#define SPOT 2

#define BASE_MAP		(1 << 0)
#define	SPECULAR_MAP	(1 << 1)
#define	EMISSIVE_MAP	(1 << 2)
#define NORMAL_MAP		(1 << 3)
#define CUBE_MAP (1 << 4)
#define SHADOW_MAP (1 << 5)


in VS_OUT
{
	vec2 texcoord;
	vec3 position;
	vec3 normal;
	vec4 shadowcoord;
	mat3 tbn;
} fs_in;

out vec4 f_color;

struct Light
{
	int type;
	vec3 position;
	vec3 direction;
	vec3 color;
	float range;
	float intensity;
	float outerSpotAngle;
	float innerSpotAngle;
	bool shadowCaster;
};

struct Material
{
	vec3 baseColor;
	vec3 emissiveColor;
	float shininess;
	vec2 tiling;
	vec2 offset;
	uint parameters;
};

uniform vec3 u_ambient_light;

uniform Material u_material;
uniform sampler2D u_baseMap;
uniform sampler2D u_specularMap;
uniform sampler2D u_emissiveMap;
uniform sampler2D u_shadowMap;
uniform sampler2D u_normalMap;

uniform Light u_lights[5];
uniform int u_numLights;

float calculateAttenuation(in float light_distance, in float range)
{	
	float attenuation = max( 0.0, (1.0 - (light_distance / range)));
	return pow(attenuation, 2.0);
} 

vec3 calculateNormal()
{
	// generate the normals from the normal map
	vec3 normal = texture(u_normalMap, fs_in.texcoord).rgb;
	// convert rgb normal (0 <-> 1) to xyx (-1 <-> 1)
	normal = normalize((normal * 2)-1);
	// transform normals to model view space
	normal = normalize(fs_in.tbn * normal);
 
	return normal;
}

float calculateShadow(in vec4 shadowcoord, in float bias){
return texture(u_shadowMap, shadowcoord.xy).z < shadowcoord.z ? 0.0 : 1.0;
}

vec3 calculateLight(in Light light,in vec3 position, in vec3 normal, in float specularMask)
{
	float attenuation;
	vec3 light_dir;
	switch(light.type){
	case POINT:
			{
			light_dir = normalize(light.position - position);

			float light_distance = length(light.position - position);
			attenuation = calculateAttenuation(light_distance, light.range);
			}
			//point light calculations
			break;
		case DIRECTIONAL:
		light_dir = -light.direction;
		attenuation = 1.0;
			//directional light calculations
			break;
		case SPOT:
		{
			light_dir = normalize(light.position - position);
			float light_distance = length(light.position - position);
			attenuation = calculateAttenuation(light_distance, light.range);
			//spot light calculations

			float angle = acos(dot(light_dir, -light.direction));
			if(angle > light.outerSpotAngle) attenuation = 0.0;
			else
			{
			float spotAttenuation = smoothstep(light.outerSpotAngle + 0.001, light.innerSpotAngle, angle);
			attenuation *= spotAttenuation;
			}
			}
			break;
	}
	//diffuse
	
	float intensity = max(dot(normal,light_dir),0);
	float NdotL = max(dot(normal,light_dir),0);
	vec3 diffuse = light.color * u_material.baseColor * NdotL;
	//vec3 diffuse = light.color * u_material.baseColor * NdotL;

	//specular
	//vec3 reflection = reflect(-light_dir,normal);
	vec3 view_dir = normalize(position);
	//intensity = max(dot(reflection,view_dir),0);
	//intensity =  pow(intensity,128);
	//float specular = max(dot(reflection,view_dir),0);//ask about removing or not

	//blinn phong
	vec3 halfway_dir = normalize(light_dir + view_dir);
	float NdotH = max(dot(normal,halfway_dir),0);
	NdotH = pow(NdotH, u_material.shininess);
	vec3 specular = vec3(NdotH) * specularMask;

	return (diffuse + specular) * light.intensity * attenuation;

}

void main()
{
	//vec3 color = calculateLight(fs_in.position, fs_in.normal);
	vec3 color = u_ambient_light;
	 
	vec3 normal = ((u_material.parameters & NORMAL_MAP) != 0u)
	? calculateNormal()
	: fs_in.normal;

	float specularMask = ((u_material.parameters & SPECULAR_MAP) != 0u)
	? texture(u_specularMap, fs_in.texcoord).r 
	: 1.0;
 for (int i = 0; i < u_numLights; i++)
 {
  float shadow = (u_lights[i].shadowCaster && ((u_material.parameters & SHADOW_MAP) != 0u))
   ? calculateShadow(fs_in.shadowcoord, 0)
   : 1.0;


  color += calculateLight(u_lights[i], fs_in.position, normal, specularMask) * shadow;
 }

	vec4 emissive = ((u_material.parameters & EMISSIVE_MAP) != 0u)
  ? texture(u_emissiveMap, fs_in.texcoord) * vec4(u_material.emissiveColor, 1)
  : vec4(u_material.emissiveColor, 1);
 //vec4 emisive = vec4(u_material.emissiveColor,1);*/
	//vec4 emissive = vec4(u_material.emissiveColor, 1);
	//emissive = vec4(u_material.emissiveColor, 1);
	
	f_color = texture(u_baseMap, fs_in.texcoord) * vec4(color, 1) + emissive;
	

}