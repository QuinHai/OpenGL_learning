#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glad/glad.h>

#include <iostream>
#include <vector>
#include <string>

#include "Mesh.h"
#include "Shader.h"
using namespace std;

class Model
{
public:
	vector<Texture> textures_loaded;
	vector<Mesh> meshes;
	string directory;
	bool gammaCorrection;


	Model(string const &path, bool gamma = false) : gammaCorrection(gamma)
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
	void loadModel(string const &path)
	{
		Assimp::Importer importer;
		const aiScene * scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
		
		//��鳡��������ڵ㲻Ϊnull�������flag���鿴�����Ƿ�����
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
		}
		
		//�ҵ�path·�� ���磺resources/model/xxx.xxx  ->��ȡ��resources/model
		directory = path.substr(0, path.find_last_of('/'));//�ַ�����ȡ

		processNode(scene->mRootNode, scene);
	}

	void processNode(aiNode *node, const aiScene *scene)
	{
		for (int i = 0; i < node->mNumMeshes; i++)
		{
			//node�б����Mesh�Ƕ�scene�е�mesh������
			//��ȡ��ǰnode������mesh���ݲ����η���processMesh����
			//����󷵻ص�Mesh�ṹ�崢�浽meshes��vector����
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}

		for (int i = 0; i < node->mNumChildren; i++)
		{
			//������ǰnode�����ӽڵ㣬�����δ���
			processNode(node->mChildren[i], scene);
		}
	}

	Mesh processMesh(aiMesh *mesh, const aiScene *scene)
	{
		vector <Vertex> vertices;
		vector <GLuint> indices;
		vector <Texture> textures;

		for (int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vector;
			//position
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;
			//normal
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
			//texture coordinates
			if (mesh->mTextureCoords[0]) {//�ж��Ƿ��в�������
				glm::vec2 vec;

				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			//tangent
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;
			//bitangent
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;

			//���붥������
			vertices.push_back(vertex);
		}

		for (int i = 0;i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
	}

	vector <Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
	{

	}
	

};
