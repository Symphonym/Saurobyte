#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstring>
#include <unordered_map>
#include <thread>
#include <memory>
/*
#include <Saurobyte/Shader.hpp>
#include <Saurobyte/ShaderProgram.hpp>
#include <Saurobyte/OpenGLWindow.hpp>
#include <Saurobyte/Mesh.hpp>
#include <Saurobyte/Camera.hpp>
#include <Saurobyte/Entity.hpp>
#include <Saurobyte/Message.hpp>
#include <Saurobyte/MessageHandler.hpp>
#include <Saurobyte/Engine.hpp>
#include <Saurobyte/System.hpp>
#include <Saurobyte/IdentifierTypes.hpp>
#include <Saurobyte/Logger.hpp>
#include <Saurobyte/AudioListener.hpp>
#include <Saurobyte/AudioFile.hpp>
#include <Saurobyte/AudioDevice.hpp>
#include <Saurobyte/BoundingBox.hpp>
*/
#include <Saurobyte/Math/Vector3.hpp>
#include <Saurobyte/Logger.hpp>
#include <Saurobyte/LuaEnvironment.hpp>
#include <Saurobyte/LuaConfig.hpp>
#include <Saurobyte/Message.hpp>
#include <Saurobyte/Input.hpp>
#include <Saurobyte/FrameCounter.hpp>
#include <Saurobyte/Engine.hpp>
#include <Saurobyte/VideoDevice.hpp>
#include <Saurobyte/AudioDevice.hpp>
#include <Saurobyte/AudioListener.hpp>
#include <Saurobyte/Time.hpp>
#include <Saurobyte/IdentifierTypes.hpp>
#include <chrono>

/*
#include <Saurobyte/Components/LuaComponent.hpp>
#include <Saurobyte/Systems/MeshSystem.hpp>
#include <Saurobyte/Components/MeshComponent.hpp>
#include <Saurobyte/Components/TransformComponent.hpp>*/
//#include <Saurobyte/Engine.hpp>

