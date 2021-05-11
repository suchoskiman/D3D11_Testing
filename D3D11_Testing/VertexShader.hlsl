cbuffer Projection : register(b0)
{
	matrix proj;
}

cbuffer View : register(b1)
{
	matrix view;
}

cbuffer Model : register(b2)
{
	matrix model;
}

struct input
{
	float3 position	: POSITION;
	float3 color : COLOR;
};

struct output
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

output main( input IN )
{
	matrix mvp = mul(proj, mul(view, model));
	output OUT;
	OUT.position = mul(mvp, float4(IN.position, 1.0f));
	OUT.color = float4(IN.color, 1.0f);

	return OUT;
}