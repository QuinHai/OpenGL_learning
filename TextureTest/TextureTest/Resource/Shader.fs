#version 450 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 ourTexCoord;

uniform sampler2D ourTexture1;//���������ʹ�õ��ڽ���������sampler<1D,2D,3D>������
uniform sampler2D ourTexture2;

void main()
{
	//FragColor = texture(outTexture, ourTexCoord);//������������� �������꣩
	//FragColor = texture(outTexture, ourTexCoord) * vec4(ourColor, 1.0f);//ͨ����˽�������ɫ�붥����ɫ���
	FragColor = mix(texture(ourTexture1, ourTexCoord), texture(ourTexture2, vec2(1.0 - ourTexCoord.x, ourTexCoord.y)), 0.2f);//��϶������
	//���������ֵ��0.0�����᷵�ص�һ�����룻�����1.0���᷵�صڶ�������ֵ��0.2�᷵��80%�ĵ�һ��������ɫ��20%�ĵڶ���������ɫ����������������Ļ��ɫ��
}