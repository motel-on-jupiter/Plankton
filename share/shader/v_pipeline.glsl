#version 120

void main(void) {
    gl_Position = ftransform();

    // Ambient
    gl_FrontColor = gl_FrontLightProduct[0].ambient * 1.2;
    
    // Diffuse
    vec3 normalDir = normalize(gl_NormalMatrix * gl_Normal);
    vec3 lightDir = normalize(gl_LightSource[0].position.xyz);
    float diffuse = dot(normalDir, lightDir);
    if (0.0 < diffuse) {
        gl_FrontColor += gl_FrontLightProduct[0].diffuse * diffuse;
        
        // Specular
        vec3 halfDir = normalize(gl_LightSource[0].halfVector.xyz);
        float specular = pow(dot(normalDir, halfDir), gl_FrontMaterial.shininess);
        gl_FrontColor += gl_FrontLightProduct[0].specular * specular;
    }
}