#ifndef JL_TRANSFORMCOMPONENT_HPP
#define JL_TRANSFORMCOMPONENT_HPP

#include <Saurobyte/Math/Vector3.hpp>
#include <Saurobyte/Math.hpp>
#include <Saurobyte/Component.hpp>

namespace Saurobyte
{
	class TransformComponent : public Component<TransformComponent>
	{
	private:

		// Individual data
		Vector3f m_position;
		Vector3f m_rotation;

		// Final model transform, complete with rotation and translation
		Matrix4 m_modelTransform;

		void updateTransform();

	public:

		explicit TransformComponent(float x = 0, float y = 0, float z = 0);
		explicit TransformComponent(const Vector3f &position, const Vector3f &rotation = Vector3f(0,0,0));

		// Sets the position
		void setPosition(float x, float y, float z);
		void setPosition(const Vector3f &position);

		// Adds to the current position
		void move(float xOffset, float yOffset = 0, float zOffset = 0);
		void move(const Vector3f &posOffset);


		// Sets the rotation
		void setRotation(float x, float y, float z);
		void setRotation(const Vector3f &rotation);

		// Adds to the current rotation
		void rotate(float x, float y = 0, float z = 0);
		void rotate(const Vector3f &rotation);

		virtual void onLuaSet(const std::string& valueName, LuaEnvironment &env);
		virtual int onLuaGet(const std::string& valueName, LuaEnvironment &env);

		virtual std::string getName() const;

		const Vector3f& getPosition() const;
		const Vector3f& getRotation() const;
		const Matrix4& getTransform() const; 

	};
};

#endif