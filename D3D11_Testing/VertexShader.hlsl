struct input
{
	float3 position	:	POSITION;
	float3 color	:	COLOR;
};

struct output
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

output main( input IN ) 
{
	output OUT;
	OUT.position = float4(IN.position, 1.0f);
	OUT.color = float4(IN.position, 1.0f);

	return OUT;
}