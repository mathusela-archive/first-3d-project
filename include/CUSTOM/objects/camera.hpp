#ifndef camera_H
#define camera_H

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



// -----------------------------------------------------------------------------------------



class Camera {

	public:

		glm::vec3 location;
		glm::mat4 position;
		glm::mat4 projection;
		glm::vec3 front;

		void init(float fov, int width, int height, float nearClipping, float farClipping) {
			
			projection = glm::perspective(glm::radians(fov), (float)width/height, nearClipping, farClipping);
			set_loc(glm::vec3(0.0, 0.0, 0.0));
			front = glm::vec3(1.0, 0.0, 0.0);
		
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