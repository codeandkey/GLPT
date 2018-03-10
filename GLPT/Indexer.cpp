#include "Library.h"
#include "Indexer.h"
#include "Graphics.h"

#include "stb_image.h"

std::map<std::string,int> Indexer::texture_index;

int Indexer::LoadTexture(std::string filename) {
	filename="Resource/Texture/" + filename + ".png";

	try {
		Indexer::texture_index.at(filename);
	} catch(std::out_of_range) {
		texture_index[filename]=Indexer::CreateFromFile(filename);
	}
	return Indexer::texture_index.at(filename);

}

int Indexer::CreateFromFile(std::string filename) {
	unsigned int output;

	int w, h;
	unsigned char* stbd = stbi_load(filename.c_str(), &w, &h, NULL, 4);

	if (!stbd) {
		printf("Failed to read texture file %s\n", filename.c_str());
		return 0;
	}

	unsigned char* id = (unsigned char*) malloc(sizeof *id * w * h * 4);
	unsigned rw = 4 * w;

	for (int i = 0; i < h; ++i) {
		memcpy(id + i * rw, stbd + (h - 1) * rw - i * rw, w * 4 * sizeof *id);
	}

	glGenTextures(1, &output);
	glBindTexture(GL_TEXTURE_2D, output);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, id);

	free(id);
	stbi_image_free(stbd);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	return output;

}
