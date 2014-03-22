#include "Camera.hpp"	

namespace jl
{

	Camera::Camera()
		:
		m_projectionTransform(1),
		m_viewTransform(1),
		m_cameraTransform(1),
		m_cameraPosition(0,0,0),
		m_lookingAt(0,0,0),
		m_fov(45.f),
		m_aspectRatio(800.f/600.f),
		m_nearPlane(0.1f),
		m_farPlane(100.f)
	{
		updatePerspective();
	}

	void Camera::updatePerspective()
	{
		m_projectionTransform = glm::perspective(m_fov, m_aspectRatio, m_nearPlane, m_farPlane);
		m_cameraTransform = m_projectionTransform * m_viewTransform;
	}

	void Camera::setFov(float fov)
	{
		m_fov = fov;
		updatePerspective();
	}
	void Camera::setAspectRatio(float aspectRatio)
	{
		m_aspectRatio = aspectRatio;
		updatePerspective();
	}
	void Camera::setPlanes(float near, float far)
	{
		m_nearPlane = near;
		m_farPlane = far;
		updatePerspective();
	}

	void Camera::move(const glm::vec3 &offset)
	{
		m_cameraPosition += offset;
		m_viewTransform = glm::lookAt(m_cameraPosition, m_lookingAt, glm::vec3(0, 1, 0));
		updatePerspective();
	}
	void Camera::setPosition(const glm::vec3 &position)
	{
		m_cameraPosition = position;
		m_viewTransform = glm::lookAt(m_cameraPosition, m_lookingAt, glm::vec3(0, 1, 0));
		updatePerspective();
	}

	void Camera::lookAt(const glm::vec3 &target)
	{
		m_lookingAt = target;
		m_viewTransform = glm::lookAt(m_cameraPosition, m_lookingAt, glm::vec3(0, 1, 0));
		updatePerspective();
	}

	const glm::mat4& Camera::getTransform() const
	{
		return m_cameraTransform;
	}

}