#include "../Header/Editor.h"

Editor::Editor()
{
	//ctor
}

Editor::~Editor()
{
	//dtor
}

void Editor::Init()
{
	m_SubMenuParent = NULL;
	m_SubSubMenuParent = NULL;
	m_SubSubSubMenuParent = NULL;
	m_Loading = false;
	GetIrrlicht::GuiEnv()->addButton(rect<s32>(0, 0, 100, 32), 0, FILE_BUTTON_ID, L"File", L"Open files and stuff.");
	GetIrrlicht::GuiEnv()->addButton(rect<s32>(100, 0, 200, 32), 0, RACE_EDITOR_BUTTON_ID, L"Race Editor", L"Edit or create Races.");
	GetIrrlicht::GuiEnv()->addButton(rect<s32>(200, 0, 300, 32), 0, JOB_EDITOR_BUTTON_ID, L"Job Editor", L"Edit or create Jobs.");
	GetIrrlicht::GuiEnv()->addButton(rect<s32>(300, 0, 400, 32), 0, BATTLEFIELD_EDITOR_BUTTON_ID, L"Battlefield Editor", L"Edit or create Battlefields.");
    
}

void Editor::Update(float deltaTime)
{
	
}

void Editor::ButtonClicked(int id)
{
	if (m_Loading) return;
	switch(id)
    {
		case FILE_BUTTON_ID:
		{
			IGUIContextMenu* contextMenu = GetIrrlicht::GuiEnv()->addContextMenu(rect<s32>(0,32,100,32), 0, FILE_MENU_ID);
			contextMenu->addItem(L"New Project...", NEW_PROJECT_BUTTON_ID);
			contextMenu->addItem(L"Open Project", OPEN_PROJECT_BUTTON_ID);
			contextMenu->addItem(L"Save Project", SAVE_PROJECT_BUTTON_ID);
			contextMenu->addItem(L"Save Project as...", SAVE_PROJECT_AS_BUTTON_ID);
			contextMenu->addItem(L"Exit", EXIT_BUTTON_ID);
			break;
		}
		case RACE_EDITOR_BUTTON_ID:
		{
			if (m_SubMenuParent)
			{
				m_SubMenuParent->remove();
				m_SubSubMenuParent = NULL;
			}
			m_SubMenuParent = GetIrrlicht::GuiEnv()->addStaticText(L"", rect<s32>(0, 32, 1280, 720), true, true, 0);
			GetIrrlicht::GuiEnv()->addButton(rect<s32>(0, 50, 100, 82), m_SubMenuParent, NEW_RACE_BUTTON_ID, L"New Race...", L"Add a new Race to the Project.");
			for (unsigned int i = 0; i < m_Project.RaceList()->size(); ++i)
			//for (vector<Job>::iterator i = m_Project.JobList()->begin(); i != m_Project.JobList()->end(); ++i)
			{
				GetIrrlicht::GuiEnv()->addButton(rect<s32>(0, i * 32 + 82, 100, i * 32 + 114), m_SubMenuParent, 1000 + i, (*m_Project.RaceList())[i].Name().c_str(), L"Edit this Race.");
			}
			break;
		}
		case JOB_EDITOR_BUTTON_ID:
		{
			if (m_SubMenuParent)
			{
				m_SubMenuParent->remove();
				m_SubSubMenuParent = NULL;
			}
			m_SubMenuParent = GetIrrlicht::GuiEnv()->addStaticText(L"", rect<s32>(0, 32, 1280, 720), true, true, 0);
			break;
		}
		case BATTLEFIELD_EDITOR_BUTTON_ID:
		{
			if (m_SubMenuParent)
			{
				m_SubMenuParent->remove();
				m_SubSubMenuParent = NULL;
			}
			m_SubMenuParent = GetIrrlicht::GuiEnv()->addStaticText(L"", rect<s32>(0, 32, 1280, 720), true, true, 0);
			break;
		}
		// Race Editor Buttons
		case NEW_RACE_BUTTON_ID:
		{
			if (m_SubSubMenuParent)
			{
				m_SubSubMenuParent->remove();
			}
			m_SubSubMenuParent = GetIrrlicht::GuiEnv()->addStaticText(L"", rect<s32>(490, 220, 720, 320), true, true, m_SubMenuParent, true);
			GetIrrlicht::GuiEnv()->addEditBox(L"", rect<s32>(10, 10, 210, 42), true, m_SubSubMenuParent, NEW_RACE_EDITBOX_ID);
			GetIrrlicht::GuiEnv()->addButton(rect<s32>(8, 50, 108, 82), m_SubSubMenuParent, NEW_RACE_OK_BUTTON_ID, L"OK", L"Add the new Race.");
			GetIrrlicht::GuiEnv()->addButton(rect<s32>(116, 50, 216, 82), m_SubSubMenuParent, NEW_RACE_CANCEL_BUTTON_ID, L"Cancel", L"Cancels the creation of the new Race.");
			GetIrrlicht::GuiEnv()->setFocus(m_SubSubMenuParent->getElementFromId(NEW_RACE_EDITBOX_ID));
			break;
		}
		case NEW_RACE_OK_BUTTON_ID:
		{
			m_Project.RaceList()->push_back(Race());
			(m_Project.RaceList()->end() - 1)->Name(m_SubSubMenuParent->getElementFromId(NEW_RACE_EDITBOX_ID)->getText());
			if (m_SubSubMenuParent)
			{
				m_SubSubMenuParent->remove();
				m_SubSubMenuParent = NULL;
			}
			ButtonClicked(RACE_EDITOR_BUTTON_ID);
			break;
		}
		case NEW_RACE_CANCEL_BUTTON_ID:
		{
			if (m_SubSubMenuParent)
			{
				m_SubSubMenuParent->remove();
				m_SubSubMenuParent = NULL;
			}
			break;
		}
		case NAMELIST_BUTTON_ID:
		{
			if (m_SubSubSubMenuParent)
			{
				m_SubSubSubMenuParent->remove();
			}
			m_SubSubSubMenuParent = GetIrrlicht::GuiEnv()->addStaticText(L"Name List:", rect<s32>(110, 5, 1080, 720), false, true, m_SubSubMenuParent, false);
			GetIrrlicht::GuiEnv()->addButton(rect<s32>(0, 18, 100, 50), m_SubSubSubMenuParent, NEW_NAME_BUTTON_ID, L"Add Name...", L"Add a standard name for this Race.");
			for (unsigned int i = 0; i < m_CurrentRace->NameList()->size(); ++i)
			{
				GetIrrlicht::GuiEnv()->addStaticText((*m_CurrentRace->NameList())[i].c_str(), rect<s32>(0, 53 + i * 20, 100, 70 + i * 20), false, true, m_SubSubSubMenuParent, false);
				GetIrrlicht::GuiEnv()->addButton(rect<s32>(100, 50 + i * 20, 110, 70 + i * 20), m_SubSubSubMenuParent, 2000 + i, L"X", L"Remove this name.");
			}
			break;
		}
		case NEW_NAME_BUTTON_ID:
		{
			IGUIElement* enterNameBox = GetIrrlicht::GuiEnv()->addStaticText(L"", rect<s32>(150, 120, 380, 220), true, true, m_SubSubSubMenuParent, true);
			GetIrrlicht::GuiEnv()->addEditBox(L"", rect<s32>(10, 10, 210, 42), true, enterNameBox, NEW_NAME_EDITBOX_ID);
			GetIrrlicht::GuiEnv()->addButton(rect<s32>(8, 50, 108, 82), enterNameBox, NEW_NAME_OK_BUTTON_ID, L"OK", L"Add the new name.");
			GetIrrlicht::GuiEnv()->addButton(rect<s32>(116, 50, 216, 82), enterNameBox, NEW_NAME_CANCEL_BUTTON_ID, L"Cancel", L"Cancels the new name.");
			GetIrrlicht::GuiEnv()->setFocus(enterNameBox->getElementFromId(NEW_NAME_EDITBOX_ID));
			break;
		}
		case NEW_NAME_OK_BUTTON_ID:
		{
			m_CurrentRace->NameList()->push_back(stringw(m_SubSubSubMenuParent->getElementFromId(NEW_NAME_EDITBOX_ID, true)->getText()));
			ButtonClicked(NAMELIST_BUTTON_ID);
			break;
		}
		case NEW_NAME_CANCEL_BUTTON_ID:
		{
			ButtonClicked(NAMELIST_BUTTON_ID);
			break;
		}
		case JOBLIST_BUTTON_ID:
		{
			if (m_SubSubSubMenuParent)
			{
				m_SubSubSubMenuParent->remove();
			}
			m_SubSubSubMenuParent = GetIrrlicht::GuiEnv()->addStaticText(L"Job List:", rect<s32>(110, 5, 1080, 720), false, true, m_SubSubMenuParent, false);
			GetIrrlicht::GuiEnv()->addButton(rect<s32>(0, 18, 100, 50), m_SubSubSubMenuParent, ADD_JOB_BUTTON_ID, L"Add Job...", L"Add a Job to this race.");
			unsigned int j = 0;
			for (unsigned int i = 0; i < m_Project.JobRaceList()->size(); ++i)
			{
				if ((*m_Project.JobRaceList())[i].GetRace() == m_CurrentRace)
				{
					GetIrrlicht::GuiEnv()->addStaticText((*m_Project.JobRaceList())[i].GetJob()->Name().c_str(), rect<s32>(0, 53 + j * 20, 100, 70 + j * 20), false, true, m_SubSubSubMenuParent, false);
					//GetIrrlicht::GuiEnv()->addStaticText((*m_CurrentRace->NameList())[i].c_str(), rect<s32>(0, 53 + i * 20, 100, 70 + i * 20), false, true, m_SubSubSubMenuParent, false);
					GetIrrlicht::GuiEnv()->addButton(rect<s32>(100, 50 + j * 20, 110, 70 + j * 20), m_SubSubSubMenuParent, 2000 + i, L"X", L"Remove this name.");
					++j;
				}
			}
			break;
		}
		default:
			if (id >= 1000 && id < 2000) // Race Buttons
			{
				m_CurrentRace = &(*m_Project.RaceList())[id - 1000];
				if (m_SubSubMenuParent)
				{
					m_SubSubMenuParent->remove();
				}
				m_SubSubMenuParent = GetIrrlicht::GuiEnv()->addStaticText(L"", rect<s32>(200, 32, 1280, 720), true, true, m_SubMenuParent, true);
				m_SubSubMenuParent->setBackgroundColor(SColor(255, 220, 220, 220));
				GetIrrlicht::GuiEnv()->addButton(rect<s32>(0, 50, 100, 82), m_SubSubMenuParent, NAMELIST_BUTTON_ID, L"Name List", L"Edit the Name List of this Race.");
				GetIrrlicht::GuiEnv()->addButton(rect<s32>(0, 82, 100, 114), m_SubSubMenuParent, JOBLIST_BUTTON_ID, L"Job List", L"Edit the Job List of this Race.");
				GetIrrlicht::GuiEnv()->addStaticText(m_CurrentRace->Name().c_str(), rect<s32>(400, 6, 500, 32), false, true, m_SubSubMenuParent);
			} else if (id >= 2000 && id < 3000) // Race Name Removing Buttons
			{
				m_CurrentRace->NameList()->erase(m_CurrentRace->NameList()->begin() + (id - 2000));
				ButtonClicked(NAMELIST_BUTTON_ID);
			}
			break;
	}
}

