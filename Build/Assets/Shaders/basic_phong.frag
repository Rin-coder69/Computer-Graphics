#version 460 core
in VS_OUT
{
in vec2 texcoord;
in vec3 position;
in vec3 normal;
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
};

struct Material
{
	sampler2D baseMap;
	vec3 baseColor;

	float shininess;
	vec2 tiling;
	vec2 offset;
};

uniform vec3 u_ambient_light;
uniform sampler2D u_texture;
uniform Material u_material;
uniform Light u_lights[5];
uniform int u_numLights;

vec3 calculateLight(in Light light,in vec3 position, in vec3 normal)
{

    vec3 light_dir = normalize(light.position - position);
    float intensity = max(dot(light_dir, normal), 0);
    //vec3 diffuse = light.color * material.baseColor * intensity;
	vec3 diffuse = light.color * intensity * u_material.baseColor;
	
	//phong
	//vec3 reflection = reflect(-light_dir, normal);
	//vec3 view_dir = normalize(-position);
	//intensity = max(dot(reflection, view_dir),0);

	//blinn-phong
	vec3 view_dir = normalize(-position);
	vec3 halfway_dir = normalize(light_dir + view_dir);
	intensity = max(dot(normal, halfway_dir), 0);

	intensity = pow(intensity, u_material.shininess);
	vec3 specular = vec3(intensity);

	return u_ambient_light + diffuse + specular;

}

void main()
{
	//vec3 color = calculateLight(fs_in.position, fs_in.normal);
	vec3 color = u_ambient_light;

	 for (int i = 0; i < u_numLights; i++) 
	{
		//v_out.color += calculateLight(u_lights[i], position, normal);
		color += calculateLight(u_lights[i], fs_in.position,fs_in.normal);

	}
	
	f_color = texture(u_material.baseMap, fs_in.texcoord) * vec4(color, 1);
	

}