#ifndef JL_MESHSYSTEM_HPP
#define JL_MESHSYSTEM_HPP

#include <Saurobyte/System.hpp>
#include <Saurobyte/IdentifierTypes.hpp>
#include <Saurobyte/Shader.hpp>
#include <Saurobyte/ShaderProgram.hpp>

namespace Saurobyte
{
	class MeshSystem : public System<MeshSystem>
	{
	private:

		Shader m_fragShader;
		Shader m_vertShader;

		ShaderProgram m_shaderProgram;


	public:

		MeshSystem(Game *game);
		~MeshSystem();

		virtual void onMessage(Message *message);

		virtual void processEntity(Entity &entity);
	};
};

#endif