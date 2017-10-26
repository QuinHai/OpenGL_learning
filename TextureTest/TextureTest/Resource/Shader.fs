#version 450 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 ourTexCoord;

uniform sampler2D ourTexture1;//供纹理对象使用的内建数据类型sampler<1D,2D,3D>采样器
uniform sampler2D ourTexture2;

void main()
{
	//FragColor = texture(outTexture, ourTexCoord);//（纹理采样器， 纹理坐标）
	//FragColor = texture(outTexture, ourTexCoord) * vec4(ourColor, 1.0f);//通过相乘将纹理颜色与顶点颜色混合
	FragColor = mix(texture(ourTexture1, ourTexCoord), texture(ourTexture2, vec2(1.0 - ourTexCoord.x, ourTexCoord.y)), 0.2f);//混合多个纹理
	//如果第三个值是0.0，它会返回第一个输入；如果是1.0，会返回第二个输入值。0.2会返回80%的第一个输入颜色和20%的第二个输入颜色，即返回两个纹理的混合色。
}