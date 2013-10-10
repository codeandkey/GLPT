// GLPT radiosity shader.

struct InputVertex {
	float3 position : POSITION;
	float2 texcoord : TEXCOORD;
};

struct InputPixel {
	float4 position : SV_Position;
	float2 texcoord : TEXCOORD;
};

SamplerState linear_sample {
	Filter=ANISOTROPIC;
	AddressU=Wrap;
	AddressV=Wrap;
};

Texture2D GLPT_texture;
float4x4 GLPT_transform;

InputPixel LightVertexShader(InputVertex input) {
	InputPixel output;

	output.position=mul(float4(input.position,1.0f),GLPT_transform);
	output.texcoord=input.texcoord;

	return output;
}

float4 LightPixelShader(InputPixel input) : SV_Target {
	//return float4(0,1,0,1);
	return GLPT_texture.Sample(linear_sample,input.texcoord);
}

technique10 GLPT_render {
	pass p0 {
		SetVertexShader(CompileShader(vs_4_0,LightVertexShader()));
		SetPixelShader(CompileShader(ps_4_0,LightPixelShader()));
	}
}