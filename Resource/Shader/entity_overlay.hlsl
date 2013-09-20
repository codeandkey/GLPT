// Shader for rendering overlays.

BlendState normal_blending {
	BlendEnable[0]=TRUE;
	SrcBlend=SRC_ALPHA;
	Destblend=INV_SRC_ALPHA;
};

SamplerState linear_sample {
	Filter=ANISOTROPIC;
	AddressU=Wrap;
	AddressV=Clamp;
};

float4x4 GLPT_transform;
Texture2D GLPT_texture;
bool GLPT_flip_horizontal;

float over_r,over_g,over_b,over_a;

struct InputVertex {
	float3 position : POSITION;
	float2 texcoord : TEXCOORD;
};

struct InputPixel {
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
};

InputPixel PostVertexShader(InputVertex input) {
	InputPixel output;

	output.position = mul(float4(input.position,1.0),GLPT_transform);
	output.texcoord=input.texcoord;

	return output;
}

float4 PostPixelShader(InputPixel input) : SV_Target {
	return float4(over_r,over_g,over_b,over_a); 
}

technique10 GLPT_render {
	pass p0 {
		SetVertexShader(CompileShader(vs_4_0,PostVertexShader()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0,PostPixelShader()));

		SetBlendState(normal_blending,float4(1,1,1,1),0xFFFFFFFF);
	}
}