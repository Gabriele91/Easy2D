#include <stdafx.h>
#include <Easy2D.h>
using namespace Easy2D;

class MyGame : Game, 
			   Input::KeyboardHandler, 
			   Input::MouseHandler

{

	//variable declaration
	ResourcesGroup resources;
	
	Texture::ptr light;
	Texture::ptr ninja;
	Mesh::ptr lightMesh;
	Mesh::ptr ninjaMesh;
	

	Render rander;
	Layer* layer1;
	Camera camera;
	Renderable objLight;
	Renderable objNinja;
	Sprite sprite;
	Sprite spriteLight;

public:

	MyGame()
	//init screen
	:Game("my game",
		  1280, //width
		  720,  //hight
		  32,   //bit
		  30,   //frame per second
		  false)//fullscreen
	//init textures
	,resources("livel/resources.rs")
	{}

	~MyGame(){};

	virtual void start(){
		//
		rander.initOpenGL();
		//input
		Application::instance()->getInput()->addHandler((Input::KeyboardHandler*)this);
		Application::instance()->getInput()->addHandler((Input::MouseHandler*)this);
		//resources
		light=resources.get<Texture>("light");
		ninja=resources.get<Texture>("ninja");
		//test sprite
		sprite.setTexture(ninja);
		//test sprite
		spriteLight.setTexture(light);
		spriteLight.enableBlend();
		spriteLight.setBlend(GL_SRC_ALPHA,GL_ONE);
		//add layers
		layer1=rander.addLayer(true);
		layer1->addRenderable(&sprite);
		layer1->addRenderable(&spriteLight);
		//set camera
		rander.setCamera(&camera);
		//load resources
		resources.load();
	}
	virtual void run(float dt){
		// old style event
		if(getInput()->getKeyHit(Key::A)) 
			std::cout<< "hit A" << std::endl;
		//draw:
		rander.setClear(Color(32,128,255,255));
		rander.draw();
	}
	virtual void end(){
	}

	/* OOP-STYLE EVENTS */
	virtual void onKeyRelease(Key::Keyboard key) {
		if(key==Key::ESCAPE)
			Application::instance()->exit();
	}
	virtual void onMouseMove(Vec2 mousePosition) {	
		Vec2 alScreen(  getScreen()->getWidth()/-2.0,
						getScreen()->getHeight()/2.0);
		spriteLight.setPosition((mousePosition*Vec2(1,-1))+alScreen);
		std::cout << ((mousePosition*Vec2(1,-1))+alScreen).toString() <<"\n";
	}
	
};

int main(int,const char **){

	Application::create();
	auto game=new MyGame();
	Application::instance()->exec((Game*)game);
	delete game;

	return 0;
}