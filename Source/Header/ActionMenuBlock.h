#ifndef ACTIONMENUBLOCK_H
#define ACTIONMENUBLOCK_H

#include "AnimatingObject.h"

class ActionMenuBlock : public UpdatingObject
{
	public:
		ActionMenuBlock();
		virtual ~ActionMenuBlock();
		void AlignActionLeft(IBillboardTextSceneNode* action, const stringw&  actionText);
		void SetActions(vector<stringw> actions);
		void Update(float deltaTime);
		void Visible(bool visible);
		bool Visible();
		int SelectedAction() { return m_SelectedAction; };
		void SelectedAction(int newSelected) { m_SelectedAction = newSelected; };
	protected:
	private:
		AnimatingObject m_BlockTop;
		AnimatingObject m_BlockBottom;
		AnimatingObject m_ActionsBackground;
		vector<IBillboardTextSceneNode*> m_ActionsText;
		unsigned int m_SelectedAction;
		IGUIFont* m_Font;
};

#endif // ACTIONMENUBLOCK_H
