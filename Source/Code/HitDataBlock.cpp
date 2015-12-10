#include "../Header/HitDataBlock.h"

HitDataBlock::HitDataBlock()
{
	IGUIFont* blockFont = GetIrrlicht::GuiEnv()->getFont(L"../../Data/FFTA/BoldTestFont.xml");
	m_DamageText = GetIrrlicht::Smgr()->addBillboardTextSceneNode(blockFont, L"", 0, static_cast<dimension2df>(blockFont->getDimension(L" ")), vector3df(0.0f,-280.0f,-0.9f));
	m_HitChanceText = GetIrrlicht::Smgr()->addBillboardTextSceneNode(blockFont, L"", 0, static_cast<dimension2df>(blockFont->getDimension(L" ")), vector3df(0.0f,-330.0f,-0.9f));
}

HitDataBlock::~HitDataBlock()
{
	//dtor
}

void HitDataBlock::SetData(HitData theData)
{
	IGUIFont* blockFont = GetIrrlicht::GuiEnv()->getFont(L"../../Data/FFTA/BoldTestFont.xml");
	stringw text = stringw(L"Damage: ") + stringw(theData.Damage);
	m_DamageText->setText(text.c_str());
	m_DamageText->setSize(static_cast<dimension2df>(blockFont->getDimension(text.c_str())));
	text = stringw(L"HitChance ") + stringw(theData.HitRate) + stringw(L"%");
	m_HitChanceText->setText(text.c_str());
	m_HitChanceText->setSize(static_cast<dimension2df>(blockFont->getDimension(text.c_str())));
}

void HitDataBlock::Visible(bool visible)
{
	if (!visible)
	{
		m_DamageText->setText(L"");
		m_HitChanceText->setText(L"");
	}
}
