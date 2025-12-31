#version 330 core

out vec4 FragColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
// texture samplers
uniform sampler2D texture1;
uniform vec3 lightColor;
uniform vec3 lightPos;
void main()
{
	vec3 tex = texture(texture1,TexCoord).rgb;
	float ambientStrength = 0.1;
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);  
	float diff = max(dot(norm,lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	vec3 ambient = ambientStrength * lightColor;
	vec3 result = (ambient + diffuse) * tex;

	// linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = vec4(result,1.0);
}


