#pragma once
#include <vector>
#include "../engine_abstractions/sampler.h"
#include "../engine_abstractions/shader.h"
#include "../rtre_base.h"
#include "../engine_abstractions/buffer_objects.h"

namespace rtre {


	using glm::vec2;
	using glm::vec3;
	using glm::vec4;
	using glm::mat2;
	using glm::mat3;
	using glm::mat4;

	class Polygone {
	public:
		std::shared_ptr<RenderShader> m_Shader;
		virtual void draw() final  {
			m_Shader->activate();
			m_Vao.bind();

			m_Shader->SetUniform("rotateM", m_Rotate);
			m_Shader->SetUniform("positionV", m_Position);
			m_Shader->SetUniform("aspectRatioV", aspectRatio);
			m_Shader->SetUniform("scaleV", m_Proportions);

			

			glDrawElements(GL_TRIANGLES, s_Indices.size(), GL_UNSIGNED_INT, 0);
		}

		virtual void setPosition(vec2 vec) final  {
			m_Position = vec;
		}
		virtual void setPosition(GLfloat x, GLfloat y) final  {
			m_Position = vec2(x, y);
		}
		virtual void setProportions(vec2 vec) final  {
			m_Proportions = vec;
		}
		virtual void setProportions(GLfloat width, GLfloat height) final  {
			m_Proportions = vec2(width, height);
		}
		/*
		 Must be normalized, between (-1,1) 
		*/
		virtual void setHeight(GLfloat height) final  {
			m_Proportions.y = height;
		}
		/*
		 Must be normalized, between (-1,1)
		*/
		virtual void setWidth(GLfloat width) final  {
			m_Proportions.x = width;
		}
		
		/*
			Degress Must be in radians
		*/
		virtual void rotate(GLfloat angle, const vec3& axis = vec3(0, 0, 1)) final  {
			m_Rotate = glm::rotate(angle, axis);
		}


	protected:

		mat4 m_Rotate = mat4(1.0f);
		vec2 m_Position = vec2(0, 0);
		vec2 m_Proportions = vec2(1, 1);


		std::shared_ptr<Sampler2D> m_Texture;

		static const std::vector<Vertex2> s_Vertices;
		static const std::vector<GLuint> s_Indices;

		Vao m_Vao;
		Vbo m_Vbo;
		Ebo m_Ebo;

		void loadData() {
			m_Vao.bind();
			m_Vbo.loadData(s_Vertices);
			m_Ebo.loadData(s_Indices);
			m_Vao.linkAttrib(0, 2, GL_FLOAT, sizeof(Vertex2), (void*)0);
			m_Vao.linkAttrib(1, 2, GL_FLOAT, sizeof(Vertex2), (void*)sizeof(vec2));
			m_Vao.unbind();
			m_Vbo.unbind();
			m_Ebo.unbind();
		}


		virtual void __placeholder_() = 0;


	};

	class Quad : public Polygone{
		virtual void __placeholder_() override {}
	public:



		Quad() {
		}

        Quad(std::shared_ptr<RenderShader> shader,
            const vec2& position = vec2(0), const vec2 scale = vec2(1))
        {
            m_Position = position;
			m_Proportions = scale;

            m_Shader = shader;

			loadData();
        }

	};


	const std::vector<Vertex2> Polygone::s_Vertices = {
		Vertex2(vec2(-.5,-.5),vec2(0,0)),
		Vertex2(vec2(.5,-.5),vec2(1,0)),
		Vertex2(vec2(.5, .5),vec2(1,1)),
		Vertex2(vec2(-.5, .5),vec2(0,1)),
	};
	
	const std::vector<GLuint> Polygone::s_Indices = {
		0,1,2,
		2,3,0
	};

}