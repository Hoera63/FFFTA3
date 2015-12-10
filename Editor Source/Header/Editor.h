#ifndef EDITOR_H
#define EDITOR_H

#include "../../Source/Header/Project.h"

// Buttons
static const int FILE_BUTTON_ID(0);
static const int RACE_EDITOR_BUTTON_ID(1);
static const int JOB_EDITOR_BUTTON_ID(2);
static const int BATTLEFIELD_EDITOR_BUTTON_ID(3);

// Menu's
static const int FILE_MENU_ID(0);

// Menu Buttons
static const int NEW_PROJECT_BUTTON_ID(0);
static const int OPEN_PROJECT_BUTTON_ID(1);
static const int SAVE_PROJECT_BUTTON_ID(2);
static const int SAVE_PROJECT_AS_BUTTON_ID(3);
static const int EXIT_BUTTON_ID(4);

// Open File Windows
static const int OPEN_PROJECT_WINDOW_ID(0);

// Race Editor Buttons
static const int NEW_RACE_BUTTON_ID(100);
static const int NAMELIST_BUTTON_ID(101);
static const int JOBLIST_BUTTON_ID(102);
static const int NEW_RACE_OK_BUTTON_ID(103);
static const int NEW_RACE_CANCEL_BUTTON_ID(104);
static const int NEW_NAME_BUTTON_ID(105);
static const int NEW_NAME_OK_BUTTON_ID(106);
static const int NEW_NAME_CANCEL_BUTTON_ID(107);
static const int ADD_JOB_BUTTON_ID(108);

// Race Editor EditBoxes
static const int NEW_RACE_EDITBOX_ID(200);
static const int NEW_NAME_EDITBOX_ID(201);

class Editor : public UpdatingObject
{
	public:
		Editor();
		virtual ~Editor();
		void Init();
		void Update(float deltaTime);
		void ButtonClicked(int id);
		void ContextMenuClicked(int id, int item);
		void FileOpened(int id, stringw fileName);
		void RefreshProject();
	protected:
	private:
		stringw m_ProjectFile;
		Project m_Project;
		IGUIStaticText* m_SubMenuParent;
		IGUIStaticText* m_SubSubMenuParent;
		IGUIStaticText* m_SubSubSubMenuParent;
		bool m_Loading;
		Race* m_CurrentRace;
		
};

#endif // EDITOR_H
