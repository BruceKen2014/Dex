
//存储颜色混合的比例值s，其中
//Scalar.x = s,	Scalar.y = 1-s
vector Scalar;

//纹理
texture Tex0;
texture Tex1;

//纹理采样器
sampler Samp0 =
sampler_state
{
	//关联纹理
	Texture = <Tex0>;
	//采样器状态
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
//输入两套纹理坐标
struct PS_INPUT
{
	float4 color:COLOR0;
	float2 uvCoords0: TEXCOORD0;
	float2 uvCoords1: TEXCOORD1;
};
//输出像素颜色
struct PS_OUTPUT
{
	float4	Color:COLOR0;
};

//入口函数
PS_OUTPUT	PS_Main(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;
	//分别对两个纹理进行采样按照比例混合后输出颜色值
	//tex2D(Samp0, input.uvCoords0)表示从Samp0的纹理中取得uv处的颜色值
	output.Color = tex2D(Samp0, input.uvCoords0)* Scalar.x + tex2D(Samp1, input.uvCoords1) * Scalar.y;
	output.Color.yz = 0;
	output.Color.w = 0.1f;// input.color.a;
	return output;
}
