#pragma once
#include <chrono>
#include "GLFW/rtre_Window.h"
#include "../engine_rendering/camera.h"
#include "../rtre_base.h" 


namespace rtre {

	typedef uint64_t rTtime;

	rTtime getTime() {
		using std::chrono::microseconds;
		using std::chrono::duration_cast;
		return duration_cast<microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	}

	namespace controller {
		
		static rTtime lastTime,crntTime,deltaTime;
		static const float mSensitivity = 7.0f;
		static bool firstCall = true;
		static const glm::vec2 sensitivityModifier(100, 50);
		glm::vec2 prevCursorPos(0,0);
		glm::vec2 crntCursorPos(0,0);
		glm::vec2 cursorDelta(0, 0);
		void control() {


			crntTime = getTime();
			deltaTime = crntTime - lastTime;

			if (firstCall) {
				firstCall = false;
				eWindow->setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				eWindow->setCursorPosition(viewportWidth / 2, viewportHeight / 2);
				crntCursorPos = glm::vec2(viewportWidth /2, viewportHeight/2);
				prevCursorPos = glm::vec2(viewportWidth / 2, viewportHeight / 2);
			}
			else {
				auto crntCursorPos_PlaceHolder = eWindow->getCursorPosition();
				crntCursorPos = glm::vec2(crntCursorPos_PlaceHolder.x, crntCursorPos_PlaceHolder.y);


				cursorDelta = -mSensitivity * ((crntCursorPos - prevCursorPos) / sensitivityModifier);

				camera.setSpeed( glm::vec3(deltaTime)*camera.speed());

				// Guards so camera doesn't get cuckery
				if ((camera.orientation().y <= -0.99f) && cursorDelta.y < 0) {
					cursorDelta.y = 0;
					camera.setOrientation(glm::vec3(camera.orientation().x, -0.99f, camera.orientation().z));
				}
				else if ((camera.orientation().y >= 0.99f) && cursorDelta.y > 0) {
					cursorDelta.y = 0;
					camera.setOrientation(glm::vec3(camera.orientation().x, 0.99f, camera.orientation().z));
				}

				
				if (eWindow->isKeyPressed(GLFW_KEY_SPACE)) {
					camera.moveUp();
				}
				else if (eWindow->isKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
					camera.moveDown();
				}
				if (eWindow->isKeyPressed(GLFW_KEY_W)) {
					camera.moveForward();
				}
				else if (eWindow->isKeyPressed(GLFW_KEY_S)) {
					camera.moveBackward();
				}
				if (eWindow->isKeyPressed(GLFW_KEY_A)) {
					camera.moveLeft();
				}
				else if (eWindow->isKeyPressed(GLFW_KEY_D)) {
					camera.moveRight();
				}

				//	Rotate World around the vertical axis
				camera.setOrientation(glm::rotate(camera.orientation(), glm::radians(GLfloat(cursorDelta.x)), camera.upDirection()));
				//	Rotate World around the horizontal axis
				camera.setOrientation(glm::normalize(camera.orientation() + glm::vec3(0.0f, glm::radians(cursorDelta.y), 0.0f)));

				prevCursorPos = crntCursorPos;
			}


			lastTime = crntTime;

		}

	}
}