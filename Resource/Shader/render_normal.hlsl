// Normal rendering shader.

BlendState normal_blending {
	BlendEnable[0]=TRUE;
	SrcBlend=SRC_ALPHA;
	Destblend=INV_SRC_ALPHA;
};

SamplerState linear_sample {
	Filter=ANISOTROPIC;
	AddressU=Wrap;
	AddressV=Wrap;
};

float4x4 GLPT_transform;
Texture2D GLPT_texture;
bool GLPT_flip_horizontal;

struct InputVertex {
	float3 position : POSITION;
	float2 texcoord : TEXCOORD;
};

struct InputPixel {
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float width : WIDTH;
};

InputPixel PostVertexShader(InputVertex input) {
	InputPixel output;

	output.width=abs(input.position.x);

	output.position = mul(float4(input.position,1.0),GLPT_transform);
	output.texcoord=input.texcoord;

	return output;
}

float4 PostPixelShader(InputPixel input) : SV_Target {
	if (GLPT_flip_horizontal) {
		input.texcoord.x*=-1.0;
	}

	return GLPT_texture.Sample(linear_sample,input.texcoord);
}

technique10 GLPT_render {
	pass p0 {
		SetVertexShader(CompileShader(vs_4_0,PostVertexShader()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0,PostPixelShader()));

		SetBlendState(normal_blending,float4(1,1,1,1),0xFFFFFFFF);
	}
}