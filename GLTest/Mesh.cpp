#ifndef MESH_H
#define MESH_H

#include <glm\glm.hpp>
#include <iostream>
#include <string>
#include <vector>

#include <Shader_m.cpp>


using namespace std;

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texture;
};

struct Texture
{
	unsigned int id;
	string mtype;
	string path;
};

class Mesh
{
public:
	const string TEXTURE_DIF = "texture_diffuse";
	const string TEXTURE_SPEC = "texture_specular";
	unsigned int VAO, VBO, EBO;

	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;

		setupMesh();
	}

	void draw(Shader_m* shader) {
		float diffN = 1;
		float specN = 1;
		string tmp = "";
		for (int i = 0; i < textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			if (textures[i].mtype == TEXTURE_DIF) {
				tmp = TEXTURE_DIF + to_string(diffN);
				diffN++;
			}
			else if (textures[i].mtype == TEXTURE_SPEC) {
				tmp = TEXTURE_DIF + to_string(specN);
				specN++;

			}

			//cout << "Mesh::draw::" << tmp.c_str() << endl;

			shader->setFloat(("material." + tmp).c_str(), i);

			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}

		// »æÖÆÍø¸ñ
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE0);
	}
private:
	void setupMesh() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

		glBindVertexArray(0);

	}

};

#endif


