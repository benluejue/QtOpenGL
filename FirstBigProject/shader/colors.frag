#version 330 core
out vec4 FragColor;



// ��������
struct Material {
//����ɫ���У�vec3 ���͵ı������ڴ洢�ͼ�����ɫֵ���� sampler2D ���͵ı������ڴ������в�����ɫֵ
    //vec3 ambient; // ��������ɫ
    //vec3 diffuse;  // ������
    sampler2D  specular;   // ���淴��
    float shininess; // �����(Shininess)������ķ����Խ�ߣ�����������Խǿ
    sampler2D diffuse;
    // sampler2D emission; //�������ͼ
}; 
// ������
struct Light {
    vec3 position;  // ��Դλ��
    vec3 direction; // ƽ�й�
    float cutOff;   
    float outerCutOff;

    vec3 ambient;   // ���������ɫ
    vec3 diffuse;   // iffuse: ����������ɫ
    vec3 specular;  // specular: ��������ɫ
    // ��Ӧ���������kc,kl,kq
    float constant;
    float linear;
    float quadratic;

};


in vec3 Normal;  // ������
in vec3 FragPos; // ����
in vec2 TexCoords;// ������

uniform vec3 viewPos;  // ���й��ռ���ʱ������λ�ã���Ҫʹ��������й��ռ���,�ڶ�����ɫ����Ƭ����ɫ��֮�䴫��camera��������Ϣ
uniform Material material;
uniform Light light;

void main()
{

   // ambient
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  
    
    // spotlight (soft edges)
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;
    
    // attenuation
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    ambient  *= attenuation; 
    diffuse   *= attenuation;
    specular *= attenuation;   
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
