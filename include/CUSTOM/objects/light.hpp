#ifndef light_H
#define light_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <custom/objects/object.hpp>
#include <custom/objects/camera.hpp>



// -----------------------------------------------------------------------------------------



class Light {

	public:

		glm::vec3 pos;
		glm::vec3 color; 

		Camera shadowCamera;

		unsigned int framebuffers[6];
		unsigned int textures[6];

		unsigned int depthCubemap;

		Light(glm::vec3 setPos, glm::vec3 setColor) {
			
			pos = setPos;
			color = setColor;

			shadowCamera.init(90.0, 1024, 1024, 0.1, 100.0);
			// shadowCamera.set_loc(glm::vec3(pos.x*-1, pos.y*-1, pos.z*-1));
			shadowCamera.set_loc(pos);

			shadowInit();
		
		}

		void shadowInit() {

			glGenTextures(1, &depthCubemap);

			const unsigned int SHADOW_HEIGHT = 1024, SHADOW_WIDTH = 1024;
			glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
			
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			
			for (int i=0; i<6; i++) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
			}

			// glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
			// glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

			for (int i=0; i<6; i++) {
				glGenFramebuffers(1, &framebuffers[i]);
				glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[i]);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, depthCubemap, 0);
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}

		}

		void renderShadowPass(std::vector<Object> objectVector) {

			for(int i=0; i<6; i++) {

				glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[i]);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				switch (i) {
					case 0: 
						shadowCamera.front = glm::vec3(-1.0, 0.0, 0.0);
						shadowCamera.position = glm::lookAt(shadowCamera.location, shadowCamera.location + shadowCamera.front, glm::vec3(0.0, 1.0, 0.0));
						break;
					case 1:
						shadowCamera.front = glm::vec3(1.0, 0.0, 0.0);
						shadowCamera.position = glm::lookAt(shadowCamera.location, shadowCamera.location + shadowCamera.front, glm::vec3(0.0, 1.0, 0.0));
						break;
					case 2:
						shadowCamera.front = glm::vec3(0.0, -1.0, 0.01);
						shadowCamera.position = glm::lookAt(shadowCamera.location, shadowCamera.location + shadowCamera.front, glm::vec3(0.0, 1.0, 0.0));
						break;
					case 3:
						shadowCamera.front = glm::vec3(0.0, 1.0, 0.01);
						shadowCamera.position = glm::lookAt(shadowCamera.location, shadowCamera.location + shadowCamera.front, glm::vec3(0.0, 1.0, 0.0));
						break;
					case 4:
						shadowCamera.front = glm::vec3(0.0, 0.0, -1.0);
						shadowCamera.position = glm::lookAt(shadowCamera.location, shadowCamera.location + shadowCamera.front, glm::vec3(0.0, 1.0, 0.0));
						break;
					case 5:
						shadowCamera.front = glm::vec3(0.0, 0.0, 1.0);
						shadowCamera.position = glm::lookAt(shadowCamera.location, shadowCamera.location + shadowCamera.front, glm::vec3(0.0, 1.0, 0.0));
						break;
				}
				
				for (int j=0; j<objectVector.size(); j++) {
					objectVector[j].draw_shadow(shadowCamera, shadowCamera.location);
				}

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			
			}

		}

};



// -----------------------------------------------------------------------------------------



#endif