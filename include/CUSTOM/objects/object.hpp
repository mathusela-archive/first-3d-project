#ifndef object_H
#define object_H

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <custom/objects/camera.hpp>



// -----------------------------------------------------------------------------------------



class Object {

	std::vector<float> mesh;
	unsigned int VAO;
	unsigned int shaderProgram;
	unsigned int shadowShaderProgram;

	glm::vec3 location;
	glm::mat4 position;

	Material material;

	void init() {

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		unsigned int VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, mesh.size()*sizeof(float), &mesh[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)(sizeof(float)*3));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)(sizeof(float)*6));
		glEnableVertexAttribArray(2);

	}

	public:

		Object(std::vector<float> setMesh, unsigned int setShaderProgram, unsigned int setShadowShaderProgram, Material setMaterial) {
		
			mesh = setMesh;
			shaderProgram = setShaderProgram;
			shadowShaderProgram = setShadowShaderProgram;
			material = setMaterial;
			init();
		
		}

		void draw(Camera camera, std::vector<LightVals> lightVector) {

			glUseProgram(shaderProgram);
			glBindVertexArray(VAO);

			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "world"), 1, GL_FALSE, glm::value_ptr(position));
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(camera.position));
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(camera.projection));
			
			glUniform3fv(glGetUniformLocation(shaderProgram, "cameraPos"), 1, glm::value_ptr(camera.location));
			
			for (int i=0; i < lightVector.size(); i++) {
				std::string posName = "lightsVector[" + std::to_string(i) + "].pos";
				std::string colorName = "lightsVector[" + std::to_string(i) + "].color";
				std::string shadowName = "lightsVector[" + std::to_string(i) + "].shadowMap";
				glUniform3fv(glGetUniformLocation(shaderProgram, posName.c_str()), 1, glm::value_ptr(lightVector[i].pos));
				glUniform3fv(glGetUniformLocation(shaderProgram, colorName.c_str()), 1, glm::value_ptr(lightVector[i].color));
				glUniform1i(glGetUniformLocation(shaderProgram, shadowName.c_str()), 0);
				glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_CUBE_MAP, lightVector[i].shadowMap);
			}
			glUniform1i(glGetUniformLocation(shaderProgram, "lightsCount"), lightVector.size());

			glUniform1i(glGetUniformLocation(shaderProgram, "mat.diffusionMap"), 1);
			glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, material.maps.diffusionMap);
			glUniform1i(glGetUniformLocation(shaderProgram, "mat.specularMap"), 2);
			glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, material.maps.specularMap);
			glUniform1i(glGetUniformLocation(shaderProgram, "mat.normalMap"), 3);
			glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, material.maps.normalMap);

			glUniform1f(glGetUniformLocation(shaderProgram, "mat.inverseRoughness"), material.inverseRoughness);

			glDrawArrays(GL_TRIANGLES, 0, mesh.size());

		}

		void draw_shadow(Camera camera, glm::vec3 lightPos) {

			glUseProgram(shadowShaderProgram);
			glBindVertexArray(VAO);

			glUniformMatrix4fv(glGetUniformLocation(shadowShaderProgram, "world"), 1, GL_FALSE, glm::value_ptr(position));
			glUniformMatrix4fv(glGetUniformLocation(shadowShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(camera.position));
			glUniformMatrix4fv(glGetUniformLocation(shadowShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(camera.projection));

			glUniform3fv(glGetUniformLocation(shadowShaderProgram, "lightPos"), 1, glm::value_ptr(lightPos));

			glDrawArrays(GL_TRIANGLES, 0, mesh.size());

		}

		void set_loc(glm::vec3 setLoc) {

			location = setLoc;
			position = glm::mat4(1.0f);
			position = glm::translate(position, location);

		}

		void move_loc(glm::vec3 moveLoc) {

			location = location + moveLoc;
			set_loc(location);

		}

};



// -----------------------------------------------------------------------------------------



#endif