void Editor::ContextMenuClicked(int id, int item)
{
	if (m_Loading) return;
	switch(id)
    {
		case FILE_MENU_ID:
		{
			switch(item)
			{
				case NEW_PROJECT_BUTTON_ID:
				{
					break;
				}
				case OPEN_PROJECT_BUTTON_ID:
				{
					GetIrrlicht::GuiEnv()->addFileOpenDialog(L"Please open a xml project file.", true, 0, OPEN_PROJECT_WINDOW_ID, false);
					break;
				}
				case SAVE_PROJECT_BUTTON_ID:
				{
					break;
				}
				case SAVE_PROJECT_AS_BUTTON_ID:
				{
					break;
				}
				case EXIT_BUTTON_ID:
				{
					GetIrrlicht::Device()->closeDevice();
					break;
				}
				default:
					break;
			}
		default:
			break;
		}
	}
}

void Editor::FileOpened(int id, stringw fileName)
{
	switch(id)
	{
		case OPEN_PROJECT_WINDOW_ID:
		{
			//GetIrrlicht::GuiEnv()->addButton(rect<s32>(500, 300, 600, 332), 0, FILE_BUTTON_ID, L"relativename", fileName.c_str());
			if (m_SubMenuParent)
			{
				m_SubMenuParent->remove();
				m_SubMenuParent = NULL;
			}
			m_ProjectFile = fileName;
			RefreshProject();
		}
		default:
			break;
	}
}

void Editor::RefreshProject()
{
	m_Loading = true;
	IXMLReader* xml = GetIrrlicht::Device()->getFileSystem()->createXMLReader(m_ProjectFile.c_str());
	if (!xml)
			;
	else
	{
		while(xml->read())
		{
			switch (xml->getNodeType())
			{
				case irr::io::EXN_ELEMENT:
				{
					if (ProjectTag.equals_ignore_case(xml->getNodeName()))
					{
						m_Project.ReadXML(xml);
					}
				}
				default:
					break;
			}
		}
		xml->drop();
		m_Project.LoadData();
	}
	m_Loading = false;
}
