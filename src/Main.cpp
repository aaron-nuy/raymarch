#pragma once
#include <vector>
#include <filesystem>
#include <Windows.h>
#include "rtre.h"
#include "GLFW/rtre_Window.h"
#include "engine_movement/controller.h"

#define LOG(x) std::cout << x << "\n"

void prv(glm::vec3 v) {
	std::cout << v.x << " " << v.y << " " << v.z << "\n";
}

float getTime() {
	using std::chrono::milliseconds;
	using std::chrono::duration_cast;
	return (float)duration_cast<milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}


glm::mat4 matrix(const rtre::Camera& c) {
	auto view = glm::lookAt(c.position(), c.position() + c.orientation(), glm::vec3(0, 1, 0));
	return glm::inverse(view);
	
}


namespace fs = std::filesystem;

int main()
{

	std::string path = ".";
	for (const auto& entry : fs::directory_iterator(path))
		if (entry.path().generic_string().find(".vs") == std::string::npos) {
			FreeConsole();
			break;
		}

	rtre::Window::init();
	rtre::Window::initHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	rtre::Window::initHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	rtre::Window::initHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	rtre::Window window(800, 480,"Engine");
	window.makeContextCurrent();
	rtre::init(1000, 1000, window);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 430");

	bool show_another_window = true;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	std::shared_ptr<rtre::RenderShader> shader = std::make_shared<rtre::RenderShader>(".\\src\\engine_resources\\vert.vert", ".\\src\\engine_resources\\frag.frag", "");
	rtre::Quad screen = rtre::Quad(shader);
	GLfloat fov = 75.f;


	float stime = getTime();
	float time = 0;

	bool cursorVisisble = true;
	glm::vec3 sphereloc = glm::vec3(0,0,3);
	GLfloat sphereRadius = 0.5f;
	GLint maxits = 500;
	GLfloat thresh = 0.001;
	float speed = 1;
	while (!window.shouldClose() && !window.isKeyPressed(GLFW_KEY_ESCAPE)) {

		screen.m_Shader->checkAndHotplug();
		rtre::Window::pollEvents();

		int display_w, display_h;


		glfwGetFramebufferSize(window.getWindow(), &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();




		if (show_another_window)
		{
			ImGui::Begin("Control Panel", &show_another_window);

			ImGui::SliderFloat3("Sphere Position", (float*)&sphereloc, -1, 3);
			ImGui::SliderFloat("Sphere Radius", &sphereRadius, 0, 2);
			ImGui::DragInt("Max Iterations", &maxits, 1.f, 1, 5000);
			ImGui::SliderFloat("Threshold", &thresh, .00001f, 0.01f,"%.5f");

			ImGui::SliderFloat("Speed", (float*)&speed, 1, 100, "%.1f");

			rtre::camera.setSpeed(glm::vec3(speed/1000000));

			ImGui::End();
		}

		GLfloat aspectRatio = aspectRatio = float(display_w) / display_h;

		rtre::setBackgroundColor(0.5, 0.1, 0.1, 1.0);
		screen.m_Shader->SetUniform("cameraPos", rtre::camera.position());
		screen.m_Shader->SetUniform("cameraFov", fov);
		screen.m_Shader->SetUniform("time", time);
		screen.m_Shader->SetUniform("aspec", aspectRatio);
		screen.m_Shader->SetUniform("sphereloc", sphereloc);
		screen.m_Shader->SetUniform("sphereRadius", sphereRadius);
		screen.m_Shader->SetUniform("maxits", maxits);
		screen.m_Shader->SetUniform("thresh", thresh);
		screen.m_Shader->SetUniform("matrix", matrix(rtre::camera));
		screen.draw();



		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (window.isKeyPressed(GLFW_KEY_F) && !cursorVisisble) {
			cursorVisisble = !cursorVisisble;
			glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else if (window.isKeyPressed(GLFW_KEY_F) && cursorVisisble) {
			cursorVisisble = !cursorVisisble;
			glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

		time = getTime() - stime;

		rtre::controller::control();
		window.swapBuffers();
	}


	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	rtre::Window::terminate();
}
