#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb/stb_image.h>

#include <string>
#include <vector>

#include "Shader.h"
#include "Mesh.h"

unsigned int LoadTextureFromFile(const std::string& fileName, const std::string& dir);

class Model
{
public:
	Model(const char* path);
	void Draw(Shader& shader);
private:
	// model data
	std::vector<Mesh> meshes;
	std::vector<Texture> textures_loaded;
	std::string directory;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	
	Texture getOrLoadTexture(const char* filePath, std::string typeName);
};

#endif