// Post processing shader.

SamplerState linear_sample {
	Filter=MIN_MAG_MIP_LINEAR;
	AddressU=Wrap;
	AddressV=Wrap;
};

BlendState disable_blending {
	BlendEnable[0]=FALSE;
	SrcBlend=SRC_ALPHA;
	Destblend=INV_SRC_ALPHA;
};

float4x4 GLPT_transform;
Texture2D GLPT_texture;


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
	return GLPT_texture.Sample(linear_sample,input.texcoord);
}

technique10 GLPT_render {
	pass p0 {
		SetVertexShader(CompileShader(vs_4_0,PostVertexShader()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0,PostPixelShader()));

		SetBlendState(disable_blending,float4(1,1,1,1),0xFFFFFFFF);
	}
}