#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setupMesh();
}

void Mesh::Draw(Shader& shader)
{
	// bind textures
	unsigned int nDiffuse = 1;
	unsigned int nSpecular = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		// retrieve texture number
		// this method requires shaders to use a specific naming convention for sampler2D uniforms: "texture_[diffuse/specular][N]"
		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(nDiffuse++);
		else if (name == "texture_specular")
			number = std::to_string(nSpecular++);

		shader.SetInt("material." + name + number, i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	glActiveTexture(0);

	//draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

// set up array/buffer objects for rendering
void Mesh::setupMesh()
{
	// create array/buffer objects
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// set array/buffers active
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	// pass vertex data into vertex buffer & index data to element buffer
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0); // layout location 0 in shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); // position is a vector3 of floats (not normalized), stride is the size of one Vertex object, and starts at index 0
	// vertex normals
	glEnableVertexAttribArray(1); // layout location 1 in shader
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)); // normal is a vector3 of floats (not normalized)
	// vertex texture coordinates
	glEnableVertexAttribArray(2); // layout location 2 in shader
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords)); // texcoords is a vector2 of floats (not normalized)

	// set VAO inactive to prevent accidentally writing changes to it
	glBindVertexArray(0);
}
