#version 430 core
out vec4 FragColor;

struct Light {
   vec3 position;

   vec3 ambient;
   vec3 diffuse;
};

in vec3 FragPos;  
in vec3 Normal;  

uniform vec3 viewPos;
uniform Light light;
uniform vec4 objectColor;

void main()
{
	// ambient
    vec3 ambient = light.ambient * objectColor.xyz;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * objectColor.xyz;
    vec3 result = ambient + diffuse;
    FragColor = vec4(result, 1.0);
} 