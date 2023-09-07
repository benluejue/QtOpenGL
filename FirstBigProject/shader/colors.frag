#version 330 core
out vec4 FragColor;



// ��������
struct Material {
//����ɫ���У�vec3 ���͵ı������ڴ洢�ͼ�����ɫֵ���� sampler2D ���͵ı������ڴ������в�����ɫֵ
    //vec3 ambient; // ��������ɫ
    //vec3 diffuse;  // ������
    sampler2D  texture_specular1;   // ���淴��
    float shininess; // �����(Shininess)������ķ����Խ�ߣ�����������Խǿ
    sampler2D texture_diffuse1;
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

uniform vec3 viewPos;  // ���й��ռ���ʱ���"���λ��"����Ҫʹ��������й��ռ���,�ڶ�����ɫ����Ƭ����ɫ��֮�䴫��camera��������Ϣ
// ��Ϊ���淴��Ҳ�͹۲췽���й�
uniform Material material;
uniform Light light;

// �л��ƹ�Ч��
// �Ƿ�����̹�Ч
uniform bool blinn = false;
// ƽ�й�
uniform bool direction_light = false;
// �Ƿ�Ϊ�����
uniform bool point_light = false;
// �۹�
uniform bool spotlight = false;

void main()
{

   // ambient
   vec3 color = texture(material.texture_diffuse1, TexCoords).rgb;
   vec3 ambient = light.ambient * color;
    
    // diffuse 
    vec3 norm = normalize(Normal);
    // light.position���λ�� - Ƭ��λ��
    vec3 lightDir = normalize(light.position - FragPos);
    if(direction_light){
        lightDir =  normalize(-light.position);
    }else{
        lightDir = normalize(light.position - FragPos);
    }

    // A = [4 -1 2];
    // B = [2 -2 -1]; dot(A,B) = 8
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse1, TexCoords).rgb;  
    
    // specular
    float spec = 0.0;
    vec3 viewDir = normalize(viewPos - FragPos);
    if(blinn){
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0);
    }else{
        // ����ģ�͵�spec���㷽ʽ
        vec3 reflectDir = reflect(-lightDir, norm); 
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);    
    }

    
    vec3 specular = light.specular * spec * texture(material.texture_specular1, TexCoords).rgb;  
    if(spotlight){
        // spotlight (soft edges)
        float theta = dot(lightDir, normalize(-light.direction)); 
        float epsilon = (light.cutOff - light.outerCutOff);
        float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
        diffuse  *= intensity;
        specular *= intensity;
    
        // attenuation ����˥��
        float distance    = length(light.position - FragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
        ambient  *= attenuation; 
        diffuse   *= attenuation;
        specular *= attenuation;   
    }
    if(point_light){
         float distance    = length(light.position - FragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

        ambient  *= attenuation;  
        diffuse   *= attenuation;
        specular *= attenuation;   
    }
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
  
   
}
