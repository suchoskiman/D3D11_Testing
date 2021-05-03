struct input
{
	float4	position	:	POSITION;
	float4	color		:	COLOR;
};

float4 main(input IN) : SV_TARGET
{
	return IN.color;
}