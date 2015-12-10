#include "../Header/ActionMenuBlock.h"

ActionMenuBlock::ActionMenuBlock()
{
	Animation animation;
	vector<u32> vec;
	vec.push_back(0);
	animation.FrameOrder(vec);
	animation.FPS(1.0f);
	animation.FramePosition(vector2df(0, 0));
	animation.FrameSize(vector2df(300, 50));
	video::ITexture* image = GetIrrlicht::Driver()->getTexture("../../Data/ActionMenuBlockTop.png");
	animation.Texture(image);
	animation.Name(L"ActionMenuBlockTop");
	m_BlockTop.Animations().push_back(animation);
	m_BlockTop.Sprite().CreateNode(300, 50);
	m_BlockTop.PlayAnimation(0);
	image = GetIrrlicht::Driver()->getTexture("../../Data/ActionMenuBlockBottom.png");
	animation.Texture(image);
	animation.Name(L"ActionMenuBlockBottom");
	m_BlockBottom.Animations().push_back(animation);
	m_BlockBottom.Sprite().CreateNode(300, 50);
	m_BlockBottom.PlayAnimation(0);
	m_BlockBottom.Sprite().Node()->getParent()->setParent(m_BlockTop.Sprite().Node());
	image = GetIrrlicht::Driver()->getTexture("../../Data/ActionMenuBlockMiddle.png");
	animation.Texture(image);
	animation.Name(L"ActionMenuBlockMiddle");
	m_ActionsBackground.Animations().push_back(animation);
	m_ActionsBackground.PlayAnimation(0);
	m_ActionsBackground.Sprite().Node()->getParent()->setParent(m_BlockTop.Sprite().Node());
	m_Font = GetIrrlicht::GuiEnv()->getFont(L"../../Data/FFTA/BoldTestFont.xml");
	/*m_ActionsText.push_back(GetIrrlicht::Smgr()->addBillboardTextSceneNode(m_Font, L"*Move", m_BlockTop.Sprite().Node(), static_cast<dimension2df>(m_Font->getDimension(L"*Move")), vector3df(0.0f, -25.0f, -0.1f)));
	m_ActionsBackground.Sprite().Node()->setPosition(vector3df(0.0f, -50.0f + m_ActionsText[0]->getSize().Height / 2, 0.0f));
	m_ActionsBackground.Sprite().NodeSize(300, m_ActionsText[0]->getSize().Height);
	m_BlockBottom.Sprite().Node()->setPosition(vector3df(0.0f, -50.0f - m_ActionsText[0]->getSize().Height, 0.0f));
	m_ActionsText[0]->setColor(SColor(255, 255, 255, 0));
	AlignActionLeft(m_ActionsText[0], L"*Move");*/
}

void ActionMenuBlock::AlignActionLeft(IBillboardTextSceneNode* action, const stringw& actionText)
{
	action->setPosition(vector3df(m_Font->getDimension(actionText.c_str()).Width / 2.0f -120, action->getPosition().Y, action->getPosition().Z));
}

void ActionMenuBlock::SetActions(vector<stringw> actions)
{
	for (vector<IBillboardTextSceneNode*>::iterator i = m_ActionsText.begin(); i != m_ActionsText.end(); ++i)
	{
		(*i)->remove();
	}
	m_ActionsText.clear();
	float longestString = 0;
	for (vector<stringw>::iterator i = actions.begin(); i != actions.end(); ++i)
	{
		m_ActionsText.push_back(GetIrrlicht::Smgr()->addBillboardTextSceneNode(
								m_Font, i->c_str(), m_BlockTop.Sprite().Node(),
								static_cast<dimension2df>(m_Font->getDimension(i->c_str())),
								vector3df(0.0f, -35.0f - (m_ActionsText.size() * 1.0f * m_Font->getDimension(L"G").Height), -0.1f)));
		AlignActionLeft(m_ActionsText[m_ActionsText.size() - 1], *i);
		longestString = std::max(static_cast<float>(m_Font->getDimension(i->c_str()).Width), longestString);
	}
	m_ActionsBackground.Sprite().NodeSize(300, m_Font->getDimension(L"G").Height * m_ActionsText.size());
	m_ActionsBackground.Sprite().Node()->setPosition(vector3df(0.0f, -25.0f - (m_Font->getDimension(L"G").Height * (m_ActionsText.size())) * 1.0f, 0.0f));
	m_BlockBottom.Sprite().Node()->setPosition(vector3df(0.0f, -73.0f - m_Font->getDimension(L"G").Height * m_ActionsText.size(), 0.0f));
	m_BlockTop.Sprite().Node()->setPosition(vector3df(730.0f - longestString, m_Font->getDimension(L"G").Height * m_ActionsText.size() / 2.0f, 0.0f));
	m_SelectedAction = 0;
	m_ActionsText[m_SelectedAction]->setColor(SColor(255, 255, 255, 0));
}

void ActionMenuBlock::Update(float deltaTime)
{
	if (GetIrrlicht::Events()->keyPressed(KEY_KEY_W) || GetIrrlicht::Events()->keyPressed(KEY_UP))
	{
		m_ActionsText[m_SelectedAction]->setColor(SColor(255, 255, 255, 255));
		if (m_SelectedAction > 0) m_SelectedAction--; else m_SelectedAction = m_ActionsText.size() - 1; 
		m_ActionsText[m_SelectedAction]->setColor(SColor(255, 255, 255, 0));
	}
	if (GetIrrlicht::Events()->keyPressed(KEY_KEY_S) || GetIrrlicht::Events()->keyPressed(KEY_DOWN))
	{
		m_ActionsText[m_SelectedAction]->setColor(SColor(255, 255, 255, 255));
		if (m_SelectedAction < m_ActionsText.size() - 1u) m_SelectedAction++; else m_SelectedAction = 0;
		m_ActionsText[m_SelectedAction]->setColor(SColor(255, 255, 255, 0));
	}
}

void ActionMenuBlock::Visible(bool visible)
{
	m_BlockTop.Sprite().Node()->setVisible(visible);
}

bool ActionMenuBlock::Visible()
{
	return m_BlockTop.Sprite().Node()->isVisible();
}


ActionMenuBlock::~ActionMenuBlock()
{
	//dtor
}
