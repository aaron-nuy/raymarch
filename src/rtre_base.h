#pragma once
#include <memory>
#include <iostream>
#include <algorithm>
#include "engine_abstractions/shader.h"
#include "engine_abstractions/sampler.h"
#include "engine_abstractions/dtypes.h"
#include "engine_rendering/camera.h"
#include "GLFW/rtre_Window.h"

namespace rtre {
	
	static Camera camera;
	static GLuint viewportWidth;
	static GLuint viewportHeight;
	static Window* eWindow;
	static GLfloat aspectRatio = 1;


	void setViewport(GLuint vWidth, GLuint vHeight) {
		viewportWidth = vWidth;
		viewportHeight = vHeight;
		aspectRatio = viewportWidth / (GLfloat)viewportHeight;

		camera.setAspectRatio(aspectRatio);

		glViewport(0, 0, viewportWidth, viewportHeight);
	}

	/*
	Initilize glad
	Must be called after setting window context
	*/
	void init(GLuint viewportWidth,GLuint viewportHeight, Window& window,
			const glm::vec3& pos = glm::vec3(0,0,0),GLfloat aspectRatio = 1.0f, 
			GLfloat fov = 75.0f, GLfloat zNear = 0.05f, GLfloat zFar = 500.0f) {


		if (!gladLoadGL()) {
			throw std::exception("Could not load glad.\n");
		}

		setViewport(viewportWidth, viewportHeight);


		camera = Camera(pos, aspectRatio, fov, zNear, zFar);

		eWindow = &window;
		
	}



	void enable(int glflags) {
		glEnable(glflags);
	}
	void cullFace(int glflag) {
		glCullFace(glflag);
	}
	void setBackgroundColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 0.0) {
		glClearColor(r, g, b, a);
	}
	void setBackgroundColor(vec3 vec, GLfloat a = 0.0) {
		glClearColor(vec.x, vec.y, vec.z, a);
	}
	void setBackgroundColor(vec4 vec) {
		glClearColor(vec.x, vec.y, vec.z, vec.y);
	}
	void clearBuffers(int glflags) {
		glClear(glflags);
	}
	/*
	Set which faces to be considered the front ones
	Decision is made based on the indices ordering/direction
	Arguments are either, GL_CCW or GL_CW (clockwise/counter clockwise)
	*/
	void setFrontFace(int glflag) {
		glFrontFace(glflag);
	}



}