#ifndef _GLPT_TEXTURE_INDEXER
#define _GLPT_TEXTURE_INDEXER

#include "Library.h"

// Texture indexing device for loading and binding textures.

class Indexer {
public:
	static ID3D10ShaderResourceView* LoadTexture(std::string filename);
private:
	static ID3D10ShaderResourceView* CreateFromFile(std::string filename);
	static std::map<std::string,ID3D10ShaderResourceView*> texture_index;
};

#endif