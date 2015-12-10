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

#include "../Editor Source/Header/Editor.h"
#include "../Editor Source/Header/GuiEventReceiver.h"

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
	GuiEventReceiver guiEventReceiver;
	MastEventReceiver eventReceiver;
	eventReceiver.init();
	guiEventReceiver.SetMastEventReceiver(&eventReceiver);
	/*
	// create a NULL device to detect screen resolution
	IrrlichtDevice *nulldevice = createDevice(video::EDT_NULL);

	core::dimension2d<u32> deskRes = nulldevice->getVideoModeList()->getDesktopResolution();

	nulldevice -> drop();

	// now the dimensions can be used to create the real device
	IrrlichtDevice *device = createDevice(EDT_OPENGL, deskRes, 32, true, false, true, &eventReceiver);*/
	IrrlichtDevice* device = createDevice(EDT_OPENGL, dimension2d<u32>(1280, 720), 32, false, false, true, &guiEventReceiver);
	
	/*
	Set the caption of the window to some nice text. Note that there is
	a 'L' in front of the string. The Irrlicht Engine uses wide character
	strings when displaying text.
	*/
	device->setWindowCaption(L"FFTA3 Editor");

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
	*/
	
	Editor editor;
	editor.Init();
	guiEventReceiver.SetEditor(&editor);
	/*
	Ok, now we have set up the scene, lets draw everything:
	We run the device in a while() loop, until the device does not
	want to run any more. This would be when the user closed the window
	or pressed ALT+F4 in windows.
	*/
	u32 then = device->getTimer()->getTime();
	u32 now = 0;
	f32 deltaTime = 0;
	//dimension2du screenSize = driver->getScreenSize();
	while(device->run())
	{
		eventReceiver.endEventProcess();
		// Mouse Movement
		//cout << "Mouse X " << eventReceiver.mouseX() << ", y " << eventReceiver.mouseY() << "\n";
		/*
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
		editor.Update(deltaTime);
		//aisenfield.Update(deltaTime);
		driver->beginScene(true, true, SColor(0, 200, 200, 200));
		smgr->drawAll();
		guienv->drawAll();
		driver->endScene();
		
		eventReceiver.startEventProcess();
	}
	device->drop();

	return 0;
}
