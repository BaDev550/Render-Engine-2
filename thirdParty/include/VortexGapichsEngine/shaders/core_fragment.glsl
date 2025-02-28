#version 440 core
out vec4 FragColor;

in VS_OUT{
    vec3 Position;
    vec2 TexCoord;
    vec3 Normal;
    vec4 FragPosLightSpace;
} fs_in;

struct Light {
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
    int type; // 0 = point, 1 = spotlight, 2 = directional
    float cutoff;
    float outerCutoff;
};

uniform int numLights;
uniform Light lights[10];

struct Texture {
    sampler2D texture_diffuse;
    sampler2D texture_normal;
    sampler2D texture_specular;
};

uniform Texture m_texture;
uniform vec3 cameraPos;
uniform int renderMode = 3; // 0: Default, 1: Light, 2 Texture, 3: Depth

vec3 Tonemap(vec3 x) {
	float Brightness = 0.28;
	x*= Brightness;
	float A = 0.28;
	float B = 0.29;		
	float C = 0.10;
	float D = 0.2;
	float E = 0.025;
	float F = 0.35;
	return ((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F))-E/F;
}

vec3 Tonemapping(vec3 color)
{
	vec3 curr = Tonemap(color*4.7);
	color = curr/Tonemap(vec3(15.2));	
	return color;
}

vec3 BlinnPhong(vec3 normal, vec3 fragPos, vec3 lightPos, vec3 lightColor){
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float shininess = 32.0;
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    vec3 specular = spec * lightColor;

    float distance = length(lightPos - fragPos);
    float attenuation = 1.0 / (distance * distance);

    diffuse *= attenuation;
    specular *= attenuation;
    vec3 result = diffuse + specular;

    return result;
}

void main() {
    vec3 color = texture(m_texture.texture_diffuse, fs_in.TexCoord).rgb;
    vec3 lighting;

    for (int i = 0; i < numLights; i++){
        Light light = lights[i];

        if (light.type == 0) {
            lighting += BlinnPhong(normalize(fs_in.Normal), fs_in.Position, light.position, light.color);
        } else if (light.type == 1) {
            vec3 lightDir = normalize(light.position - fs_in.Position);
            float theta = dot(lightDir, normalize(-light.direction));
            float epsilon = light.cutoff - light.outerCutoff;
            float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);
            
            vec3 spotlightEffect = light.color * intensity;
            lighting += spotlightEffect;
        } else if (light.type == 2) {
            
        }
    }

    if(renderMode == 0){
        color *= lighting;
        color = Tonemapping(color);

        FragColor = vec4(color, 1.0);
    }else if(renderMode == 1){
		FragColor = vec4(lighting, 1.0);
    }else if(renderMode == 2){
		FragColor = vec4(color, 1.0);
    }else if(renderMode == 3){
        float depth = length(fs_in.Position - cameraPos);
        FragColor = vec4(vec3(depth), 1.0);
	}
}
