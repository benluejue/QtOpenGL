#version 330 core
out vec4 FragColor;



// 材质属性
struct Material {
//在着色器中，vec3 类型的变量用于存储和计算颜色值，而 sampler2D 类型的变量用于从纹理中采样颜色值
    //vec3 ambient; // 环境光颜色
    //vec3 diffuse;  // 漫反射
    sampler2D  texture_specular1;   // 镜面反射
    float shininess; // 反光度(Shininess)个物体的反光度越高，反射光的能力越强
    sampler2D texture_diffuse1;
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

uniform vec3 viewPos;  // 进行光照计算时候的"相机位置"，需要使用这个进行光照计算,在顶点着色器和片段着色器之间传递camera的坐标信息
// 因为镜面反射也和观察方向有关
uniform Material material;
uniform Light light;

// 切换灯光效果
// 是否开启半程光效
uniform bool blinn = false;
// 平行光
uniform bool direction_light = false;
// 是否为定向光
uniform bool point_light = false;
// 聚光
uniform bool spotlight = false;

void main()
{

   // ambient
   vec3 color = texture(material.texture_diffuse1, TexCoords).rgb;
   vec3 ambient = light.ambient * color;
    
    // diffuse 
    vec3 norm = normalize(Normal);
    // light.position光的位置 - 片段位置
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
        // 冯氏模型的spec计算方式
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
    
        // attenuation 计算衰减
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
