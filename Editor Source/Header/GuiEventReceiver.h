#include <irrlicht.h>

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#include "Editor.h"

class GuiEventReceiver : public IEventReceiver
{
private: 
	IEventReceiver* m_MastEventReceiver = NULL;
	Editor* m_Editor = NULL;
public:
	virtual bool OnEvent(const SEvent& event)
	{
		bool eventProcessed = false;
		//////////////////////////////
		// Gui Input Event
		//////////////////////////////
		if (event.EventType == EET_GUI_EVENT)
		{
			int id = event.GUIEvent.Caller->getID();
			switch(event.GUIEvent.EventType)
            {
            	case EGET_BUTTON_CLICKED:
            		m_Editor->ButtonClicked(id);
            		break;
				case EGET_MENU_ITEM_SELECTED:
				{
					IGUIContextMenu* menu = static_cast<IGUIContextMenu*>(event.GUIEvent.Caller);
					m_Editor->ContextMenuClicked(id, menu->getItemCommandId(menu->getSelectedItem()));
					break;
				}
				case EGET_FILE_SELECTED:
				{
					IGUIFileOpenDialog* dialog = static_cast<IGUIFileOpenDialog*>(event.GUIEvent.Caller);
					m_Editor->FileOpened(id, stringw(GetIrrlicht::Device()->getFileSystem()->getRelativeFilename(dialog->getFileName(), GetIrrlicht::Device()->getFileSystem()->getWorkingDirectory()).c_str()));
					break;
				}
				default:
					break;
            }
			eventProcessed = true;
		}
		if (!eventProcessed && m_MastEventReceiver)
		{
			eventProcessed = m_MastEventReceiver->OnEvent(event);
		}
		return false;
	}
	
	void SetMastEventReceiver(IEventReceiver* eventReceiver)
	{
		m_MastEventReceiver = eventReceiver;
	}
	
	void SetEditor(Editor* editor)
	{
		m_Editor = editor;
	}
};

