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
		light=resources.load<Texture>("light");
		ninja=resources.load<Texture>("ninja");
		lightMesh=resources.load<Mesh>("sprite");
		ninjaMesh=resources.load<Mesh>("sprite");
		//set randable
		objLight.setTexture(light);
		objLight.setMesh(lightMesh);
		objLight.setScale(Vec2(1,1));
		objLight.enableBlend();
		objLight.setBlend(GL_SRC_ALPHA,GL_ONE);
		objLight.setZ(1.0);

		objNinja.setTexture(ninja);
		objNinja.setMesh(ninjaMesh);
		objNinja.setScale(Vec2(1,1));
		objNinja.enableBlend();
		objNinja.setBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//add layers
		layer1=rander.addLayer(true);
		layer1->addRenderable(&objLight);
		layer1->addRenderable(&objNinja);
		//set camera
		rander.setCamera(&camera);
		//load resources
		resources.load();
	}
	virtual void run(float dt){
		glClearColor(0.25,0.5,1,1);
		glClear(GL_COLOR_BUFFER_BIT);
		/* old style event */
		if(Application::instance()->getInput()->getKeyHit(Key::A)) 
			std::cout<< "hit A" << std::endl;
		//draw:
		rander.draw();
	}
	virtual void end(){
	}

	/* OOP-STYLE EVENTS */
	virtual void onKeyPress(Key::Keyboard key) {
		std::cout<<"KeyPress:"<<Key::stringKeyboard[key]<<std::endl;
	}
	virtual void onKeyRelease(Key::Keyboard key) {
		std::cout<<"KeyRelease:"<<Key::stringKeyboard[key]<<std::endl;	
	}
	virtual void onMouseMove(Vec2 mousePosition) {	
		std::cout<<"MouseMove:"<<mousePosition.toString()<<std::endl;
	}
	virtual void onMousePress(Vec2 mousePosition, Key::Mouse button) {	
		std::cout<<"MousePress:"<<Key::stringMouse[button]<<" "<<mousePosition.toString()<<std::endl;
	}
	virtual void onMouseRelease(Vec2 mousePosition, Key::Mouse button) {
		std::cout<<"MouseRelease:"<<Key::stringMouse[button]<<" "<<mousePosition.toString()<<std::endl;	
	}
	virtual void onMouseScroll(short scrollDelta) {
		std::cout<<"MouseScroll:"<<scrollDelta<<std::endl;	
	}
	
};

int main(int,const char **){

	Application::create();
	auto game=new MyGame();
	Application::instance()->exec((Game*)game);
	delete game;
/*
	
	Object root;	
	Object *child1=new Object();
	Object *child2=new Object();
	root.addChild(child1);
	root.addChild(child2);
	child1->setScale(Vec2(2,1));
	child2->setScale(Vec2(2,1));
	root.setScale(Vec2(2,1));
	child1->setScale(Vec2(3,1),true);
	

	std::cout << child1->getScale().x << std::endl;
	std::cout << child2->getScale(true).x << std::endl;
	std::cout << root.getScale(true).x << std::endl;

	Table test;
	test.set(2);
	test.set("ciao :D");
	test.set(1);
	test.set("c44",3);
	test.set("c44",4);
	for( auto t : test ){
		if(t.first.isString())
			std::cout<< "elment:" <<  t.first.string()  << std::endl;
		else
		    std::cout<< "elment:" <<  t.first.integer() << std::endl;
	}
	
	std::cout << "float value: " << test.getFloat("c44") << std::endl;
	std::cout << "float value: " << test.getFloat(1) << std::endl;
	std::cout << "string value: " << test.getString(200) << std::endl;
	
	Table test2;
	if(test2.deserialize(
		"{\n"
		"//{ 2 3 }\n"
		"index = { 1 2 3 }\n"
		"}"
		))
	{
		for( auto t :  test2.getTable("index") )
			std::cout << "float value: " << 
			t.second->get<float>() << std::endl;

		Table& tindex=test2.getTable("index");
		for( int i=0;i<tindex.indexUnnomed();++i ){
			if(tindex.exists(i)){
				std::cout << "sicurity float value: " <<  tindex.find(i)->second->get<float>() << std::endl;
			}
		}
	}else
		std::cout << test2.getDeserializeErros();

	Table test3;
	test3.set(Vec2(20,1));
	test3.set(Vec3(1,1,1));
	test3.set(Vec4(4,1,2,3));
	test3.set("M4x4",Matrix4x4());
	test3.set("ciao_mamma","mamma mai!");
	Table& test4=test3.createTable("mario");
	test4.set("_ss_",Vec2(1,1.3));
	test4.set("M4x4",Matrix4x4());
	test3.set(20.2);
	std::cout << "\n\n\n" << test3.serialize();
*/

	return 0;
}