#version 460 core
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 objectColor = vec3(1.0, 1.0, 1.0);
uniform vec3 lightColor  = vec3(1.0, 1.0, 1.0);
uniform vec3 lightPos    = vec3(1.2, 1.0, 2.0);
uniform vec3 viewPos; // Kamera-Position

void main()
{
    // --- Lichtberechnung (einfaches Phong Lighting) ---
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    // Diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Ambient
    vec3 ambient = 0.1 * lightColor;

    // Ergebnisfarbe
    vec3 result = (ambient + diffuse) * objectColor;

    FragColor = vec4(result, 1.0);
}