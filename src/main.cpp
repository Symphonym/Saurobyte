#include <iostream>
#include <string>
#include <sstream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <cstring>
#include <unordered_map>
#include <thread>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.hpp"
#include "ShaderProgram.hpp"
#include "OpenGLWindow.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"
#include "Entity.hpp"
#include "Message.hpp"
#include "MessageHandler.hpp"
#include "Game.hpp"
#include "System.hpp"
#include "IdentifierTypes.hpp"
#include "Logger.hpp"
#include "AudioListener.hpp"
#include "AudioFile.hpp"
#include "AudioDevice.hpp"

#include "Components/LuaComponent.hpp"
#include "Systems/MeshSystem.hpp"
#include "Components/MeshComponent.hpp"

struct Compiz : public jl::Component<Compiz>
{
	int compizVariable;

	Compiz()
		:
		jl::Component<Compiz>("Compiz"),
		compizVariable(0)
	{}

	virtual int onLuaGet(const std::string &valueName, lua_State *state)
	{
		if(valueName == "Variable")
		{
			lua_pushnumber(state, compizVariable);
			return 1;
		}
		else
		{
			lua_pushnil(state);
			return 1;
		}

	};
	virtual void onLuaSet(const std::string &valueName, lua_State *state)
	{
		if(valueName == "Variable")
			compizVariable = luaL_checknumber(state, 1);
	};
};

struct Dem : public jl::System<Dem>
{
	Dem(jl::Game *game)
		:
		jl::System<Dem>(game)
	{
		subscribe("SWAGY");
		addRequirement({jl::TypeIdGrabber::getUniqueTypeID<Compiz>()});
	}

	virtual void processEntity(jl::Entity &entity)
	{
		//broadcast(new jl::Message("SWAGES"));
		//SDL_Log("Processing %i", entity.getID());
		//if(entity.getComponent<Compiz>()->compizVariable == 1337)
		//	entity.removeComponent<Compiz>();
	}

	virtual void onMessage(jl::Message *message)
	{
		SDL_Log("SWAG GOT %s", message->name.c_str());
	};
};

