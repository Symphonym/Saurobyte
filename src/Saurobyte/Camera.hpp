#ifndef JL_CAMERA_HPP
#define JL_CAMERA_HPP

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Saurobyte
{

	class Camera
	{
	private:

		glm::mat4 m_projectionTransform;
		glm::mat4 m_viewTransform;
		glm::mat4 m_cameraTransform; // Cache for proj * view transform

		glm::vec3 m_cameraPosition;
		glm::vec3 m_lookingAt; // Where you're looking


		float m_fov;
		float m_aspectRatio;
		float m_nearPlane, m_farPlane;

		void updatePerspective();

	public:

		Camera();

		void setFov(float fov);
		void setAspectRatio(float aspectRatio);
		void setPlanes(float near, float far);

		void move(const glm::vec3 &offset);
		void setPosition(const glm::vec3 &position);

		void lookAt(const glm::vec3 &target);

		const glm::mat4& getTransform() const;
	};
};

#endif