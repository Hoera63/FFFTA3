/*
The program will show how to use the
basics of the VideoDriver, the GUIEnvironment and the
SceneManager.

To use the engine, we will have to include the header file
irrlicht.h, which can be found in the Irrlicht Engine SDK
directory \include.
*/
#include <irrlicht.h>
#include <iostream>


/*
In the Irrlicht Engine, everything can be found in the namespace
'irr'. So if you want to use a class of the engine, you have to
write an irr:: before the name of the class. For example to use
the IrrlichtDevice write: irr::IrrlichtDevice. To get rid of the
irr:: in front of the name of every class, we tell the compiler
that we use that namespace from now on, and we will not have to
write that 'irr::'.
*/
using namespace irr;

/*
There are 5 sub namespaces in the Irrlicht Engine. Take a look
at them, you can read a detailed description of them in the
documentation by clicking on the top menu item 'Namespace List'
or using this link: http://irrlicht.sourceforge.net/docu/namespaces.html.
Like the irr Namespace, we do not want these 5 sub namespaces now,
to keep this example simple. Hence we tell the compiler again
that we do not want always to write their names:
*/
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#include "Header/GetIrrlicht.h"
#include "Header/BattleField.h"


#include <vector>
using namespace std;

/*
This is the main method. We can use void main() on every platform.
On Windows platforms, we could also use the WinMain method
if we would want to get rid of the console window, which pops up when
starting a program with main(), but to keep this example simple,
we use main().
*/
int main(int argc, char** argv)
{
	
	/*
	The most important function of the engine is the 'createDevice'
	function. The Irrlicht Device can be created with it, which is the
	root object for doing everything with the engine.
	createDevice() has 7 paramters:
	deviceType: Type of the device. This can currently be the Null-device,
	   the Software device, DirectX8, DirectX9, or OpenGL. In this example we use
	   EDT_OPENGL.
	windowSize: Size of the Window or FullscreenMode to be created. In this
	   example we use 640x480.
	bits: Amount of bits per pixel when in fullscreen mode. This should
	   be 16 or 32. This parameter is ignored when running in windowed mode.
	fullscreen: Specifies if we want the device to run in fullscreen mode
	   or not.
	stencilbuffer: Specifies if we want to use the stencil buffer for drawing shadows.
	vsync: Specifies if we want to have vsync enabled, this is only useful in fullscreen
	  mode.
	eventReceiver: An object to receive events. We do not want to use this
	   parameter here, and set it to 0.
	*/
	MastEventReceiver eventReceiver;
	eventReceiver.init();
	/*
	// create a NULL device to detect screen resolution
	IrrlichtDevice *nulldevice = createDevice(video::EDT_NULL);

	core::dimension2d<u32> deskRes = nulldevice->getVideoModeList()->getDesktopResolution();

	nulldevice -> drop();

	// now the dimensions can be used to create the real device
	IrrlichtDevice *device = createDevice(EDT_OPENGL, deskRes, 32, true, false, true, &eventReceiver);*/
	IrrlichtDevice* device = createDevice(EDT_OPENGL, dimension2d<u32>(1280, 720), 32, false, false, true, &eventReceiver);
	
	/*
	Set the caption of the window to some nice text. Note that there is
	a 'L' in front of the string. The Irrlicht Engine uses wide character
	strings when displaying text.
	*/
	device->setWindowCaption(L"FFTA3 \"A Start\" v2 (Even while it still looks like FFTA)");

	/*
	Get a pointer to the video driver, the SceneManager and the
	graphical user interface environment, so that
	we do not always have to write device->getVideoDriver(),
	device->getSceneManager() and device->getGUIEnvironment().
	*/
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();
	ICameraSceneNode* camera = smgr->addCameraSceneNode(0, vector3df(0.f, 0.f, -5), vector3df(0, 0, 1));
	camera->setProjectionMatrix(matrix4().buildProjectionMatrixOrthoLH(1280, 720, -1, 500), true);
	
	
	GetIrrlicht::Setup(device, driver, smgr, guienv, camera, &eventReceiver);
	/*
	video::ITexture* images2 = driver->getTexture("../../Data/FFTA/FFTA Viera Archer.png");
	video::ITexture* images3 = driver->getTexture("../../Data/FFTA/FFTA Aisen Field.png");
	video::ITexture* images4 = driver->getTexture("../../Data/FFTA/FFTA Aisen Field top only.png");
	
	Animation animation;
	vector<u32> vec;
	vec.push_back(0);
	vec.push_back(1);
	vec.push_back(2);
	vec.push_back(1);
	animation.FrameOrder(vec);
	animation.FPS(4.0f);
	animation.FramePosition(vector2df(90, 9));
	animation.FrameSize(vector2df(17, 30));
	animation.Texture(images2);
	animation.Name(L"Walking");
	
	Animation animation2;
	animation2.FrameOrder(vec);
	animation2.FPS(4.0f);
	animation2.FramePosition(vector2df(89, 42));
	animation2.FrameSize(vector2df(17, 30));
	animation2.Texture(images2);
	animation2.Name(L"BackWalking");
	
	Animation BFAnimation;
	vector<u32> vec3;
	vec3.push_back(0);
	BFAnimation.FrameOrder(vec3);
	BFAnimation.FPS(0.0f);
	BFAnimation.FramePosition(vector2df(0, 0));
	BFAnimation.FrameSize(vector2df(474, 273));
	BFAnimation.Texture(images3);
	BFAnimation.Name(L"Aisen Field");
	
	Animation BFO1Animation;
	BFO1Animation.FrameOrder(vec3);
	BFO1Animation.FPS(0.0f);
	BFO1Animation.FramePosition(vector2df(0, 179));
	BFO1Animation.FrameSize(vector2df(474, 32));
	BFO1Animation.Texture(images4);
	BFO1Animation.Name(L"Aisen Field overlay 1"); // 2.5
	
	Animation BFO2Animation;
	BFO2Animation.FrameOrder(vec3);
	BFO2Animation.FPS(0.0f);
	BFO2Animation.FramePosition(vector2df(0, 211));
	BFO2Animation.FrameSize(vector2df(474, 62));
	BFO2Animation.Texture(images4);
	BFO2Animation.Name(L"Aisen Field overlay 2"); // 7
	
	Animation BFO3Animation;
	BFO3Animation.FrameOrder(vec3);
	BFO3Animation.FPS(0.0f);
	BFO3Animation.FramePosition(vector2df(0, 157));
	BFO3Animation.FrameSize(vector2df(474, 22));
	BFO3Animation.Texture(images4);
	BFO3Animation.Name(L"Aisen Field overlay 3"); // 9
	
	Animation BFO4Animation;
	BFO4Animation.FrameOrder(vec3);
	BFO4Animation.FPS(0.0f);
	BFO4Animation.FramePosition(vector2df(0, 131));
	BFO4Animation.FrameSize(vector2df(474, 26));
	BFO4Animation.Texture(images4);
	BFO4Animation.Name(L"Aisen Field overlay 4"); // 12.5
	
	Animation BFO5Animation;
	BFO5Animation.FrameOrder(vec3);
	BFO5Animation.FPS(0.0f);
	BFO5Animation.FramePosition(vector2df(69, 0));
	BFO5Animation.FrameSize(vector2df(18, 273));
	BFO5Animation.Texture(images4);
	BFO5Animation.Name(L"Aisen Field overlay 5"); // 17.5
	
	Animation BFO6Animation;
	BFO6Animation.FrameOrder(vec3);
	BFO6Animation.FPS(0.0f);
	BFO6Animation.FramePosition(vector2df(0, 0));
	BFO6Animation.FrameSize(vector2df(90, 273));
	BFO6Animation.Texture(images4);
	BFO6Animation.Name(L"Aisen Field overlay 6"); // 18.5
	
	Animation BFO7Animation;
	BFO7Animation.FrameOrder(vec3);
	BFO7Animation.FPS(0.0f);
	BFO7Animation.FramePosition(vector2df(130, 77));
	BFO7Animation.FrameSize(vector2df(25, 25));
	BFO7Animation.Texture(images4);
	BFO7Animation.Name(L"Aisen Field overlay 7"); // 20.5
	
	Animation BFO8Animation;
	BFO8Animation.FrameOrder(vec3);
	BFO8Animation.FPS(0.0f);
	BFO8Animation.FramePosition(vector2df(241, 103));
	BFO8Animation.FrameSize(vector2df(25, 25));
	BFO8Animation.Texture(images4);
	BFO8Animation.Name(L"Aisen Field overlay 8"); // 16.5
	
	Animation BFO9Animation;
	BFO9Animation.FrameOrder(vec3);
	BFO9Animation.FPS(0.0f);
	BFO9Animation.FramePosition(vector2df(226, 71));
	BFO9Animation.FrameSize(vector2df(25, 25));
	BFO9Animation.Texture(images4);
	BFO9Animation.Name(L"Aisen Field overlay 9"); // 19.5
	
	Animation BFOAAnimation;
	BFOAAnimation.FrameOrder(vec3);
	BFOAAnimation.FPS(0.0f);
	BFOAAnimation.FramePosition(vector2df(307, 86));
	BFOAAnimation.FrameSize(vector2df(40, 25));
	BFOAAnimation.Texture(images4);
	BFOAAnimation.Name(L"Aisen Field overlay A"); // 16.5
	
	Animation BFOBAnimation;
	BFOBAnimation.FrameOrder(vec3);
	BFOBAnimation.FPS(0.0f);
	BFOBAnimation.FramePosition(vector2df(0, 52));
	BFOBAnimation.FrameSize(vector2df(474, 20));
	BFOBAnimation.Texture(images4);
	BFOBAnimation.Name(L"Aisen Field overlay B"); // 20.5
	
	Animation BFOCAnimation;
	BFOCAnimation.FrameOrder(vec3);
	BFOCAnimation.FPS(0.0f);
	BFOCAnimation.FramePosition(vector2df(357, 0));
	BFOCAnimation.FrameSize(vector2df(17, 273));
	BFOCAnimation.Texture(images4);
	BFOCAnimation.Name(L"Aisen Field overlay C"); // 15.5
	
	Animation BFODAnimation;
	BFODAnimation.FrameOrder(vec3);
	BFODAnimation.FPS(0.0f);
	BFODAnimation.FramePosition(vector2df(357, 0));
	BFODAnimation.FrameSize(vector2df(50, 273));
	BFODAnimation.Texture(images4);
	BFODAnimation.Name(L"Aisen Field overlay D"); // 16.5
	
	Animation BFOEAnimation;
	BFOEAnimation.FrameOrder(vec3);
	BFOEAnimation.FPS(0.0f);
	BFOEAnimation.FramePosition(vector2df(0, 0));
	BFOEAnimation.FrameSize(vector2df(474, 35));
	BFOEAnimation.Texture(images4);
	BFOEAnimation.Name(L"Aisen Field overlay E"); // 25
	
	
	
	Unit testVieraArcher;
	testVieraArcher.Animations().push_back(animation);
	testVieraArcher.Animations().push_back(animation2);
	testVieraArcher.PlayAnimation(L"Walking");
	testVieraArcher.Name(L"Lilila");
	testVieraArcher.BFPosition(BattleFieldPosition(2,0,1));
	
	Unit testVieraArcher2;
	testVieraArcher2.Animations().push_back(animation2);
	testVieraArcher2.PlayAnimation(0, true);
	testVieraArcher2.Name(L"Miriam");
	testVieraArcher2.BFPosition(BattleFieldPosition(7,7,0));
	
	BattleField aisenfield;
	vector< vector<int> > heightMap;
	{
	vector<int> row;          // 0
	row.push_back(1);
	row.push_back(1);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(1);
	row.push_back(1);
	row.push_back(0);
	heightMap.push_back(row); // 1
	row.clear();
	row.push_back(2);
	row.push_back(1);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(1);
	row.push_back(0);
	heightMap.push_back(row); // 2
	row.clear();
	row.push_back(3);
	row.push_back(2);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(1);
	heightMap.push_back(row); // 3
	row.clear();
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	heightMap.push_back(row); // 4
	row.clear();
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	heightMap.push_back(row); // 5
	row.clear();
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	heightMap.push_back(row); // 6
	row.clear();
	row.push_back(2);
	row.push_back(0);
	row.push_back(1);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	heightMap.push_back(row); // 7
	row.clear();
	row.push_back(2);
	row.push_back(2);
	row.push_back(2);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	heightMap.push_back(row); // 8
	row.clear();
	row.push_back(2);
	row.push_back(2);
	row.push_back(2);
	row.push_back(1);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(1);
	row.push_back(1);
	row.push_back(0);
	row.push_back(0);
	row.push_back(1);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	heightMap.push_back(row); // 9
	row.clear();
	row.push_back(3);
	row.push_back(3);
	row.push_back(2);
	row.push_back(1);
	row.push_back(1);
	row.push_back(2);
	row.push_back(2);
	row.push_back(2);
	row.push_back(2);
	row.push_back(2);
	row.push_back(2);
	row.push_back(2);
	row.push_back(2);
	row.push_back(1);
	row.push_back(0);
	row.push_back(0);
	heightMap.push_back(row); // 10
	row.clear();
	row.push_back(3);
	row.push_back(3);
	row.push_back(3);
	row.push_back(2);
	row.push_back(2);
	row.push_back(2);
	row.push_back(2);
	row.push_back(2);
	row.push_back(2);
	row.push_back(3);
	row.push_back(2);
	row.push_back(2);
	row.push_back(2);
	row.push_back(2);
	row.push_back(2);
	row.push_back(0);
	heightMap.push_back(row); // 11
	row.clear();
	row.push_back(3);
	row.push_back(3);
	row.push_back(3);
	row.push_back(3);
	row.push_back(3);
	row.push_back(2);
	row.push_back(3);
	row.push_back(2);
	row.push_back(2);
	row.push_back(2);
	row.push_back(2);
	row.push_back(3);
	row.push_back(2);
	row.push_back(2);
	row.push_back(2);
	row.push_back(2);
	heightMap.push_back(row); // 12
	row.clear();
	row.push_back(3);
	row.push_back(3);
	row.push_back(3);
	row.push_back(3);
	row.push_back(3);
	row.push_back(2);
	row.push_back(2);
	row.push_back(3);
	row.push_back(2);
	row.push_back(2);
	row.push_back(2);
	row.push_back(4);
	row.push_back(3);
	row.push_back(3);
	row.push_back(2);
	row.push_back(2);
	heightMap.push_back(row);
	}
	aisenfield.Init(vector3di(32,16,8), vector2di(263,251), BFAnimation, heightMap, vector2di(1422, 819)); // Scaled 3 times
	//aisenfield.Init(vector3di(32,16,8), vector2di(263,251), BFAnimation, heightMap, vector2di(474, 273)); // Unscaled
	aisenfield.AddUnit(testVieraArcher);
	aisenfield.AddUnit(testVieraArcher2);
	aisenfield.AddOverlay(BFO1Animation, 7.0f);
	aisenfield.AddOverlay(BFO2Animation, 2.5f);
	aisenfield.AddOverlay(BFO3Animation, 9.0f);
	aisenfield.AddOverlay(BFO4Animation, 12.5f);
	aisenfield.AddOverlay(BFO5Animation, 16.5f);
	aisenfield.AddOverlay(BFO6Animation, 17.5f);
	aisenfield.AddOverlay(BFO7Animation, 19.5f);
	aisenfield.AddOverlay(BFO8Animation, 16.5f);
	aisenfield.AddOverlay(BFO9Animation, 19.5f);
	aisenfield.AddOverlay(BFOAAnimation, 16.5f);
	aisenfield.AddOverlay(BFOBAnimation, 20.5f);
	aisenfield.AddOverlay(BFOCAnimation, 15.5f);
	aisenfield.AddOverlay(BFODAnimation, 16.5f);
	aisenfield.AddOverlay(BFOEAnimation, 25.0f);
	aisenfield.Name(L"Example Battlefield");
	irr::io::IXMLWriter* xwriter = device->getFileSystem()->createXMLWriter(L"Test XML.xml");
	if (!xwriter)
            ;
	else
	{
		xwriter->writeXMLHeader();
		aisenfield.WriteXML(xwriter);
		xwriter->drop();
	}
	*/
	BattleField aisenfield;
	irr::io::IXMLReader* xreader = device->getFileSystem()->createXMLReader(L"Test XML.xml");
	if (!xreader)
            ;
	else
	{
		while(xreader->read())
		{
			switch (xreader->getNodeType())
			{
				case irr::io::EXN_ELEMENT:
				{
					if (BattleFieldTag.equals_ignore_case(xreader->getNodeName()))
					{
						aisenfield.ReadXML(xreader);
					}
				}
				default:
					break;
			}
		}
		xreader->drop();
	}
	
	/*
	Ok, now we have set up the scene, lets draw everything:
	We run the device in a while() loop, until the device does not
	want to run any more. This would be when the user closed the window
	or pressed ALT+F4 in windows.
	*/
	u32 then = device->getTimer()->getTime();
	u32 now = 0;
	f32 deltaTime = 0;
	dimension2du screenSize = driver->getScreenSize();
	while(device->run())
	{
		eventReceiver.endEventProcess();
		// Mouse Movement
		//cout << "Mouse X " << eventReceiver.mouseX() << ", y " << eventReceiver.mouseY() << "\n";
		if (eventReceiver.leftMouseDown())
		{
			if (eventReceiver.mouseX() < screenSize.Width / 5.0f)
			{
				aisenfield.Sprite().Node()->setPosition(vector3df(min(aisenfield.Sprite().Node()->getPosition().X + (screenSize.Width / 5.0f - eventReceiver.mouseX()) / 10, 700.0f),
																	 aisenfield.Sprite().Node()->getPosition().Y,
																	 aisenfield.Sprite().Node()->getPosition().Z));
			} else if (eventReceiver.mouseX() > screenSize.Width * 0.8f)
			{
				aisenfield.Sprite().Node()->setPosition(vector3df(max(aisenfield.Sprite().Node()->getPosition().X - (eventReceiver.mouseX() - screenSize.Width * 0.8f) / 10, -700.0f),
																	 aisenfield.Sprite().Node()->getPosition().Y,
																	 aisenfield.Sprite().Node()->getPosition().Z));
			}
			if (eventReceiver.mouseY() < screenSize.Height / 5.0f)
			{
				aisenfield.Sprite().Node()->setPosition(vector3df(aisenfield.Sprite().Node()->getPosition().X,
																	 max(aisenfield.Sprite().Node()->getPosition().Y - (screenSize.Height / 5.0f - eventReceiver.mouseY()) / 10, -620.0f),
																	 aisenfield.Sprite().Node()->getPosition().Z));
			} else if (eventReceiver.mouseY() > screenSize.Height * 0.8f)
			{
				aisenfield.Sprite().Node()->setPosition(vector3df(aisenfield.Sprite().Node()->getPosition().X,
																	 min(aisenfield.Sprite().Node()->getPosition().Y + (eventReceiver.mouseY() - screenSize.Height * 0.8f) / 10, 100.0f),
																	 aisenfield.Sprite().Node()->getPosition().Z));
			}
		}
		//cout << "X " << aisenfield.Sprite().Node()->getPosition().X << ", Y " << aisenfield.Sprite().Node()->getPosition().Y << ", Z " << aisenfield.Sprite().Node()->getPosition().Z << "\n";
		//cout << "Selector X " << aisenfield.BFSelector().Sprite().Node()->getPosition().X << ",Selector  Y " << aisenfield.BFSelector().Sprite().Node()->getPosition().Y << ",Selector  Z " << aisenfield.BFSelector().Sprite().Node()->getPosition().Z << "\n";
		/*if (eventReceiver.keyPressed(KEY_KEY_A))
		{
			aisenfield.Units()[0].BFPosition(BattleFieldPosition(aisenfield.Units()[0].BFPosition().X(),
																	 aisenfield.Units()[0].BFPosition().Y() + 1.0f,
																	 aisenfield.GetHeightByPos(round_(aisenfield.Units()[0].BFPosition().X()), round_(aisenfield.Units()[0].BFPosition().Y() + 1))));
			cout << "BF Position, x = " << aisenfield.Units()[0].BFPosition().X() << ", y = " << aisenfield.Units()[0].BFPosition().Y() << ", z = " << aisenfield.Units()[0].BFPosition().Z() << " Real Z = " << aisenfield.Units()[0].Sprite().Node()->getPosition().Z <<"\n";
			aisenfield.Units()[0].PlayAnimation(L"BackWalking", false);
		}
		if (eventReceiver.keyPressed(KEY_KEY_D))
		{
			aisenfield.Units()[0].BFPosition(BattleFieldPosition(aisenfield.Units()[0].BFPosition().X(),
																	 aisenfield.Units()[0].BFPosition().Y() - 1.0f,
																	 aisenfield.GetHeightByPos(round_(aisenfield.Units()[0].BFPosition().X()), round_(aisenfield.Units()[0].BFPosition().Y() - 1))));
			cout << "BF Position, x = " << aisenfield.Units()[0].BFPosition().X() << ", y = " << aisenfield.Units()[0].BFPosition().Y() << ", z = " << aisenfield.Units()[0].BFPosition().Z() << " Real Z = " << aisenfield.Units()[0].Sprite().Node()->getPosition().Z <<"\n";
			aisenfield.Units()[0].PlayAnimation(L"Walking", true);
		}
		if (eventReceiver.keyPressed(KEY_KEY_W))
		{
			aisenfield.Units()[0].BFPosition(BattleFieldPosition(aisenfield.Units()[0].BFPosition().X() + 1.0f,
																	 aisenfield.Units()[0].BFPosition().Y(),
																	 aisenfield.GetHeightByPos(round_(aisenfield.Units()[0].BFPosition().X() + 1.0f), round_(aisenfield.Units()[0].BFPosition().Y()))));
			cout << "BF Position, x = " << aisenfield.Units()[0].BFPosition().X() << ", y = " << aisenfield.Units()[0].BFPosition().Y() << ", z = " << aisenfield.Units()[0].BFPosition().Z() << " Real Z = " << aisenfield.Units()[0].Sprite().Node()->getPosition().Z <<"\n";
			aisenfield.Units()[0].PlayAnimation(L"BackWalking", true);
		}
		if (eventReceiver.keyPressed(KEY_KEY_S))
		{
			aisenfield.Units()[0].BFPosition(BattleFieldPosition(aisenfield.Units()[0].BFPosition().X() - 1.0f,
																	 aisenfield.Units()[0].BFPosition().Y(),
																	 aisenfield.GetHeightByPos(round_(aisenfield.Units()[0].BFPosition().X() - 1.0f), round_(aisenfield.Units()[0].BFPosition().Y()))));
			cout << "BF Position, x = " << aisenfield.Units()[0].BFPosition().X() << ", y = " << aisenfield.Units()[0].BFPosition().Y() << ", z = " << aisenfield.Units()[0].BFPosition().Z() << " Real Z = " << aisenfield.Units()[0].Sprite().Node()->getPosition().Z <<"\n";
			aisenfield.Units()[0].PlayAnimation(L"Walking", false);
		}
		aisenfield.BFSelector().BFPosition(BattleFieldPosition(aisenfield.Units()[0].BFPosition()));
		*/
		// Work out a frame delta time.
		now = device->getTimer()->getTime();
	   	deltaTime = static_cast<float>(now - then) / 1000.f; // Time in seconds
	   	then = now;
		/*
		Anything can be drawn between a beginScene() and an endScene()
		call. The beginScene clears the screen with a color and also the
		depth buffer if wanted. Then we let the Scene Manager and the
		GUI Environment draw their content. With the endScene() call
		everything is presented on the screen.
		*/
		aisenfield.Update(deltaTime);
		driver->beginScene(true, true, SColor(0, 200, 200, 200));
		smgr->drawAll();
		guienv->drawAll();
		driver->endScene();
		
		eventReceiver.startEventProcess();
	}
	device->drop();

	return 0;
}
