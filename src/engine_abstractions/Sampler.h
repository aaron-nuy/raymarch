#pragma once
#include <exception>
#include <array>
#include "glad/glad.h"
#include "dependencies/stb_image.h"
#include "shader.h"

namespace rtre {

	enum Senum {
		rTdiffuse,
		rTspecular,
		rTnormal,
		rTheight
	};
	class Sampler {
	public:
		virtual inline GLuint unit() const { return m_Unit; }
		virtual inline GLuint id() const { return m_ID; }
		virtual inline Senum type() const { return m_Type; }
		virtual inline void free() { glDeleteTextures(1, &m_ID); };

		virtual inline void setUnit(GLuint unit) { m_Unit = unit; }
		virtual inline void setType(Senum type) { m_Type = type; }

		virtual void assign(RenderShader& shader, const char* uniform, GLuint unit) 
		{
			GLuint samplerUniform = glGetUniformLocation(shader.id(), uniform);

			shader.activate();

			glUniform1i(samplerUniform, unit);
		}

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void setPath(const std::string& path) { m_Path = path; }
		virtual const std::string& path() { return m_Path; }

	protected:
		GLuint m_Unit = 0;
		GLuint m_ID = 0;
		Senum m_Type = rTdiffuse;
		std::string m_Path;
	};

	class Sampler2D : public Sampler {
	public:
		Sampler2D(const char* image, GLuint unit, Senum type = rTdiffuse)
		{
			m_Unit = unit;
			m_Type = type;
			int widthImg, heightImg, numColCh;
			stbi_set_flip_vertically_on_load(true);
			unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

			glGenTextures(1, &m_ID);
			glActiveTexture(GL_TEXTURE0 + unit);
			glBindTexture(GL_TEXTURE_2D, m_ID);
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			if (bytes) {
				switch (numColCh) {
				case 4:
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
					break;
				case 3:
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
					break;
				case 1:
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RED, GL_UNSIGNED_BYTE, bytes);
					break;
				default:
				{
					stbi_image_free(bytes);
					glBindTexture(GL_TEXTURE_2D, 0);
					throw std::exception("Automatic Texture type recognition failed\n");
					break;
				}
				}

				glGenerateMipmap(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, 0);

				stbi_image_free(bytes);
			}
			else {
				glBindTexture(GL_TEXTURE_2D, 0);
				std::string exceptionMessage = "Failed to load texture: " + std::string(image);
				stbi_image_free(bytes);
				throw std::exception(exceptionMessage.c_str());

			}
		}

		Sampler2D(const std::string& texture, GLuint unit, Senum type = rTdiffuse)
		{
			const char* image = texture.c_str();

			m_Unit = unit;
			m_Type = type;
			int widthImg, heightImg, numColCh;
			stbi_set_flip_vertically_on_load(true);
			unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

			glGenTextures(1, &m_ID);
			glActiveTexture(GL_TEXTURE0 + unit);
			glBindTexture(GL_TEXTURE_2D, m_ID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			if (bytes) {
				switch (numColCh) {
				case 4:
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
					break;
				case 3:
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
					break;
				case 1:
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RED, GL_UNSIGNED_BYTE, bytes);
					break;
				default:
				{
					stbi_image_free(bytes);
					glBindTexture(GL_TEXTURE_2D, 0);
					throw std::exception("Automatic Texture type recognition failed\n");
					break;
				}
				}

				glGenerateMipmap(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, 0);

				stbi_image_free(bytes);
			}
			else {
				glBindTexture(GL_TEXTURE_2D, 0);
				std::string exceptionMessage = "Failed to load texture: " + std::string(image);
				stbi_image_free(bytes);
				throw std::exception(exceptionMessage.c_str());

			}
		}

		inline void bind() override {
			glActiveTexture(GL_TEXTURE0 + m_Unit);
			glBindTexture(GL_TEXTURE_2D, m_ID);
		}
		inline void unbind() override { glBindTexture(GL_TEXTURE_2D, 0); }
	};

	class Sampler3D : public Sampler {
	public:

		Sampler3D(const std::array<std::string,6>& mapsides,GLuint unit,Senum type = rTdiffuse)
		{
			m_Unit = unit;
			m_Type = type;

			glGenTextures(1, &m_ID);

			glActiveTexture(GL_TEXTURE0 + unit);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			// Loading textures to cubemap
			for (GLuint i = 0; i < 6; i++) {
				stbi_set_flip_vertically_on_load(false);
				int widthImg, heightImg, numColCh;
				unsigned char* bytes = stbi_load(mapsides[i].c_str(), &widthImg, &heightImg, &numColCh, 0);

				if (!bytes) {
					glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
					std::string exceptionMessage = "Failed to load texture: " + mapsides[i] + "\n";
					stbi_image_free(bytes);
					throw std::exception(exceptionMessage.c_str());
				}

				switch (numColCh) {
				case 4:
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
					break;
				case 3:
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
					break;
				case 1:
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, widthImg, heightImg, 0, GL_RED, GL_UNSIGNED_BYTE, bytes);
					break;
				default:
				{
					glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
					stbi_image_free(bytes);
					throw std::invalid_argument("Automatic Texture type recognition failed\n");
					break;
				}

					stbi_image_free(bytes);
				}

			}
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}

		inline void bind() override {
			glActiveTexture(GL_TEXTURE0 + m_Unit);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
		}
		inline void unbind() override { glBindTexture(GL_TEXTURE_CUBE_MAP, 0); }
	};



}