#ifndef GETIRRLICHT_H
#define GETIRRLICHT_H

#include "irrlicht.h"
#include "MastEventReceiver.h"
#include <time.h>

using irr::IrrlichtDevice;
using irr::video::IVideoDriver;
using irr::scene::ISceneManager;
using irr::scene::ICameraSceneNode;
using irr::gui::IGUIEnvironment;


class GetIrrlicht
{
	public:
		static IrrlichtDevice* Device();
		static IVideoDriver* Driver();
		static ISceneManager* Smgr();
		static IGUIEnvironment* GuiEnv();
		static ICameraSceneNode* Camera();
		static MastEventReceiver* Events();
		static int GetRandomInt(int min, int max);
		static void Setup(IrrlichtDevice* theDevice, IVideoDriver* theDriver, ISceneManager* theSmgr, IGUIEnvironment* theGuiEnv, ICameraSceneNode* theCamera, MastEventReceiver* eventReceiver);
	protected:
	private:
		GetIrrlicht() { };
		virtual ~GetIrrlicht() { };
		static IrrlichtDevice* device;
		static IVideoDriver* driver;
		static ISceneManager* smgr;
		static IGUIEnvironment* guienv;
		static ICameraSceneNode* camera;
		static MastEventReceiver* events;
		
};

#endif // GETIRRLICHT_H
