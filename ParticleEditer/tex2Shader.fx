
//�洢��ɫ��ϵı���ֵs������
//Scalar.x = s,	Scalar.y = 1-s
vector Scalar;

//����
texture Tex0;
texture Tex1;

//���������
sampler Samp0 =
sampler_state
{
	//��������
	Texture = <Tex0>;
	//������״̬
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};
sampler Samp1 =
sampler_state
{
	Texture = <Tex1>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};
//����������������
struct PS_INPUT
{
	float4 color:COLOR0;
	float2 uvCoords0: TEXCOORD0;
	float2 uvCoords1: TEXCOORD1;
};
//���������ɫ
struct PS_OUTPUT
{
	float4	Color:COLOR0;
};

//��ں���
PS_OUTPUT	PS_Main(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;
	//�ֱ������������в������ձ�����Ϻ������ɫֵ
	//tex2D(Samp0, input.uvCoords0)��ʾ��Samp0��������ȡ��uv������ɫֵ
	output.Color = tex2D(Samp0, input.uvCoords0)* Scalar.x + tex2D(Samp1, input.uvCoords1) * Scalar.y;
	output.Color.yz = 0;
	output.Color.w = 0.1f;// input.color.a;
	return output;
}
