#include <vector>
#include <string>
#include <assimp\scene.h>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <Shader_m.cpp>
#include <stb_image.h>

#include <Mesh.cpp>

using namespace std;

class Model
{
public:
	vector<Mesh> meshes;
	vector<Texture> textureLoaded;
	string directory;

	Model(string path) {
		loadModel(path);
	}

	void draw(Shader_m* shader) {
		for (int i = 0; i < meshes.size(); i++) {
			meshes[i].draw(shader);
		}
	}

private:
	void loadModel(string path) {
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate || aiProcess_FlipUVs);
		if (!scene || scene->mFlags &&AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
			
			return;
		}
		directory = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene);
	}

	void processNode(aiNode* node, const aiScene* scene ) {
		for (int i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}

		for (int i = 0; i < node->mNumChildren; i++) {
			processNode(node->mChildren[i], scene);
		}

	}

	Mesh processMesh(aiMesh* mesh, const aiScene* scene) {
		vector<Vertex> vertices;

		for (int i = 0; i < mesh->mNumVertices; i++) {
			Vertex vertex;

			glm::vec3 pos = glm::vec3(0);
			pos.x = mesh->mVertices[i].x;
			pos.y = mesh->mVertices[i].y;
			pos.z = mesh->mVertices[i].z;

			vertex.position = pos;

			glm::vec3 nor = glm::vec3(0);
			nor.x = mesh->mNormals[i].x;
			nor.y = mesh->mNormals[i].y;
			nor.z = mesh->mNormals[i].z;

			vertex.normal = nor;

			glm::vec2 ttu = glm::vec2(0.0f);
			if (mesh->mTextureCoords[0]) {
				ttu.x = mesh->mTextureCoords[0][i].x;
				ttu.y = mesh->mTextureCoords[0][i].y;
			}
			vertex.texture = ttu;

			vertices.push_back(vertex);
		}


		vector<unsigned int> indexs;
		for (int i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (int j = 0; j < face.mNumIndices; j++) {
				indexs.push_back(face.mIndices[j]);
			}
		}

		vector<Texture> textures;
		if (mesh->mMaterialIndex >= 0) {
			aiMaterial* atu = scene->mMaterials[mesh->mMaterialIndex];
			vector<Texture> diffuseMaps = loadMaterTextures(atu, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

			vector<Texture> specMaps = loadMaterTextures(atu, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specMaps.begin(), specMaps.end());
		}


		return Mesh(vertices, indexs, textures);
	}

	vector<Texture> loadMaterTextures(aiMaterial* material, aiTextureType type, string typeName) {
		vector<Texture> textures;

		for (int i = 0; i < material->GetTextureCount(type); i++) {
			aiString str;
			material->GetTexture(type, i, &str);

			bool skip = false;
			for (int a = 0; a < textureLoaded.size(); a++) {
				if (strcmp(textureLoaded[a].path.c_str(), str.C_Str()) == 0) {
					textures.push_back(textureLoaded[a]);
				
					skip = true;
					break;
				}
			}

			if (!skip) {
				Texture texture;
				texture.id = TextureFromFile(str.C_Str(), directory);
				texture.mtype = type;
				texture.path = str.C_Str();

				textures.push_back(texture);
				textureLoaded.push_back(texture);			
			}
		}
	
		return textures;
	}

	unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false)
	{
		string filename = string(path);
		filename = directory + '/' + filename;

		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
		}

		return textureID;
	}

};


