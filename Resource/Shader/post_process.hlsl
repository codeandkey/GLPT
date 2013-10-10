// Post processing shader.

SamplerState linear_sample {
	Filter=ANISOTROPIC;
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
	float4 output_color=(float4) 0;
	//return GLPT_texture.Sample(linear_sample,input.texcoord);

	float4 output_sum=(float4) 0;
	float bloom_distance=2.0f;
	float bloom_factor=1.0f;
	float bloom_interval=0.002f;
	
	for(float i=-bloom_distance;i<=bloom_distance;i+=bloom_factor) {
		for (float y=-bloom_distance;y<=bloom_distance;y+=bloom_factor) {
			float2 index_position=input.texcoord + float2(i,y) * bloom_interval;

			if (index_position.x>1.0f || index_position.x < 0.0f) {
				output_sum += GLPT_texture.Sample(linear_sample,input.texcoord) / bloom_distance;
			} else if (index_position.y>1.0f || index_position.y < 0.0f) {
				output_sum += GLPT_texture.Sample(linear_sample,input.texcoord) / bloom_distance;
			} else {
				output_sum += GLPT_texture.Sample(linear_sample,index_position) / bloom_distance;
			}
		}
	}

	if (GLPT_texture.Sample(linear_sample,input.texcoord).r < 0.5) {
       output_color = output_sum*output_sum*0.012 + GLPT_texture.Sample(linear_sample,input.texcoord);
    } else if (GLPT_texture.Sample(linear_sample,input.texcoord).r < 0.8) {
        output_color = output_sum*output_sum*0.0009 + GLPT_texture.Sample(linear_sample,input.texcoord);
    } else {
        output_color = output_sum*output_sum*0.00075 + GLPT_texture.Sample(linear_sample,input.texcoord);
    }

	return output_color;
}

technique10 GLPT_render {
	pass p0 {
		SetVertexShader(CompileShader(vs_4_0,PostVertexShader()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0,PostPixelShader()));

		SetBlendState(disable_blending,float4(1,1,1,1),0xFFFFFFFF);
	}
}