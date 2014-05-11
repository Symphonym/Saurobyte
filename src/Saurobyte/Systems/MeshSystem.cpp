#include <Saurobyte/Systems/MeshSystem.hpp>
#include <Saurobyte/Entity.hpp>
#include <Saurobyte/Message.hpp>
#include <Saurobyte/Game.hpp>
#include <Saurobyte/Components/MeshComponent.hpp>
#include <Saurobyte/OpenGLWindow.hpp>
#include <SDL2/SDL.h>

namespace Saurobyte
{


	MeshSystem::MeshSystem(Game *game)
		:
		System<MeshSystem>(game),
		m_fragShader(GL_FRAGMENT_SHADER, "./Fraggy.frag"),
		m_vertShader(GL_VERTEX_SHADER, "./Verty.vert")
	{
		// Specify we want MeshComponents
		addRequirement({TypeIdGrabber::getUniqueTypeID<MeshComponent>()});

		m_shaderProgram.attachShader(m_fragShader);
		m_shaderProgram.attachShader(m_vertShader);
		m_shaderProgram.linkProgram();
		m_shaderProgram.useProgram();
	}
	MeshSystem::~MeshSystem()
	{
	}

	void MeshSystem::onMessage(Message *message)
	{

		
	}

	void MeshSystem::processEntity(Entity &entity)
	{
		GLint mousePosLoc = m_shaderProgram.getUniformLoc("MousePos");
		GLint transforMatLoc = m_shaderProgram.getUniformLoc("TransformMat");
		GLint textureLoc = m_shaderProgram.getUniformLoc("tex");

		MeshComponent *comp = entity.getComponent<MeshComponent>();

		int mouseX = 0, mouseY = 0;
		SDL_GetMouseState(&mouseX, &mouseY);

		// Give mousepos to shader
		glUniform2f(mousePosLoc, mouseX, game->getWindow().getSize().y-mouseY);

		glm::mat4 transform = glm::mat4(1) * game->getScenePool().getActiveScene()->getCamera().getTransform();
		glUniformMatrix4fv(transforMatLoc, 1, GL_FALSE, &transform[0][0]);
		glUniform1i(textureLoc, 0);


		if(!comp->isLoaded())
			comp->storeData();

		glBindVertexArray(comp->vertexArray);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, comp->meshTexture);
		glBindSampler(0, comp->meshSampler);

		glDrawArrays(GL_TRIANGLES, 0, comp->getVertexCount());

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}


};