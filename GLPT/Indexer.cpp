#include "Library.h"
#include "Indexer.h"
#include "Graphics.h"

std::map<std::string,ID3D10ShaderResourceView*> Indexer::texture_index;

ID3D10ShaderResourceView* Indexer::LoadTexture(std::string filename) {

	filename="Resource/Texture/" + filename + ".dds";

	try {
		Indexer::texture_index.at(filename);
	} catch(std::out_of_range) {
		return Indexer::CreateFromFile(filename);
	}

	return Indexer::texture_index.at(filename);

}

ID3D10ShaderResourceView* Indexer::CreateFromFile(std::string filename) {

	ID3D10ShaderResourceView* output;

	HRESULT hr=D3DX10CreateShaderResourceViewFromFile(GLPT_graphics->GetGraphicsDevice(),filename.c_str(),NULL,NULL,&output,NULL);

	if (FAILED(hr)) {
		GLPT_logger.Print("[GLPT_indexer] Failed to load texture " + filename);
		output=NULL;
	}

	return output;

}