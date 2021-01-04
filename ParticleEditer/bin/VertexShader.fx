

matrix	WVPMatrix;
vector	color;

struct DEX_VS_INPUT
{
	vector position: POSITION;
}; 

struct DEX_VS_OUTPUT
{
	vector position: POSITION;
	vector color:COLOR;
};

DEX_VS_OUTPUT SetColor(DEX_VS_INPUT input)
{
	DEX_VS_OUTPUT output = (DEX_VS_OUTPUT)0;
	output.position = mul(input.position, WVPMatrix);
	output.color = color;
	return output;
}
