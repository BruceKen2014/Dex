
#pragma pack_matrix(row_major)

const int POINT_LIGHT	=	5;
const int DIR_LIGHT = 5;
struct stLightPoint
{
	float4 diffuse; //rgb
	float4 position;//xyz
	float4 rangeAtte; // range attenuation
};
struct stLightDirection
{
	float4 color;
	float4 direction;
};
struct stMaterial
{
	float4 diffuse;
	float4 specular;
	float4 ambient;
	float4 emissive; //前三个数据是rgb，自发光时暂时不用a,所以最后一个是强度power
};

float4x4 WVPMarix;
//float4x4 JointsMatrixInvert[32];
float4x4 JointsMatrix[60];

bool     g_bLightEnable;
bool 	 g_bAmbientEnable;
bool     g_bPointLightEnable;
bool     g_bDirectionLightEnable;
bool     g_bFogEnable;

int      g_iLightCountPoint;  //传入的点光源数量
float4   g_ambientColor;
float4   g_fogColor = float4( 0.8f , 0.8f , 0.8f , 1.0f );
float    g_fogEnd = 500.0f; 
stLightPoint g_LightPoints[5];
stLightDirection g_LightDirection;
stMaterial g_material;
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
	float  depth:TEXCOORD3;
};


//像素着色

texture Tex0;
sampler Samp0 =
sampler_state
{
	Texture = <Tex0>;
	ADDRESSU = MIRROR;
	ADDRESSV = MIRROR;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

struct PS_INPUT
{
	float2 uv0:TEXCOORD0;
	float4 worldPos:TEXCOORD1;
	float4 color:COLOR0;
	float3 normal:TEXCOORD2;
	float  depth:TEXCOORD3;
};

struct PS_OUTPUT
{
	float4 color:COLOR0;
};

float3 Light_PointDiffuse(float3 vertexPos, float3 vertexNormal, float3 lightPos, float3 lightColor, float range)
{
	float3 lightDir = lightPos - vertexPos;
	float  distance = length(lightDir);
	lightDir = lightDir/distance;

	float angleCosValue = saturate(dot(lightDir, vertexNormal));
	float constant  = 100.0f/range;// 1/5
	float liner 	= 0.5f / range; // 1/500
	float quadratic = 100.0f/(2 * range * range); //1/250000
	float coefficient = 1 / (constant + liner * distance + quadratic * distance*distance);
	lightColor.x *= g_material.diffuse.r;
	lightColor.y *= g_material.diffuse.g;
	lightColor.z *= g_material.diffuse.b;
	return lightColor * angleCosValue * coefficient;
}
//coefficient表示方向光强度
float3 Light_Direction(float3 vertexPos, float3 vertexNormal, float3 dir, float3 lightColor, float coefficient)
{
	float angleCosValue = saturate(dot(-dir, vertexNormal));
	lightColor.x *= g_material.diffuse.r;
	lightColor.y *= g_material.diffuse.g;
	lightColor.z *= g_material.diffuse.b;
	return lightColor * angleCosValue * coefficient;
}

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
	output.depth = output.position.z/g_fogEnd;
	if(output.depth > 1)
		output.depth = 1;
	return output;
}
PS_OUTPUT PS_MAIN(PS_INPUT input)
{
	PS_OUTPUT  output = (PS_OUTPUT)0;
	//output.color = input.color;//Wireframe
	
	float4 tex_color = tex2D(Samp0, input.uv0);
	int i = 0;
	if(g_bLightEnable)
	{//灯光开启
		input.color.rgba = float4(0.0f, 0.0f, 0.0f, input.color.a);
		//环境光
		if(g_bAmbientEnable)
			input.color.rgb += float3(g_ambientColor.r * g_material.ambient.r, g_ambientColor.g * g_material.ambient.g, g_ambientColor.b * 	g_material.ambient.b);
		if(g_bPointLightEnable)
		{
				//点光源
			while(i < g_iLightCountPoint)
			{
				input.color.rgb += Light_PointDiffuse(input.worldPos.xyz, input.normal, g_LightPoints[i].position.xyz, g_LightPoints[i].diffuse.xyz, g_LightPoints[i].rangeAtte.x);
				i++;
			}
		}
		if(g_bDirectionLightEnable)
		{
			input.color.rgb += Light_Direction(input.worldPos.xyz, input.normal, g_LightDirection.direction.xyz, g_LightDirection.color.xyz,g_LightDirection.color.w);
		}

	}
	output.color.a = input.color.a * tex_color.a; 
	output.color.r = input.color.r * tex_color.r; 
	output.color.g = input.color.g * tex_color.g; 
	output.color.b = input.color.b * tex_color.b; 
	
	//雾效
	if(g_bFogEnable)
	{
		//按照距离平方衰减
		//float fFogFactor = 1 - input.depth * input.depth;  
		//按指数衰减 底数为1/e 如果不乘以一个浓度3的话，在depth为1 也就是远平面时，factor的值为1/e，无法取得0，还是可以看见相当比例的纹理，浓度越大，看见纹理的比例就越低
		//因为这里的depth已经归化到了0-1,所以density是一个整数，而在原来的固定管线中，之所以设定的density是一个很小的小数如0.002，我猜测用于计算的depth是实际的Z值如500，
		//这样一乘结果就是1
		float fFogFactor = exp(-input.depth * 3);
		//线性雾化
		//float fFogFactor = 1 - input.depth;
		output.color =  output.color * fFogFactor + g_fogColor * ( 1 - fFogFactor ); 
	}
	//output.color.r = 1.0f - output.color.r; 
	//output.color.g = 1.0f - output.color.g; 
	//output.color.b = 1.0f - output.color.b; 
	return output;
}

//效果框架

technique T0
{
	pass T0_P0
	{
		CullMode = None;
		FillMode = Solid ;//Wireframe
		AlphaBlendEnable = true;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		AlphaTestEnable = true;
		AlphaRef = 0x01;
		AlphaFunc = Greater;
		vertexShader = compile vs_3_0 VS_MAIN();
		pixelShader = compile ps_3_0 PS_MAIN();
	}
}







