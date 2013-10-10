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
matrix GLPT_transform;

InputPixel LightVertexShader(InputVertex input) {
	InputPixel output;

	output.position=mul(float4(input.position,1.0f),GLPT_transform);
	output.texcoord=input.texcoord;

	return output;
}

float4 LightPixelShader(InputPixel input) : SV_Target {

	float4 input_color=GLPT_texture.Sample(linear_sample,input.texcoord);

	float4 ambient_light=float4(0.0f,0.0f,0.0f,0.2f);
	float4 combine_light=float4(0.0f,0.0f,0.0f,input_color.a);
	float4 output_color=ambient_light+combine_light;

	output_color.a=1-output_color.a;

	return output_color;
}

technique10 GLPT_render {
	pass p0 {
		SetVertexShader(CompileShader(vs_4_0,LightVertexShader()));
		SetPixelShader(CompileShader(ps_4_0,LightPixelShader()));
	}
}