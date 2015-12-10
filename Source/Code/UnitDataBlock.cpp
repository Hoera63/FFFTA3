#include "../Header/UnitDataBlock.h"

UnitDataBlock::UnitDataBlock()
{
	m_Unit = NULL;
	
	Animation animation;
	vector<u32> vec;
	vec.push_back(0);
	animation.FrameOrder(vec);
	animation.FPS(1.0f);
	animation.FramePosition(vector2df(0, 0));
	animation.FrameSize(vector2df(500, 600));
	video::ITexture* image = GetIrrlicht::Driver()->getTexture("../../Data/UnitDataBlockv2.png");
	animation.Texture(image);
	animation.Name(L"BlueBlock");
	Animations().push_back(animation);
	image = GetIrrlicht::Driver()->getTexture("../../Data/UnitDataBlock2v2.png");
	animation.Texture(image);
	animation.Name(L"RedBlock");
	Animations().push_back(animation);
	PlayAnimation(0);
	Sprite().Node()->setMaterialType(irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL);
	Sprite().NodeSize(500, 600);
	Sprite().Node()->setPosition(vector3df(-390.0f, -610.0f, 0.0f));
	IGUIFont* blockFont = GetIrrlicht::GuiEnv()->getFont(L"../../Data/FFTA/BoldTestFont.xml");/*GetIrrlicht::GuiEnv()->getBuiltInFont();*/
	m_NameText = GetIrrlicht::Smgr()->addBillboardTextSceneNode(blockFont, L"Insert Name", Sprite().Node(), static_cast<dimension2df>(blockFont->getDimension(L"Insert Name")), vector3df(0.0f,280.0f,-0.9f));
	m_HPText = GetIrrlicht::Smgr()->addBillboardTextSceneNode(blockFont, L"HP  20/ 40", Sprite().Node(), static_cast<dimension2df>(blockFont->getDimension(L"HP  20/ 40")), vector3df(0.0f,230.0f,-0.9f));
	m_MPText = GetIrrlicht::Smgr()->addBillboardTextSceneNode(blockFont, L"MP  11/ 53", Sprite().Node(), static_cast<dimension2df>(blockFont->getDimension(L"MP  11/ 53")), vector3df(0.0f,180.0f,-0.9f));
	m_JPText = GetIrrlicht::Smgr()->addBillboardTextSceneNode(blockFont, L"JP  0", Sprite().Node(), static_cast<dimension2df>(blockFont->getDimension(L"JP  0")), vector3df(0.0f, 130.f,-0.9f));
	m_LevelText = GetIrrlicht::Smgr()->addBillboardTextSceneNode(blockFont, L"Lv: 24 Exp: 9", Sprite().Node(), static_cast<dimension2df>(blockFont->getDimension(L"Lv: 24 Exp: 9")), vector3df(0.0f,20.0f,-0.9f));
	m_UnitPicture.Sprite().CreateNode();
	m_UnitPicture.Sprite().Node()->getParent()->setParent(Sprite().Node());
}

UnitDataBlock::~UnitDataBlock()
{
	//dtor
}

void UnitDataBlock::DataUnit(Unit* theUnit, bool blue, bool left)
{
	m_Unit = theUnit;
	UpdateData(left);
	if (blue) PlayAnimation("BlueBlock", !left); else PlayAnimation("RedBlock", !left);
	if (left) Sprite().Node()->setPosition(vector3df(-390.0f, -610.0f, 0.0f)); else Sprite().Node()->setPosition(vector3df(390.0f, -610.0f, 0.0f));
}

void UnitDataBlock::UpdateData(bool left)
{
	if (m_Unit == NULL) return;
	IGUIFont* blockFont = GetIrrlicht::GuiEnv()->getFont(L"../../Data/FFTA/BoldTestFont.xml");
	stringw text = m_Unit->Name();
	m_NameText->setText(text.c_str());
	m_NameText->setSize(static_cast<dimension2df>(blockFont->getDimension(text.c_str())));
	text = stringw(L"HP ") + stringw(m_Unit->CurrentHP()) + stringw(L"/ ") + stringw(m_Unit->HP());
	m_HPText->setText(text.c_str());
	m_HPText->setSize(static_cast<dimension2df>(blockFont->getDimension(text.c_str())));
	text = stringw(L"MP ") + stringw(m_Unit->CurrentMP()) + stringw(L"/ ") + stringw(m_Unit->MP());
	m_MPText->setText(text.c_str());
	m_MPText->setSize(static_cast<dimension2df>(blockFont->getDimension(text.c_str())));
	text = stringw(L"JP ") + stringw(m_Unit->JP());
	m_JPText->setText(text.c_str());
	m_JPText->setSize(static_cast<dimension2df>(blockFont->getDimension(text.c_str())));
	text = stringw(L"Lv: ") + stringw(m_Unit->Level()) + stringw(L" Exp: ") + stringw(m_Unit->XP());
	m_LevelText->setText(text.c_str());
	m_LevelText->setSize(static_cast<dimension2df>(blockFont->getDimension(text.c_str())));
	m_UnitPicture.Animations() = m_Unit->Animations();
	m_UnitPicture.PlayAnimation(L"Portrait", !left);
	float scale = 200 / m_UnitPicture.Animations()[0].FrameSize().X;
	m_UnitPicture.Sprite().NodeSize(200, m_UnitPicture.Animations()[0].FrameSize().Y * scale);
	m_UnitPicture.Sprite().Node()->setPosition(vector3df((-250.0f + 100) * (left?1:-1), 300.0f - m_UnitPicture.Animations()[0].FrameSize().Y * scale / 2, -0.8f));
}

void UnitDataBlock::Update(float deltaTime)
{
	AnimatingObject::Update(deltaTime);
	m_UnitPicture.Update(deltaTime);
}
