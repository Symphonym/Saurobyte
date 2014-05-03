#ifndef JL_MESHSYSTEM_HPP
#define JL_MESHSYSTEM_HPP

#include <Saurobyte/System.hpp>
#include <Saurobyte/IdentifierTypes.hpp>
#include <Saurobyte/Shader.hpp>
#include <Saurobyte/ShaderProgram.hpp>

namespace jl
{
	class MeshSystem : public System<MeshSystem>
	{
	private:

		jl::Shader m_fragShader;
		jl::Shader m_vertShader;

		jl::ShaderProgram m_shaderProgram;


	public:

		MeshSystem(Game *game);
		~MeshSystem();

		virtual void onMessage(jl::Message *message);

		virtual void processEntity(Entity &entity);
	};
};

#endif