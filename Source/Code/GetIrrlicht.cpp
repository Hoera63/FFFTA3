#include "../Header/GetIrrlicht.h"

IrrlichtDevice* GetIrrlicht::device = NULL;
IVideoDriver* GetIrrlicht::driver = NULL;
ISceneManager* GetIrrlicht::smgr = NULL;
IGUIEnvironment* GetIrrlicht::guienv = NULL;
ICameraSceneNode* GetIrrlicht::camera = NULL;
MastEventReceiver* GetIrrlicht::events = NULL;

IrrlichtDevice* GetIrrlicht::Device()
{
	return device;
}

IVideoDriver* GetIrrlicht::Driver()
{
	return driver;
}

ISceneManager* GetIrrlicht::Smgr()
{
	return smgr;
}

IGUIEnvironment* GetIrrlicht::GuiEnv()
{
	return guienv;
}

ICameraSceneNode* GetIrrlicht::Camera()
{
	return camera;
}

MastEventReceiver* GetIrrlicht::Events()
{
	return events;
}

int GetIrrlicht::GetRandomInt(int min, int max)
{
	int n = max - min + 1;
	int remainder = RAND_MAX % n;
	int x;
	do
	{
		x = rand();
	} while (x >= RAND_MAX - remainder);
	return min + x % n;
}

void GetIrrlicht::Setup(IrrlichtDevice* theDevice, IVideoDriver* theDriver, ISceneManager* theSmgr, IGUIEnvironment* theGuiEnv, ICameraSceneNode* theCamera, MastEventReceiver* eventReceiver)
{
	device = theDevice;
	driver = theDriver;
	smgr = theSmgr;
	guienv = theGuiEnv;
	camera = theCamera;
	events = eventReceiver;
	srand(time(NULL));
}
