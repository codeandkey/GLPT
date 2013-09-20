#include "Library.h"
#include "Shader.h"
#include "Graphics.h"

Shader* Shader::current_shader=NULL;

EResult Shader::Initialize(std::string filename,Layout* input_layout,unsigned int element_count) {
	EResult output;
	ID3D10Blob* dx_error;
	HRESULT hr;
	D3D10_INPUT_ELEMENT_DESC *dx_input_layout_desc;
	D3D10_PASS_DESC dx_pass_desc;

	shader_name=filename;

	filename="Resource/Shader/"+filename;

	hr=D3DX10CreateEffectFromFile(filename.c_str(),NULL,NULL,"fx_4_0",D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_DEBUG,0,GLPT_graphics->GetGraphicsDevice(),NULL,NULL,&dx_effect,&dx_error,NULL);

	if (FAILED(hr)) {
		if (dx_error) {
			output.Error(std::string("Shader compilation error : ") + ((char*)  dx_error->GetBufferPointer()));
			GLPT_logger.Print(output.ErrorMessage());
		} else {
			output.Error("Failed to locate shader file.");
			GLPT_logger.Print(output.ErrorMessage());
		}
		return output;
	}

	dx_input_layout_desc=new D3D10_INPUT_ELEMENT_DESC[element_count];

	ZeroMemory(dx_input_layout_desc,sizeof(D3D10_INPUT_ELEMENT_DESC)*element_count);

	for(unsigned int i=0;i<element_count;i++) {
		dx_input_layout_desc[i].AlignedByteOffset=(i) ? D3D10_APPEND_ALIGNED_ELEMENT : 0;
		dx_input_layout_desc[i].Format=input_layout[i].type;
		dx_input_layout_desc[i].SemanticName=input_layout[i].name.c_str();
		dx_input_layout_desc[i].InputSlotClass=D3D10_INPUT_PER_VERTEX_DATA;
	}

	dx_effect->GetTechniqueByName("GLPT_render")->GetPassByIndex(0)->GetDesc(&dx_pass_desc);

	hr=GLPT_graphics->GetGraphicsDevice()->CreateInputLayout(dx_input_layout_desc,element_count,dx_pass_desc.pIAInputSignature,dx_pass_desc.IAInputSignatureSize,&dx_input_layout);

	if (FAILED(hr)) {
		output.Error("Failed to create shader input layout.");
		return output;
	}

	return output;

}

void Shader::Bind(void) {
	D3D10_TECHNIQUE_DESC dx_shader_desc;

	GLPT_graphics->GetGraphicsDevice()->IASetInputLayout(dx_input_layout);

	dx_effect->GetTechniqueByName("GLPT_render")->GetDesc(&dx_shader_desc);

	for(unsigned int i=0;i<dx_shader_desc.Passes;i++) {
		dx_effect->GetTechniqueByName("GLPT_render")->GetPassByIndex(i)->Apply(0);
	}
}

void Shader::Update(void) {
	dx_effect->GetVariableByName("GLPT_random_number")->AsScalar()->SetFloat((float) rand() / (float)  RAND_MAX);
}

void Shader::Release(void) {
	dx_input_layout->Release();
	dx_effect->Release();
}

void Shader::Texture(ID3D10ShaderResourceView* texture) {
	dx_effect->GetVariableByName("GLPT_texture")->AsShaderResource()->SetResource(texture);
}

void Shader::Transform(D3DXMATRIX transform) {
	dx_effect->GetVariableByName("GLPT_transform")->AsMatrix()->SetMatrix(transform);
}

void Shader::Blur(float blur) {
	dx_effect->GetVariableByName("GLPT_blur_intensity")->AsScalar()->SetFloat(blur);
}

void Shader::FlipH(bool flip) {
	dx_effect->GetVariableByName("GLPT_flip_horizontal")->AsScalar()->SetBool(flip);
}

void Shader::SetVariant(std::string name,float value) {
	dx_effect->GetVariableByName(name.c_str())->AsScalar()->SetFloat(value);
}