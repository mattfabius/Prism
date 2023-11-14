#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 LightPos;
in vec3 Normal;

uniform vec3 objectColor;
uniform vec3 lightColor;

float round_to_decimal_place(float value, int decimal_places);
vec3 round_to_decimal_place(vec3 value, int decimal_places);

void main()
{
    float ambientStrength = 0.25f;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0f); // greater than 90 degrees is negative so clamp to 0
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.5f;
    vec3 viewDir = normalize(-FragPos); // subtract FragPos from view position which is (0,0,0) in view coordinates
    vec3 reflectDir = reflect(-lightDir, norm); // reflect expects vector from light source pointing to fragment, lightSource currently faces opposite direction
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(round_to_decimal_place(result, 1), 1.0f);
}

float round_to_decimal_place(float value, int decimal_places)
{
    float multiplier = pow(10.0, decimal_places);
    return round(value * multiplier)/multiplier;
}

vec3 round_to_decimal_place(vec3 value, int decimal_places)
{
    vec3 result = vec3(0.0);
    result.x = round_to_decimal_place(value.x, decimal_places);
    result.y = round_to_decimal_place(value.y, decimal_places);
    result.z = round_to_decimal_place(value.z, decimal_places);
    return result;
}