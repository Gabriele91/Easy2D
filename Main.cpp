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
	FrameSet::ptr ninjaWalk;


	Render rander;
	Layer* layer1;
	Camera camera;
	Sprite sprite;
	Sprite spriteLight;
	AnimatedSprite spriteAnim;

public:

	MyGame()
	//init screen
	:Game("my game",
		  1280, //width
		  720,  //hight
		  32,   //bit
		  60,   //frame per second
		  false,//fullscreen
		  Easy2D::Screen::MSAAx8)//anti aliasing
	//init textures
	,resources("livel/resources.rs.e2d")
	{}

	~MyGame(){};

	virtual void start(){
		//
		rander.initOpenGL();
		//input
		getInput()->addHandler((Input::KeyboardHandler*)this);
		getInput()->addHandler((Input::MouseHandler*)this);
		//resources
		light=resources.get<Texture>("light");
		ninja=resources.get<Texture>("ninja");
		ninjaWalk=resources.load<FrameSet>("ninjaWalk");
		//test sprite
		sprite.setTexture(ninja);
		//test animation
		spriteAnim.addAnimation(ninjaWalk);
		spriteAnim.setPosition(Vec2(200.0,0.0));
		//test sprite
		spriteLight.setTexture(light);
		spriteLight.enableBlend();
		spriteLight.setBlend(GL_SRC_ALPHA,GL_ONE);
		//add layers
		layer1=rander.addLayer(true);
		layer1->addRenderable(&sprite);
		layer1->addRenderable(&spriteLight);
		layer1->addRenderable(&spriteAnim);
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
		rander.update(dt);
		rander.draw();
	}
	virtual void end(){
	}

	/* OOP-STYLE EVENTS */
	virtual void onKeyRelease(Key::Keyboard key) {
		if(key==Key::ESCAPE)
			Application::instance()->exit();
	}

	virtual void onKeyPress(Key::Keyboard  key){}

	virtual void onKeyDown(Key::Keyboard  key){
			if(key==Key::A){
				spriteAnim.setTranslation(Vec2(-1,0));
				spriteAnim.setScale(Vec2(1.0,1.0));
				spriteAnim.setCull(GL_BACK);
			}
			else
			if(key==Key::D){
				spriteAnim.setTranslation(Vec2(1,0));
				spriteAnim.setScale(Vec2(-1.0,1.0));
				spriteAnim.setCull(GL_FRONT);
			}
	}

	virtual void onMouseMove(Vec2 mousePosition) {
		Vec2 alScreen(  getScreen()->getWidth()/-2.0,
						getScreen()->getHeight()/2.0);
		spriteLight.setPosition((mousePosition*Vec2(1,-1))+alScreen);
	}


};

int main(int,const char **){

	Application::create();
	Application::instance()->exec((Game*)new MyGame());
	delete Application::instance()->getGame();

	return 0;
}
