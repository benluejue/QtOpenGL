#version 330 core
out vec4 FragColor;



// 材质属性
struct Material {
//在着色器中，vec3 类型的变量用于存储和计算颜色值，而 sampler2D 类型的变量用于从纹理中采样颜色值
    //vec3 ambient; // 环境光颜色
    //vec3 diffuse;  // 漫反射
    sampler2D  specular;   // 镜面反射
    float shininess; // 反光度(Shininess)个物体的反光度越高，反射光的能力越强
    sampler2D diffuse;
    // sampler2D emission; //放射光贴图
}; 
// 光属性
struct Light {
    vec3 position;  // 光源位置
    vec3 direction; // 平行光
    float cutOff;   
    float outerCutOff;

    vec3 ambient;   // 环境光的颜色
    vec3 diffuse;   // iffuse: 漫反射光的颜色
    vec3 specular;  // specular: 镜面光的颜色
    // 对应方程里面的kc,kl,kq
    float constant;
    float linear;
    float quadratic;

};


in vec3 Normal;  // 法向量
in vec3 FragPos; // 顶点
in vec2 TexCoords;// 纹理顶点

uniform vec3 viewPos;  // 进行光照计算时候的相机位置，需要使用这个进行光照计算,在顶点着色器和片段着色器之间传递camera的坐标信息
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
