#version 330 core
layout (location = 0) in vec3 aPos;
 
uniform mat4 view;
uniform mat4 projection;
 
out vec3 TexCoords;
 
void main(){
	TexCoords = aPos;
	vec4 pos = projection * view * vec4(aPos, 1.0f);
	//	���pos.xyww�ر�ħ�ԣ�һ��Ĭ�ϴ���ֵ������pos.xyzw��
	//  ��������z/w����ֵ������Ƭ�ε����ֵ�����Ե��Ľ�z��Ϊwʱ��w/w=1.0f������պ����е�Ƭ����Ƚ�Ϊ1.0f��
	gl_Position = pos.xyww;
}
