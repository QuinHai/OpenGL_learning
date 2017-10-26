#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <vector>
#include <string>

#include "Mesh.h"
#include "Shader.h"
using namespace std;

class Model
{
public:
	Model(char *path)
	{
		loadModel(path);
	}

	void Draw(Shader shader)
	{
		for (int i = 0; i < meshes.size(); i++)
			meshes[i].Draw(shader);
	}
private:
	vector<Mesh> meshes;
	string directory;
private:
	void loadModel(string path)
	{
		Assimp::Importer importer;
		const aiScene * scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
		
		//检查场景及其根节点不为null，并检查flag来查看数据是否完整
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
		}
		
		directory = path.substr(0, path.find_last_of('/');

		processNode(scene->mRootNode, scene);
	}

	void processNode(aiNode *node, const aiScene *scene)
	{
	
	}

	Mesh processMesh(aiMesh *mesh, const aiScene *scene)
	{

	}

	vector <Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
	{

	}
	

};
