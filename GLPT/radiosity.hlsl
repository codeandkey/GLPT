// GLPT radiosity shader.

struct InputVertex {
	float3 position : POSITION;
	float2 texcoord;
};

struct InputPixel {
	float4 position : SV_Position;
	float2 texcoord;
};