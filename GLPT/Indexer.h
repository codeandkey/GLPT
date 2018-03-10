#ifndef _GLPT_TEXTURE_INDEXER
#define _GLPT_TEXTURE_INDEXER

#include "Library.h"

// Texture indexing device for loading and binding textures.

class Indexer {
public:
	static int LoadTexture(std::string filename);
private:
	static int CreateFromFile(std::string filename);
	static std::map<std::string,int> texture_index;
};

#endif
