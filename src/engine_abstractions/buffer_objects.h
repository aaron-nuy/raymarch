#pragma once
#include <vector>
#include <array>
#include "glad/glad.h"
#include "glm/glm.hpp"


namespace rtre {

	using glm::vec4;
	using glm::vec3;
	using glm::vec2;
	using glm::mat4;
	using glm::mat3;
	using glm::mat2;

	class Ebo {

		GLuint m_ID = 0;

	public:
		Ebo() {
			glGenBuffers(1, &m_ID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
		}
		Ebo(Ebo& eb) {
			m_ID = eb.id();
		}
		Ebo(Ebo&& eb) noexcept {
			m_ID = eb.id();
			eb.free();
		}

		~Ebo() {
			glDeleteBuffers(1, &m_ID);
		}


		Ebo(const std::vector<GLuint>& vertices) {
			glGenBuffers(1, &m_ID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertices.size() * sizeof(GLuint), vertices.data(), GL_STATIC_DRAW);
		}
		template<std::size_t n>
		Ebo(const std::array<GLuint, n>& vertices) {
			glGenBuffers(1, &m_ID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertices.size() * sizeof(GLuint), vertices.data(), GL_STATIC_DRAW);
		}
		Ebo(GLuint* vertices, GLsizeiptr size) {
			glGenBuffers(1, &m_ID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
		}


		inline void loadData(const std::vector<GLuint>& vertices) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertices.size() * sizeof(GLuint), vertices.data(), GL_STATIC_DRAW);
		}
		template<std::size_t n>
		inline void loadData(const std::array<GLuint, n>& vertices) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertices.size() * sizeof(GLuint), vertices.data(), GL_STATIC_DRAW);
		}
		inline void loadData(GLuint* vertices, GLsizeiptr size) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
		}

		inline void bind() {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
		}
		inline void unbind() {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
		}
		inline void free() {
			glDeleteBuffers(1, &m_ID);
		}

		inline GLuint id() const {
			return m_ID;
		}
	};
	class Vbo {
		GLuint m_ID = 0;

	public:

		Vbo() {
			glGenBuffers(1, &m_ID);
		}
		Vbo(Vbo& vb) {
			m_ID = vb.id();
		}
		Vbo(Vbo&& vb) noexcept {
			m_ID = vb.id();
			vb.free();
		}
		template<class T>
		Vbo(const std::vector<T>& vertices) {
			glGenBuffers(1, &m_ID);
			glBindBuffer(GL_ARRAY_BUFFER, m_ID);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T), vertices.data(), GL_STATIC_DRAW);
		}
		template<class T, std::size_t n>
		Vbo(const std::array<T, n>& vertices) {
			glGenBuffers(1, &m_ID);
			glBindBuffer(GL_ARRAY_BUFFER, m_ID);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T), vertices.data(), GL_STATIC_DRAW);
		}
		Vbo(GLfloat* vertices, GLsizeiptr size) {
			glGenBuffers(1, &m_ID);
			glBindBuffer(GL_ARRAY_BUFFER, m_ID);
			glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
		}

		~Vbo() {
			glDeleteBuffers(1, &m_ID);
		}

		template<class T>
		inline void loadData(const std::vector<T>& vertices) {
			glBindBuffer(GL_ARRAY_BUFFER, m_ID);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T), vertices.data(), GL_STATIC_DRAW);
		}
		template<class T, std::size_t n>
		inline void loadData(const std::array<T, n>& vertices) {
			glBindBuffer(GL_ARRAY_BUFFER, m_ID);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T), vertices.data(), GL_STATIC_DRAW);
		}
		inline void loadData(GLfloat* vertices, GLsizeiptr size) {
			glBindBuffer(GL_ARRAY_BUFFER, m_ID);
			glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
		}

		inline void bind() {
			glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		}
		inline void unbind() {
			glBindBuffer(GL_ARRAY_BUFFER, NULL);
		}
		inline void free() {
			glDeleteBuffers(1, &m_ID);
		}

		inline GLuint id() const {
			return m_ID;
		}

	};
	class Vao {
		GLuint m_ID = 0;

	public:

		Vao() {
			glGenVertexArrays(1, &m_ID);
			glBindVertexArray(m_ID);

		}

		~Vao() {
			glDeleteVertexArrays(1, &m_ID);
		}


		/*
			Links a VBO to the current Vao

			NOTE : This function does not bind the Vao, it must be binded before calling it
		*/
		inline void linkAttrib( GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
			glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
			glEnableVertexAttribArray(layout);
		}

		inline void bind() {
			glBindVertexArray(m_ID);
		}
		inline void unbind() {
			glBindVertexArray(0);
		}
		inline void free() {
			glDeleteVertexArrays(1, &m_ID);
		}

		inline GLuint id() const {
			return m_ID;
		}
	};

}