int main(int argc, const char* argv[]){


	Saurobyte::Engine game("Hello world", 800, 600, Saurobyte::Window::Normal);
	Saurobyte::VideoDevice::setBackgroundColor(Saurobyte::Color::Gray);

	SAUROBYTE_INFO_LOG("SPEED TEST C++11 typeid");

	const long testCount = 1000000000;
	typedef std::chrono::high_resolution_clock FrameClock;
	FrameClock::time_point nowSar = FrameClock::now();
	for(int i = 0; i < testCount; i++)
	{
		typeid(Saurobyte::Engine);
		typeid(Saurobyte::Time);
		typeid(Saurobyte::VideoDevice);
		typeid(Saurobyte::AudioDevice);
		typeid(Saurobyte::AudioHandle);
		typeid(Saurobyte::Message);
	}
	FrameClock::time_point thenSar = FrameClock::now();
	SAUROBYTE_INFO_LOG("SPEED TEST C++11 typeid: ", std::chrono::duration_cast<std::chrono::duration<float> >(FrameClock::duration(thenSar-nowSar)).count(), " seconds");

	SAUROBYTE_INFO_LOG("SPEED TEST SaurobyteGrabber");
	nowSar = FrameClock::now();
	for(int i = 0; i < testCount; i++)
	{
		Saurobyte::TypeIdGrabber::getUniqueTypeID<Saurobyte::Engine>();
		Saurobyte::TypeIdGrabber::getUniqueTypeID<Saurobyte::Time>();
		Saurobyte::TypeIdGrabber::getUniqueTypeID<Saurobyte::VideoDevice>();
		Saurobyte::TypeIdGrabber::getUniqueTypeID<Saurobyte::AudioDevice>();
		Saurobyte::TypeIdGrabber::getUniqueTypeID<Saurobyte::AudioHandle>();
		Saurobyte::TypeIdGrabber::getUniqueTypeID<Saurobyte::Message>();
	}
	thenSar = FrameClock::now();
	SAUROBYTE_INFO_LOG("SPEED TEST SaurobyteGrabber: ", std::chrono::duration_cast<std::chrono::duration<float> >(FrameClock::duration(thenSar-nowSar)).count(), " seconds");

	Saurobyte::AudioListener::setVolume(0.5f);

	std::vector<std::string> pb = Saurobyte::AudioDevice::getPlaybackDevices();
	for(std::size_t i = 0; i < pb.size(); i++)
		SAUROBYTE_INFO_LOG("Device ", i, " ", pb[i]);
	std::vector<std::string> pc = Saurobyte::AudioDevice::getCaptureDevices();
	for(std::size_t i = 0; i < pc.size(); i++)
		SAUROBYTE_INFO_LOG("Device ", i, " ", pc[i]);

	Saurobyte::AudioHandle handle = Saurobyte::AudioDevice::createStream("./Ove Melaa - ItaloLoopDikkoDikko_1.ogg");

	SAUROBYTE_INFO_LOG("Offset: ", handle->getOffset().asSeconds(), " Playing: ", handle->isPlaying());
	handle->play();

	//while(handle->isPlaying())
	{
		//Saurobyte::sleep(Saurobyte::seconds(0.01));
		//SAUROBYTE_INFO_LOG("Progress ", handle->getOffset().asSeconds(), " / ", handle->getDuration().asSeconds());
	}

	//handle->stop();
	//Saurobyte::sleep(Saurobyte::seconds(3.5));
	SAUROBYTE_INFO_LOG("Offset: ", handle->getOffset().asSeconds(), " Playing: ", handle->isPlaying());
	//handle->pause();
	//Saurobyte::sleep(Saurobyte::seconds(1));
	SAUROBYTE_INFO_LOG("Offset: ", handle->getOffset().asSeconds(), " Playing: ", handle->isPlaying());
	//handle->play();
	//Saurobyte::sleep(Saurobyte::seconds(1));
	//handle->setOffset(Saurobyte::seconds(0));
	//handle->setOffset(Saurobyte::seconds(0));
	SAUROBYTE_INFO_LOG("Offset: ", handle->getOffset().asSeconds(), " Playing: ", handle->isPlaying());
	//Saurobyte::sleep(6000);
	SAUROBYTE_INFO_LOG("Offset: ", handle->getOffset().asSeconds(), " Playing: ", handle->isPlaying());
	//handle->setLooping(true);
	//Saurobyte::AudioHandle handle2 = Saurobyte::AudioDevice::createSound("./Ove Melaa - ItaloLoopDikkoDikko_1.ogg");
	//handle2->setLooping(true);

	//handle->setOffset(Saurobyte::seconds(12.37));
	//handle2->setOffset(Saurobyte::seconds(12.37));

	//handle->play(); // TODO stream isPlaying is incorrect
	//handle2->play(); // TODO threading bug not fixederu
	//while(handle2->isPlaying())
	//{
		//Saurobyte::sleep(500);
		//SAUROBYTE_INFO_LOG("TIME ", handle->getOffset().asSeconds(), " seconds");
	//}
	//SAUROBYTE_INFO_LOG("IS VALID ", handle2->isValid(), " Seconderu file ", handle2->getDuration().asSeconds());
	game.start();

	/*Saurobyte::Vector3f vec(2, 2);
	vec = vec * 10.f;
	SAUROBYTE_INFO_LOG("Initializing test! X=", vec.x, " Y=", vec.y, "Length=", vec.length());

	Saurobyte::MessageData<int> msg("Jebus", 1337);

	Saurobyte::Message &mesg = msg;
	SAUROBYTE_INFO_LOG("SHIEEET ", mesg.read<int>());


	//while(!Saurobyte::Mouse::isPressed(Saurobyte::Mouse::Left))
	//{
	//	SAUROBYTE_INFO_LOG("X ", Saurobyte::Mouse::getMousePosition().x, " Y ", Saurobyte::Mouse::getMousePosition().y);
	//}

	Saurobyte::LuaEnvironment env;

	Saurobyte::LuaConfig conf(env);
	//conf.writeInt("SauroConf.window.width", 55);
	//conf.writeBool("SauroConf.isCool", true);
	//conf.save("./luaConf.lua");
	conf.load("./luaConf.lua");
	conf.load("./luaManual.lua");

	bool val = false;
	int value = 0;
	conf.readBool("SauroConf.isCool", val);
	conf.readInt("SauroConf.window.width", value);
	SAUROBYTE_INFO_LOG("VAL ", val, " VALUE ", value);
	conf.save("luaConf.lua");*/

	/*env.pushArgs(5);
	env.writeGlobal("TESTERU");

	env.pushArgs(5);
	env.writeGlobal("swag.swag.test");

	env.createClass("Jebus",
	{
		{ "Print", [] (Saurobyte::LuaEnvironment& env) -> int
			{
				printf("\nVal %i\n", env.readArg<int>("Jebus"));
				return 0;
			}
		},
		{ "Add", [] (Saurobyte::LuaEnvironment& env) -> int
			{
				int &val = env.readArg<int>("Jebus");
				int valueToAdd = env.readArg<int>();
				val += valueToAdd;
				return 0;
			}
		},
		{ "Clone", [] (Saurobyte::LuaEnvironment& env) -> int
			{
				int val = env.readArg<int>("Jebus");
				env.pushObject<int>(val, "Jebus");
				return 1;
			}
		}
	});
	env.pushObject(5, "Jebus");
	env.writeGlobal("SWAG");

	env.registerFunction(
		{ "Testeru", [] (Saurobyte::LuaEnvironment& env) -> int
			{
				env.pushArgs("Hello world");
				return 1;
			}
		});
	if(env.runScript("./luaTest.lua"))
			printf("\nReturn args %i\n", env.callFunction("grabString"));
	printf("\nHARU SHITTERU %s\n", env.readStack<std::string>().c_str());

	if(env.readGlobal("swag.swag.test"))
		printf("\nVal %i\n", env.readStack<int>());
	env.pushArgs(
		[] (Saurobyte::LuaEnvironment& env) -> int
			{
				int &val = env.readArg<int>("Jebus");
				int valueToAdd = env.readArg<int>();
				val += 10000;
				return 0;
			}
		);
	env.writeGlobal("SWAG.Add");
	env.runScript("./luaTest.lua");

	env.readGlobal("SDASDASD");
	env.readGlobal("SWAG.goodValue");
	SAUROBYTE_INFO_LOG("VALUE ", env.readStack<int>());*/



	//SAUROBYTE_INFO_LOG("Initializing test! X=%f  Y=%f   Length=%f", vec.x, vec.y, vec.length());
	//SAUROBYTE_INFO_LOG("NORMALIZE test! X=%f  Y=%f   Length=%f", vec.normalized().x, vec.normalized().y, vec.normalized().length());
	//SAUROBYTE_INFO_LOG("Initializing test! X=%f  Y=%f   Length=%f", vec.x, vec.y, vec.length());
	//SAUROBYTE_THROW_ERROR("This shit went cray");
	
	//if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	///{
	//	SDL_Log("%s returned an error. SDL_Error: %s", "SDL_Init", SDL_GetError());
	//	return 1;
	//}


	//REPLACE MESSAGING WITH GLOBAL EVENT SYSTEM IN GAME CLASS WHERE SHIT IS PUSHED TO AND
	//WHICH CAN BE POLLED BY SYSTEMS AND WHATNOT

	//jl::Engine game("HERRO", 800, 600);
	//game.setLogging(jl::EngineLogging::Debug);

	/*jl::SystemPool& sysPool = game.getSystemPool();
	sysPool.addSystem(new jl::MeshSystem(&game));

	jl::Entity& ent = game.createEntity();

	//ent.save("FancyCompiz");

	jl::Entity& ent2 = game.createEntity();
	/*ent2.addComponent<jl::MeshComponent, const std::vector<jl::MeshComponent::Triangle>& >(
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
	ent2.addComponent<jl::TransformComponent>(0,1,0);
	jl::Scene& sc = game.createScene("HUE");

	jl::Scene& sc2 = game.createScene("H");
	sc2.getCamera().setAspectRatio((float)game.getWindow().getWidth()/(float)game.getWindow().getHeight());
	sc2.getCamera().setPosition(glm::vec3(0,0,7));
	sc2.attach(ent2);
	sc2.attach(ent);
	//sc.attach(ent);

	sc.getCamera().setAspectRatio((float)game.getWindow().getWidth()/(float)game.getWindow().getHeight());
	sc.getCamera().setPosition(glm::vec3(0,0,7));
	SAUROBYTE_INFO_LOG("SIZE %i", sizeof(jl::MeshComponent));
	game.changeScene("H");

	// Background color
	glClearColor(0,1,0,1);

	glEnable(GL_DEPTH_TEST);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glDepthFunc(GL_LESS);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


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
	/*std::srand(std::time(0));

	auto randFloat = [] () -> float
	{
		float value = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

		if(value <= 0.5)
			value *= -1.4f;

		value *= static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

		return value;
	};

	jl::BoundingBox box1(jl::Vector3f(1,2,0), 1, 1, 1);
	jl::BoundingBox box2(jl::Vector3f(0,0,0), 1, 1, 1);

	int customDateru = 152;
	
	jl::RTree<int> leTree;
	int varu = 1337;

	std::size_t id = leTree.insert(customDateru, box2);
	//SAUROBYTE_INFO_LOG("VALUE OF 999 %i", *leTree.get(id, box2));
	const float rtreeSpread = 20.f;
	jl::BoundingBox err(jl::Vector3f(0,0,0), rtreeSpread*4,rtreeSpread*4,rtreeSpread*4);
	for(int i = 0; i < 10; i++)
	{
		//SAUROBYTE_INFO_LOG("VALUE %f", randFloat()*rtreeSpread);
		jl::BoundingBox box(jl::Vector3f(
					randFloat()*rtreeSpread,
					randFloat()*rtreeSpread,
					randFloat()*rtreeSpread), 0.1f, 0.1f, 0.1f);

		std::size_t ide = leTree.insert(varu, box);	

		//if(leTree.remove(ide, box))
		//	SAUROBYTE_INFO_LOG("REMOVAL");
	}
	for(int i = 0; i < 5; i++)
		if(leTree.remove(i, err))*/
			//SAUROBYTE_INFO_LOG("REMOVAL");
	//leTree.insert(&varu, box1);
	//leTree.insert(&varu, box2);

	//leTree.printTree();


	//SAUROBYTE_INFO_LOG("Query count: %i, Value %i", leTree.query(box2).size(), *leTree.query(box2)[0]);
	//SAUROBYTE_INFO_LOG("TOTAL AMOUNT OF BOUNDS: %i", leTree.getAllBounds().size());
		
	//auto queryTree = leTree.getAllBounds();
	//SAUROBYTE_INFO_LOG("QUERY COUNT %i", leTree.query(err).size());
	/*for(std::size_t i = 0; i < queryTree.size(); i++)
	{
		jl::Entity &treeCube = game.createEntity();
		jl::BoundingBox box = queryTree[i];
		jl::Vector3f min = box.getMinPoint();
		jl::Vector3f max = box.getMaxPoint();

		jl::Vector3f topFrontLeft = jl::Vector3f(min.x, max.y, min.z);
		jl::Vector3f topFrontRight = jl::Vector3f(max.x, max.y, min.z);
		jl::Vector3f botFrontLeft = jl::Vector3f(min.x, min.y, min.z);
		jl::Vector3f botFrontRight = jl::Vector3f(max.x, min.y, min.z);

		jl::Vector3f topBackLeft = jl::Vector3f(min.x, max.y, max.z);
		jl::Vector3f topBackRight = jl::Vector3f(max.x, max.y, max.z);
		jl::Vector3f botBackLeft = jl::Vector3f(min.x, min.y, max.z);
		jl::Vector3f botBackRight = jl::Vector3f(max.x, min.y, max.z);

		std::vector<jl::MeshComponent::Triangle> triangles =
		{
			{{botFrontLeft.x, botFrontLeft.y, botFrontLeft.z}, {0,0,0}, {0.000059f, 1.0f-0.000004f}},
			{{botBackLeft.x, botBackLeft.y, botBackLeft.z}, {0,0,0}, {0.000103f, 1.0f-0.336048f}},
			{{topBackLeft.x, topBackLeft.y, topBackLeft.z}, {0,0,0}, {0.335973f, 1.0f-0.335903}},
			{{topFrontRight.x, topFrontRight.y, topFrontRight.z}, {0,0,0}, {1.000023f, 1.0f-0.000013f}},
			{{botFrontLeft.x, botFrontLeft.y, botFrontLeft.z}, {0,0,0}},
			{{topFrontLeft.x, topFrontLeft.y, topFrontLeft.z}, {0,0,0}, {0.999958f, 1.0f-0.336064f}},
			{{botBackRight.x, botBackRight.y, botBackRight.z}, {0,0,0}, {0.667979f, 1.0f-0.335851f}},
			{{botFrontLeft.x, botFrontLeft.y, botFrontLeft.z}, {0,0,0}, {0.336024f, 1.0f-0.671877f}},
			{{botFrontRight.x, botFrontRight.y, botFrontRight.z}, {0,0,0}, {0.667969f, 1.0f-0.671889f}},
			{{topFrontRight.x, topFrontRight.y, topFrontRight.z}, {0,0,0}, {1.000023f, 1.0f-0.000013f}},
			{{botFrontRight.x, botFrontRight.y, botFrontRight.z}, {0,0,0}, {0.668104f, 1.0f-0.000013f}},
			{{botFrontLeft.x, botFrontLeft.y, botFrontLeft.z}, {0,0,0}, {0.667979f, 1.0f-0.335851f}},
			{{botFrontLeft.x, botFrontLeft.y, botFrontLeft.z}, {0,0,0}, {0.000059f, 1.0f-0.000004f}},
			{{topBackLeft.x, topBackLeft.y, topBackLeft.z}, {0,0,0}, {0.335973f, 1.0f-0.335903f}},
			{{topFrontLeft.x, topFrontLeft.y, topFrontLeft.z}, {0,0,0}, {0.336098f, 1.0f-0.000071f}},
			{{botBackRight.x, botBackRight.y, botBackRight.z}, {0,0,0}, {0.667979f, 1.0f-0.335851f}},
			{{botBackLeft.x, botBackLeft.y, botBackLeft.z}, {0,0,0}, {0.335973f, 1.0f-0.335903f}},
			{{botFrontLeft.x, botFrontLeft.y, botFrontLeft.z}, {0,0,0}, {0.336024f, 1.0f-0.671877f}},
			{{topBackLeft.x, topBackLeft.y, topBackLeft.z}, {0,0,0}, {1.000004f, 1.0f-0.671847f}},
			{{botBackLeft.x, botBackLeft.y, botBackLeft.z}, {0,0,0}, {0.999958f, 1.0f-0.336064f}},
			{{botBackRight.x, botBackRight.y, botBackRight.z}, {0,0,0}, {0.667979f, 1.0f-0.335851f}},
			{{topBackRight.x, topBackRight.y, topBackRight.z}, {0,0,0}, {0.668104f, 1.0f-0.000013f}},
			{{botFrontRight.x, botFrontRight.y, botFrontRight.z}, {0,0,0}, {0.335973f, 1.0f-0.335903f}},
			{{topFrontRight.x, topFrontRight.y, topFrontRight.z}, {0,0,0}, {0.667979f, 1.0f-0.335851f}},
			{{botFrontRight.x, botFrontRight.y, botFrontRight.z}, {0,0,0}, {0.335973f, 1.0f-0.335903f}},
			{{topBackRight.x, topBackRight.y, topBackRight.z}, {0,0,0}, {0.668104f, 1.0f-0.000013f}},
			{{botBackRight.x, botBackRight.y, botBackRight.z}, {0,0,0}, {0.336098f, 1.0f-0.000071f}},
			{{topBackRight.x, topBackRight.y, topBackRight.z}, {0,0,0}, {0.000103f, 1.0f-0.336048f}},
			{{topFrontRight.x, topFrontRight.y, topFrontRight.z}, {0,0,0}, {0.000004f, 1.0f-0.671870f}},
			{{topFrontLeft.x, topFrontLeft.y, topFrontLeft.z}, {0,0,0}, {0.336024f, 1.0f-0.671877f}},
			{{topBackRight.x, topBackRight.y, topBackRight.z}, {0,0,0}, {0.000103f, 1.0f-0.336048f}},
			{{topFrontLeft.x, topFrontLeft.y, topFrontLeft.z}, {0,0,0}, {0.336024f, 1.0f-0.671877f}},
			{{topBackLeft.x, topBackLeft.y, topBackLeft.z}, {0,0,0}, {0.335973f, 1.0f-0.335903f}},
			{{topBackRight.x, topBackRight.y, topBackRight.z}, {0,0,0}, {0.667969f, 1.0f-0.671889f}},
			{{topBackLeft.x, topBackLeft.y, topBackLeft.z}, {0,0,0}, {1.000004f, 1.0f-0.671847f}},
			{{botBackRight.x, botBackRight.y, botBackRight.z}, {0,0,0}, {0.667979f, 1.0f-0.335851f}},	
		};

		treeCube.addComponent<jl::MeshComponent, const std::vector<jl::MeshComponent::Triangle>& >(triangles, "whiteBG.jpg");
		sc2.attach(treeCube);
	}

	
	SAUROBYTE_INFO_LOG("OPENAL VENDOR: %s", alGetString(AL_VERSION));
	jl::AudioListener::setVolume(0.001f);

	//jl::AudioDevice::registerAudio("Ove Melaa - ItaloLoopDikkoDikko_1.ogg", "Swag");
	jl::AudioDevice::registerAudio("Ove Melaa - ItaloLoopDikkoDikko_1.ogg", "Swag");
	SAUROBYTE_INFO_LOG("SSSSSSSSSSSSSSSSSSS jl AudioFile %i", sizeof(jl::AudioChunk));
	{
		for(int i = 0; i < 300; i++)
		{
			jl::AudioHandle sounderu = jl::AudioDevice::playSound("Swag");
			sounderu->setVolume(0.5f);
		}

		// TODO fix leastImportantSource algorithm of AudioDevice

		//jl::AudioHandle sounderu = jl::AudioDevice::playSound("Swag");
				//sounderu->setOffset(5);
				//sounderu->setLooping(true);
				//sounderu->setRelativeToListener(true);
				//float x = 0;
		while(true)
		{
			SDL_PumpEvents();
			if(SDL_GetMouseState(NULL,NULL) & SDL_BUTTON(1))
			{
				//jl::AudioDevice::stopAllAudio();
				//x+= 0.1f;
				//sounderu->setPosition(jl::Vector3f(x,0,0));
			}
			else if(SDL_GetMouseState(NULL,NULL) & SDL_BUTTON(2))
			{
							jl::AudioHandle sounderu = jl::AudioDevice::playSound("Swag");
			sounderu->setPosition(jl::Vector3f(0, 100, 0));
			sounderu->setVolume(0.5f);
			sounderu->setLooping(true);
				//source->setOffset(5);
				//source->setLooping(true);
			}
			if(SDL_GetKeyboardState(NULL)[SDL_SCANCODE_A])
				break;
			SDL_Delay(100);
		}
	}*/
	//game.gameLoop();


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
		game.updateEngine();

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

	//SDL_Quit();

	return 0;
}