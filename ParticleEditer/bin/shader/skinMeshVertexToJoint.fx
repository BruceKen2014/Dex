

float4x4 WVPMarix;
//float4x4 JointsMatrixInvert[32];
float4x4 JointsMatrix[60];
//顶点着色
struct VS_INPUT
{
	float4 position:POSITION;
	float3 normal:NORMAL;
	float4 color:COLOR0;
	float2 uv0:TEXCOORD0;
	int4   indice:BLENDINDICES;
	float4   weight:BLENDWEIGHT;
};

struct VS_OUTPUT
{
	float4 position:POSITION; //乘以WVPMarix后的坐标
	float2 uv0:TEXCOORD0;    
	float4 worldPos:TEXCOORD1;//乘以WVPMarix前的坐标
	float4 color:COLOR0;
	float3 normal:TEXCOORD2;//顶点的实时法线
};

struct PS_INPUT
{
	float2 uv0:TEXCOORD0;
	float4 worldPos:TEXCOORD1;
	float4 color:COLOR0;
	float3 normal:TEXCOORD2;
};

struct PS_OUTPUT
{
	float4 color:COLOR0;
};

VS_OUTPUT VS_MAIN(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	int   indice[4] = {input.indice.x, input.indice.y, input.indice.z, input.indice.w};
	float weight[4] = {input.weight.x, input.weight.y, input.weight.z, input.weight.w};
	float4 normalPos = input.position + float4(input.normal, 0.0f);
	output.worldPos = mul(input.position, JointsMatrix[indice[0]]) * weight[0] + 
					  mul(input.position, JointsMatrix[indice[1]]) * weight[1] + 
					  mul(input.position, JointsMatrix[indice[2]]) * weight[2] + 
					  mul(input.position, JointsMatrix[indice[3]]) * weight[3];
	normalPos = mul(normalPos, JointsMatrix[indice[0]]) * weight[0] + 
					 mul(normalPos, JointsMatrix[indice[1]]) * weight[1] + 
					 mul(normalPos, JointsMatrix[indice[2]]) * weight[2] + 
					 mul(normalPos, JointsMatrix[indice[3]]) * weight[3];
	normalPos = normalPos - output.worldPos;
	output.normal = normalPos.xyz;
	normalize(output.normal);
	output.position = mul(output.worldPos, WVPMarix);
	output.uv0 = input.uv0;
	output.color = input.color;
	return output;
}
PS_OUTPUT PS_MAIN(PS_INPUT input)
{
	PS_OUTPUT  output = (PS_OUTPUT)0;
	//output.color = input.color;//Wireframe
	
	output.color = input.color;

	return output;
}

//效果框架

technique T0
{
	pass T0_P0
	{
		CullMode = None;
		FillMode = Solid ;//Wireframe
		vertexShader = compile vs_3_0 VS_MAIN();
		pixelShader = compile ps_3_0 PS_MAIN();
	}
}







