#include "TransformComponent.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <Lua/lua.hpp>

namespace jl
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


	void TransformComponent::onLuaSet(const std::string& valueName, lua_State *state)
	{
		if(valueName == "position")
		{
			m_position.x = luaL_checknumber(state, 1);
			m_position.y = luaL_checknumber(state, 2);
			m_position.z = luaL_checknumber(state, 3);
		}
		else if(valueName == "x")
			m_position.x = luaL_checknumber(state, 1);
		else if(valueName == "y")
			m_position.y = luaL_checknumber(state, 1);
		else if(valueName == "z")
			m_position.z = luaL_checknumber(state, 1);
		else if(valueName == "rotation")
		{
			m_rotation.x = luaL_checknumber(state, 1);
			m_rotation.y = luaL_checknumber(state, 2);
			m_rotation.z = luaL_checknumber(state, 3);
		}

	}
	int TransformComponent::onLuaGet(const std::string& valueName, lua_State *state)
	{
		if(valueName == "position")
		{
			lua_pushnumber(state, m_position.x);
			lua_pushnumber(state, m_position.y);
			lua_pushnumber(state, m_position.z);

			return 3;
		}
		else if(valueName == "x")
		{
			lua_pushnumber(state, m_position.x);
			return 1;
		}
		else if(valueName == "y")
		{
			lua_pushnumber(state, m_position.y);
			return 1;
		}
		else if(valueName == "z")
		{
			lua_pushnumber(state, m_position.z);
			return 1;
		}
		else if(valueName == "rotation")
		{
			lua_pushnumber(state, m_rotation.x);
			lua_pushnumber(state, m_rotation.y);
			lua_pushnumber(state, m_rotation.z);

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
		Matrix4 positionMat = glm::translate(glm::mat4(1), m_position);

		Matrix4 rotationMat = Matrix4(1);
		rotationMat = glm::rotate(rotationMat, m_rotation.x, glm::vec3(0, 1, 0));
		rotationMat = glm::rotate(rotationMat, m_rotation.y, glm::vec3(-1, 0, 0));
		rotationMat = glm::rotate(rotationMat, m_rotation.z, glm::vec3(0, 0, -1));

		// Set model transform by multiplying rotaton and position
		m_modelTransform = rotationMat * positionMat;

	}

};