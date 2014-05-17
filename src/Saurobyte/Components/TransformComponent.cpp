#include <Saurobyte/Components/TransformComponent.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Lua/lua.hpp>

namespace Saurobyte
{

	TransformComponent::TransformComponent(float x, float y, float z)
		:
		m_position(x, y, z),
		m_rotation(0,0,0),
		m_modelTransform(1)
	{

	}
	TransformComponent::TransformComponent(const Vector3f &position, const Vector3f &rotation)
		:
		m_position(position),
		m_rotation(rotation),
		m_modelTransform(1)
	{

	}

	void TransformComponent::setPosition(float x, float y, float z)
	{
		m_position = Vector3f(x, y, z);
		updateTransform();
	}
	void TransformComponent::setPosition(const Vector3f &position)
	{
		m_position = position;
		updateTransform();
	}

	void TransformComponent::move(float xOffset, float yOffset, float zOffset)
	{
		m_position += Vector3f(xOffset, yOffset, zOffset);
		updateTransform();
	}
	void TransformComponent::move(const Vector3f &posOffset)
	{
		m_position += posOffset;
		updateTransform();
	}

	void TransformComponent::setRotation(float x, float y, float z)
	{
		m_rotation = Vector3f(x, y, z);
		updateTransform();
	}
	void TransformComponent::setRotation(const Vector3f &rotation)
	{
		m_rotation = rotation;
		updateTransform();
	}

	void TransformComponent::rotate(float x, float y, float z)
	{
		m_rotation = Vector3f(x, y, z);
		updateTransform();
	}
	void TransformComponent::rotate(const Vector3f &rotation)
	{
		m_rotation += rotation;
		updateTransform();
	}


	void TransformComponent::onLuaSet(const std::string& valueName, LuaEnvironment &env)
	{
		if(valueName == "position")
		{
			m_position.x = env.toNumber();
			m_position.y = env.toNumber();
			m_position.z = env.toNumber();
		}
		else if(valueName == "x")
			m_position.x = env.toNumber();
		else if(valueName == "y")
			m_position.y = env.toNumber();
		else if(valueName == "z")
			m_position.z = env.toNumber();
		else if(valueName == "rotation")
		{
			m_rotation.x = env.toNumber();
			m_rotation.y = env.toNumber();
			m_rotation.z = env.toNumber();
		}

	}
	int TransformComponent::onLuaGet(const std::string& valueName, LuaEnvironment &env)
	{
		if(valueName == "position")
		{
			env.pushArgs(m_position.x, m_position.y, m_position.z);
			return 3;
		}
		else if(valueName == "x")
		{
			env.pushArgs(m_position.x);
			return 1;
		}
		else if(valueName == "y")
		{
			env.pushArgs(m_position.y);
			return 1;
		}
		else if(valueName == "z")
		{
			env.pushArgs(m_position.z);
			return 1;
		}
		else if(valueName == "rotation")
		{
			env.pushArgs(m_rotation.x, m_rotation.y, m_rotation.z);
			return 3;
		}
	}

	std::string TransformComponent::getName() const
	{
		return "TransformComponent";
	}


	const Vector3f& TransformComponent::getPosition() const
	{
		return m_position;
	}
	const Vector3f& TransformComponent::getRotation() const
	{
		return m_rotation;
	}
	const Matrix4& TransformComponent::getTransform() const
	{
		return m_modelTransform;
	}



	void TransformComponent::updateTransform()
	{
		/* TODO Matrix4 positionMat = glm::translate(glm::mat4(1), m_position);

		Matrix4 rotationMat = Matrix4(1);
		rotationMat = glm::rotate(rotationMat, m_rotation.x, glm::vec3(0, 1, 0));
		rotationMat = glm::rotate(rotationMat, m_rotation.y, glm::vec3(-1, 0, 0));
		rotationMat = glm::rotate(rotationMat, m_rotation.z, glm::vec3(0, 0, -1));

		// Set model transform by multiplying rotaton and position
		m_modelTransform = rotationMat * positionMat;*/

	}

};