bool audioRunner = true;
int main(int argc, const char* argv[]){
	
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		SDL_Log("%s returned an error. SDL_Error: %s", "SDL_Init", SDL_GetError());
		return 1;
	}

	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

	//REPLACE MESSAGING WITH GLOBAL EVENT SYSTEM IN GAME CLASS WHERE SHIT IS PUSHED TO AND
	//WHICH CAN BE POLLED BY SYSTEMS AND WHATNOT

	jl::Game game("HERRO", 800, 600);
	game.addSystem<Dem>();
	game.addSystem<jl::MeshSystem>();
	game.setLogging(jl::GameLogging::Debug);

	jl::Entity& ent = game.createEntity();
	ent.addComponent<Compiz>();
	//ent.addComponent<jl::LuaComponent>("luaFile.lua");

	//ent.save("FancyCompiz");

	jl::Entity& ent2 = game.createEntity();
	ent2.addComponent<jl::MeshComponent, const std::vector<jl::MeshComponent::Triangle>& >(
	{
		{{-1.0f,-1.0f,-1.0f}, {0.583f, 0.771f, 0.014f}, {0.000059f, 1.0f-0.000004f}},
		{{-1.0f,-1.0f, 1.0f}, {0.609f, 0.115f, 0.436f}, {0.000103f, 1.0f-0.336048f}},
		{{-1.0f, 1.0f, 1.0f}, {0.327f, 0.483f, 0.844f}, {0.335973f, 1.0f-0.335903}},
		{{1.0f, 1.0f,-1.0f}, {0.822f, 0.569f, 0.201f}, {1.000023f, 1.0f-0.000013f}},
		{{-1.0f,-1.0f,-1.0f}, {0.435f, 0.602f, 0.223f}, {0.667979f, 1.0f-0.335851f}},
		{{-1.0f, 1.0f,-1.0f}, {0.310f, 0.747f, 0.185f}, {0.999958f, 1.0f-0.336064f}},
		{{1.0f,-1.0f, 1.0f}, {0.597f, 0.770f, 0.761f}, {0.667979f, 1.0f-0.335851f}},
		{{-1.0f,-1.0f,-1.0f}, {0.559f, 0.436f, 0.730f}, {0.336024f, 1.0f-0.671877f}},
		{{1.0f,-1.0f,-1.0f}, {0.359f, 0.583f, 0.152f}, {0.667969f, 1.0f-0.671889f}},
		{{1.0f, 1.0f,-1.0f}, {0.483f, 0.596f, 0.789f}, {1.000023f, 1.0f-0.000013f}},
		{{1.0f,-1.0f,-1.0f}, {0.559f, 0.861f, 0.639f}, {0.668104f, 1.0f-0.000013f}},
		{{-1.0f,-1.0f,-1.0f}, {0.195f, 0.548f, 0.859f}, {0.667979f, 1.0f-0.335851f}},
		{{-1.0f,-1.0f,-1.0f}, {0.014f, 0.184f, 0.576f}, {0.000059f, 1.0f-0.000004f}},
		{{-1.0f, 1.0f, 1.0f}, {0.771f, 0.328f, 0.970f}, {0.335973f, 1.0f-0.335903f}},
		{{-1.0f, 1.0f,-1.0f}, {0.406f, 0.615f, 0.116f}, {0.336098f, 1.0f-0.000071f}},
		{{1.0f,-1.0f, 1.0f}, {0.676f, 0.977f, 0.133f}, {0.667979f, 1.0f-0.335851f}},
		{{-1.0f,-1.0f, 1.0f}, {0.971f, 0.572f, 0.833f}, {0.335973f, 1.0f-0.335903f}},
		{{-1.0f,-1.0f,-1.0f}, {0.140f, 0.616f, 0.489f}, {0.336024f, 1.0f-0.671877f}},
		{{-1.0f, 1.0f, 1.0f}, {0.997f, 0.513f, 0.064f}, {1.000004f, 1.0f-0.671847f}},
		{{-1.0f,-1.0f, 1.0f}, {0.945f, 0.719f, 0.592f}, {0.999958f, 1.0f-0.336064f}},
		{{1.0f,-1.0f, 1.0f}, {0.543f, 0.021f, 0.978f}, {0.667979f, 1.0f-0.335851f}},
		{{1.0f, 1.0f, 1.0f}, {0.279f, 0.317f, 0.505f}, {0.668104f, 1.0f-0.000013f}},
		{{1.0f,-1.0f,-1.0f}, {0.167f, 0.620f, 0.077f}, {0.335973f, 1.0f-0.335903f}},
		{{1.0f, 1.0f,-1.0f}, {0.347f, 0.857f, 0.137f}, {0.667979f, 1.0f-0.335851f}},
		{{1.0f,-1.0f,-1.0f}, {0.055f, 0.953f, 0.042f}, {0.335973f, 1.0f-0.335903f}},
		{{1.0f, 1.0f, 1.0f}, {0.714f, 0.505f, 0.345f}, {0.668104f, 1.0f-0.000013f}},
		{{1.0f,-1.0f, 1.0f}, {0.783f, 0.290f, 0.734f}, {0.336098f, 1.0f-0.000071f}},
		{{1.0f, 1.0f, 1.0f}, {0.722f, 0.645f, 0.174f}, {0.000103f, 1.0f-0.336048f}},
		{{1.0f, 1.0f,-1.0f}, {0.302f, 0.455f, 0.848f}, {0.000004f, 1.0f-0.671870f}},
		{{-1.0f, 1.0f,-1.0f}, {0.225f, 0.587f, 0.040f}, {0.336024f, 1.0f-0.671877f}},
		{{1.0f, 1.0f, 1.0f}, {0.517f, 0.713f, 0.338f}, {0.000103f, 1.0f-0.336048f}},
		{{-1.0f, 1.0f,-1.0f}, {0.053f, 0.959f, 0.120f}, {0.336024f, 1.0f-0.671877f}},
		{{-1.0f, 1.0f, 1.0f}, {0.393f, 0.621f, 0.362f}, {0.335973f, 1.0f-0.335903f}},
		{{1.0f, 1.0f, 1.0f}, {0.673f, 0.211f, 0.457f}, {0.667969f, 1.0f-0.671889f}},
		{{-1.0f, 1.0f, 1.0f}, {0.820f, 0.883f, 0.371f}, {1.000004f, 1.0f-0.671847f}},
		{{1.0f,-1.0f, 1.0f}, {0.982f, 0.099f, 0.879f}, {0.667979f, 1.0f-0.335851f}},
	},
	"panda.jpg");
	ent2.addComponent<jl::LuaComponent>("luaFile.lua");
	jl::Scene& sc = game.createScene("HUE");

	jl::Scene& sc2 = game.createScene("H");
	sc2.getCamera().setAspectRatio((float)game.getWindow().getWidth()/(float)game.getWindow().getHeight());
	sc2.getCamera().setPosition(glm::vec3(0,0,7));
	sc2.attach(ent2);
	sc2.attach(ent);
	//sc.attach(ent);

	sc.getCamera().setAspectRatio((float)game.getWindow().getWidth()/(float)game.getWindow().getHeight());
	sc.getCamera().setPosition(glm::vec3(0,0,7));
	JL_INFO_LOG("SIZE %i", sizeof(jl::MeshComponent));
	game.changeScene("H");

	// Background color
	glClearColor(0,1,0,1);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glDepthFunc(GL_LESS);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	SF_INFO info;
	SNDFILE *sndFile = sf_open("heartbeat.wav", SFM_READ, &info);
	int sampleNumber = info.channels*info.samplerate;
	JL_INFO_LOG("Channels: %i", info.channels);
	JL_INFO_LOG("Samplerate: %i", info.samplerate);
	JL_INFO_LOG("Frames: %i", info.frames);
	JL_INFO_LOG("Samples: %i", sampleNumber);

	sf_close(sndFile);

	/*
		AudioEngine design

		AudioEngine stores buffers
		playSound(string) creates a new audio source with the buffer stored at 'string'
		the sound will be tracked in a separate thread and deletion of it is taken care of there as well.

		AudioEngine makes sure streams can only be made to files not already streamed and so that static
		sounds use the same buffer (if they want the same sound).

		PlaySound sounds are unconfigurable and is deleted when playback ends.
		CreateSound sounds are returned to the user for further configuration
		and are not deleted until the end of the application unless the user
		says otherwise.

	*/



	JL_INFO_LOG("OPENAL VENDOR: %s", alGetString(AL_VERSION));

	jl::AudioListener::setVolume(0.1f);

	bool wab = true;
	bool wub = false;

	if(true == true && (wab || wub))
		JL_INFO_LOG("TRUE");



	//jl::AudioDevice::registerAudio("Ove Melaa - ItaloLoopDikkoDikko_1.ogg", "Swag");
	jl::AudioDevice::registerAudio("Ove Melaa - ItaloLoopDikkoDikko_1.ogg", "Swag");
	JL_INFO_LOG("SSSSSSSSSSSSSSSSSSS jl AudioFile %i", sizeof(jl::AudioChunk));

	{
		//jl::StreamHandle aaa = jl::AudioDevice::playStream("dadad");
		//aaa->setPitch(2);
		while(true)
		{
			SDL_PumpEvents();
			if(SDL_GetMouseState(NULL,NULL) & SDL_BUTTON(1))
			{
				jl::AudioDevice::stopAllAudio();
			}
			else if(SDL_GetMouseState(NULL,NULL) & SDL_BUTTON(2))
			{
				jl::SoundHandle source = jl::AudioDevice::playSound("Swag");
				source->setPitch(2);
				source->setOffset(5);
			}
			if(SDL_GetKeyboardState(NULL)[SDL_SCANCODE_A])
				break;
			SDL_Delay(100);
		}
	}
	game.gameLoop();


	/*SDL_Event event;

	// Grab location of shader uniform variables
	GLint screenSizeLoc = shaderProgram.getUniformLoc("ScreenSize");
	GLint mousePosLoc = shaderProgram.getUniformLoc("MousePos");
	GLint transforMatLoc = shaderProgram.getUniformLoc("TransformMat");
	GLint textureLoc = shaderProgram.getUniformLoc("tex");

	glUniform1i(textureLoc, 0);

	int mouseX = 0, mouseY = 0;
	const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);

	glm::mat4 transformMat = mesh.getTransform();

	glm::mat4 MVP = camera.getTransform() * transformMat;
	while(window.running())
	{
		game.updateGame();

		// Create normalized values from mouse position
		SDL_GetMouseState(&mouseX, &mouseY);

		float nMouseX = mouseX / (float)window.getWindowWidth(), nMouseY = mouseY / (float)window.getWindowHeight();
		camera.lookAt(glm::vec3(nMouseX*10.f,0, 0));

		MVP = camera.getTransform() * mesh.getTransform();

		// Get keyboard state
		keyboardState = SDL_GetKeyboardState(nullptr);

		while(window.pollEvent(event))
		{
			if(event.type == SDL_QUIT)
				window.close();
			else if(event.type == SDL_KEYDOWN)
			{
				game.broadcast(jl::createMessage<SDL_Event>("KeyDown", event));
				if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
					window.close();
				else if(event.key.keysym.scancode == SDL_SCANCODE_A)
					game.changeScene("HUE");
				else if(event.key.keysym.scancode == SDL_SCANCODE_Q)
					sc.attach(ent);
				else if(event.key.keysym.scancode == SDL_SCANCODE_W)
					game.broadcast(jl::createMessage("ReloadLua"));

			}
			else if(event.type == SDL_KEYUP)
				game.broadcast(jl::createMessage<SDL_Event>("KeyUp", event));
			else if(event.type == SDL_MOUSEBUTTONDOWN)
			{
				//SDL_Log("VAR %i", ent.getComponent<Compiz>()->compizVariable);
				//game.changeScene("H");
				//sc.detach(ent);
				mesh.setColor(1, 1, 1, 1);
				mesh.setRotation(6.28f, 0, 0);
				transformMat = mesh.getTransform();
				std::cout << "FPS: " << window.getFps() << std::endl;
				std::cout << "Delta: " << window.getDelta() << std::endl;
			}
			else if(event.type == SDL_WINDOWEVENT)
			{
				if(event.window.event == SDL_WINDOWEVENT_RESIZED)
					glViewport(
						0,0,
						static_cast<int>(event.window.data1),
						static_cast<int>(event.window.data2));
			}
		}

		mesh.rotate(window.getDelta(),window.getDelta());

		// Give mousepos and screensize to shader
		glUniform2f(mousePosLoc, mouseX, window.getWindowHeight()-mouseY);
		glUniform2f(screenSizeLoc, window.getWindowWidth(), window.getWindowHeight());

		int cameraY = 0;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		MVP = camera.getTransform() * mesh.getTransform();
		glUniformMatrix4fv(transforMatLoc, 1, GL_FALSE, &MVP[0][0]);
		mesh.draw();
		//MVP = camera.getTransform() * mesh2.getTransform();
		//glUniformMatrix4fv(transforMatLoc, 1, GL_FALSE, &MVP[0][0]);
		//mesh2.draw();

		glFlush();

		window.swapBuffers();
	}*/

	SDL_Quit();
	IMG_Quit();

	return 0;
}