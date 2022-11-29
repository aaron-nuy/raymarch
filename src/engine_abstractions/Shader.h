#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <iostream>
#include <ctime>
#include <sys/types.h>
#include <sys/stat.h>
#include <cerrno>
#include <cstring>
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"


namespace rtre {

	using glm::vec4;
	using glm::vec3;
	using glm::vec2;
	using glm::mat4;
	using glm::mat3;
	using glm::mat2;

	std::string get_file_contents(const char* filename) {
		std::fstream file = std::fstream(std::string(filename));
		std::string contents;
		if (file.is_open())
		{
			file.seekg(0, std::ios::end);
			contents.resize(file.tellg());
			file.seekg(0, std::ios::beg);
			file.read(&contents[0], contents.size());
			file.close();

			return(contents);
		}
		else {
			std::string err = std::string("Couldn't load file ") + std::string(filename) + " .\n";
			throw(std::exception(err.c_str()));	
		}
	}

	class AbstractShader {

	protected:
		GLuint m_ID = 0;

	public:
		inline void activate() {
			glUseProgram(m_ID);
		}
		inline virtual void free() {
			glDeleteProgram(m_ID);
		}

		static inline void checkError(GLuint shaderID, const char* type) {

			GLint hasCompiled;

			char infoLog[1024];
			if (type != "PROGRAM")
			{
				glGetShaderiv(shaderID, GL_COMPILE_STATUS, &hasCompiled);
				if (hasCompiled == GL_FALSE)
				{
					glGetShaderInfoLog(shaderID, 1024, NULL, infoLog);
					std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << std::endl;
				}
			}
			else
			{
				glGetProgramiv(shaderID, GL_LINK_STATUS, &hasCompiled);
				if (hasCompiled == GL_FALSE)
				{
					glGetProgramInfoLog(shaderID, 1024, NULL, infoLog);
					std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << infoLog << std::endl;
				}
			}
		}

		inline GLint getUnifromID(const char* name) const {
			return glGetUniformLocation(m_ID, name);
		}

		inline void SetUniform(const char* name, mat4 value) {
			glUniformMatrix4fv(getUnifromID(name), 1, GL_FALSE, glm::value_ptr(value));
		}
		inline void SetUniform(const char* name, mat3 value) {
			glUniformMatrix3fv(getUnifromID(name), 1, GL_FALSE, glm::value_ptr(value));
		}
		inline void SetUniform(const char* name, mat2 value) {
			glUniformMatrix2fv(getUnifromID(name), 1, GL_FALSE, glm::value_ptr(value));
		}
		inline void SetUniform(const char* name, GLfloat v1, GLfloat v2) {
			glUniform2f(getUnifromID(name), v1, v2);
		}
		inline void SetUniform(const char* name, vec2 vec) {
			glUniform2f(getUnifromID(name), vec.x, vec.y);
		}
		inline void SetUniform(const char* name, GLfloat v1, GLfloat v2, GLfloat v3) {
			glUniform3f(getUnifromID(name), v1, v2, v3);
		}
		inline void SetUniform(const char* name, vec3 vec) {
			glUniform3f(getUnifromID(name), vec.x, vec.y, vec.z);
		}
		inline void SetUniform(const char* name, GLfloat v1, GLfloat v2, GLfloat v3, GLfloat v4) {
			glUniform4f(getUnifromID(name), v1, v2, v3, v4);
		}
		inline void SetUniform(const char* name, vec4 vec) {
			glUniform4f(getUnifromID(name), vec.x, vec.y, vec.z, vec.w);
		}
		inline void SetUniform(const char* name, GLint value) {
			glUniform1i(getUnifromID(name), value);
		}
		inline void SetUniform(const char* name, GLuint value) {
			glUniform1ui(getUnifromID(name), value);
		}
		inline void SetUniform(const char* name, GLfloat value) {
			glUniform1f(getUnifromID(name), value);
		}
		inline void SetUniform(const char* name, GLdouble value) {
			glUniform1d(getUnifromID(name), value);
		}

		inline GLuint id() const {
			return m_ID;
		}

	};


	class RenderShader : public AbstractShader {

		std::string vfile;
		std::string ffile;
		time_t lastmod;

	public:

		void checkAndHotplug() {
			struct stat fileInfo;

			stat(ffile.c_str(), &fileInfo);

			if (lastmod == fileInfo.st_mtime)
				return;

			lastmod = fileInfo.st_mtime;

			auto fragmentCode = get_file_contents(ffile.c_str());
			auto vertexCode = get_file_contents(vfile.c_str());


			GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			const char* fc = fragmentCode.data();
			glShaderSource(fragmentShader, 1, &fc, NULL);
			glCompileShader(fragmentShader);
			checkError(fragmentShader, "FRAGMENT");


			GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
			const char* vc = vertexCode.data();
			glShaderSource(vertexShader, 1, &vc, NULL);
			glCompileShader(vertexShader);
			checkError(vertexShader, "VERTEX");

			glDeleteProgram(m_ID);
			m_ID = glCreateProgram();
			glAttachShader(m_ID, vertexShader);
			glAttachShader(m_ID, fragmentShader);
			glLinkProgram(m_ID);

			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

		}


		RenderShader(const char* vertexFile, const char* fragmentFile, const char* geometryFile = NULL)
		{

			 const char* vertexCode;
			 const char* fragmentCode;
			 
			 vfile = vertexFile;
			 ffile = fragmentFile;

			std::string vertexSource = get_file_contents(vertexFile);
			std::string fragSource = get_file_contents(fragmentFile);
			// Read vertexFile and fragmentFile and store the strings
			vertexCode = vertexSource.c_str();
			fragmentCode = fragSource.c_str();


			// Create Vertex Shader Object and get its reference
			GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, &vertexCode, NULL);

			glCompileShader(vertexShader);
			checkError(vertexShader, "VERTEX");

			// Create Fragment Shader Object and get its reference
			GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, &fragmentCode, NULL);
			glCompileShader(fragmentShader);
			checkError(fragmentShader, "FRAGMENT");

			// Create Shader Program Object and get its reference
			m_ID = glCreateProgram();
			glAttachShader(m_ID, vertexShader);
			glAttachShader(m_ID, fragmentShader);
			glLinkProgram(m_ID);
			checkError(m_ID, "PROGRAM");

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			struct stat fileInfo;
			stat(ffile.c_str(), &fileInfo);
			lastmod = fileInfo.st_mtime;

		}

		~RenderShader() {
			glDeleteProgram(m_ID);
		}
	};
}