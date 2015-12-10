#include "../Header/AnimatedSprite.h"

#include <iostream>

AnimatedSprite::AnimatedSprite()
{
	m_CurrentAnimation = NULL;
	m_PauseTime = 0;
	m_Node = NULL;
}

AnimatedSprite::~AnimatedSprite()
{
	//dtor
}

void AnimatedSprite::CurrentAnimation(Animation* newAnimation, bool mirrored, bool loop) 
{
	if (m_Node == NULL) CreateNode();
	m_CurrentAnimation = newAnimation;
	RestoreAnimationData();
	m_Node->setMaterialTexture(0, newAnimation->Texture());
	m_Mirrored = mirrored; 
	m_Loop = loop;
}

void AnimatedSprite::Update(float deltaTime)
{
	if (m_CurrentAnimation == NULL || m_Node == NULL) return;
	if (m_PauseTime > 0.0f)
	{
		m_PauseTime -= deltaTime;
		if (m_PauseTime >= 0.0f) deltaTime = 0.0f;
		else deltaTime = -m_PauseTime;
	} else m_PauseTime = 0;
	m_CurrentFrame += deltaTime * m_FPS;
	while(m_CurrentFrame >= m_CurrentAnimation->TotalFrames()) 
		if (m_Loop) m_CurrentFrame -= static_cast<float>(m_CurrentAnimation->TotalFrames());
		else m_CurrentFrame = m_CurrentAnimation->TotalFrames() - 1.0f;
	if (m_CurrentFrame < 0) m_CurrentFrame = 0;
	
	float translateX = 0.0f;
	float translateY = 0.0f;
	float scaleX = 0.0f;
	float scaleY = 0.0f;
	
	float framePositionX = m_CurrentAnimation->FramePosition().X;
	float framePositionY = m_CurrentAnimation->FramePosition().Y;
	
	vector<unsigned int> frameOrders = m_CurrentAnimation->FrameOrder();
	float theCurrentFrame = static_cast<float>(frameOrders[static_cast<int>(m_CurrentFrame)]);
	float textureWidth = static_cast<float>(m_CurrentAnimation->Texture()->getSize().Width);
	float textureHeight = static_cast<float>(m_CurrentAnimation->Texture()->getSize().Height);
	float frameSizeX = m_CurrentAnimation->FrameSize().X;
	float frameSizeY = m_CurrentAnimation->FrameSize().Y;
	
	translateX = frameSizeX * theCurrentFrame;
	translateX = translateX + framePositionX;
	translateX = translateX / textureWidth;
	
	translateY = framePositionY / textureHeight;
	
	scaleX = frameSizeX / textureWidth;
	scaleY = frameSizeY / textureHeight;
	
	m_Matrix.buildTextureTransform(0.0f, vector2df(0.0f, 0.0f), vector2df(translateX, translateY), vector2df(scaleX, scaleY));
	
	/*
	if (m_Mirrored) m_Matrix.buildTextureTransform(0.0f, vector2df(0.0f, 0.0f), vector2df((m_CurrentAnimation->FramePosition().X - 1 + m_CurrentAnimation->FrameSize().X * 
																	static_cast<float>(m_CurrentAnimation->FrameOrder()[static_cast<int>(m_CurrentFrame)] + 1)) / static_cast<float>(m_CurrentAnimation->Texture()->getSize().Width),
																	m_CurrentAnimation->FramePosition().Y / static_cast<float>(m_CurrentAnimation->Texture()->getSize().Height)),
									vector2df(-(m_CurrentAnimation->FrameSize().X / static_cast<float>(m_CurrentAnimation->Texture()->getSize().Width)),
									m_CurrentAnimation->FrameSize().Y / static_cast<float>(m_CurrentAnimation->Texture()->getSize().Height)));
	else m_Matrix.buildTextureTransform(0.0f, vector2df(0.0f, 0.0f), vector2df((m_CurrentAnimation->FramePosition().X + m_CurrentAnimation->FrameSize().X * 
																	static_cast<float>(m_CurrentAnimation->FrameOrder()[static_cast<int>(m_CurrentFrame)])) / static_cast<float>(m_CurrentAnimation->Texture()->getSize().Width),
																	m_CurrentAnimation->FramePosition().Y / static_cast<float>(m_CurrentAnimation->Texture()->getSize().Height)),
									vector2df(m_CurrentAnimation->FrameSize().X / static_cast<float>(m_CurrentAnimation->Texture()->getSize().Width),
									m_CurrentAnimation->FrameSize().Y / static_cast<float>(m_CurrentAnimation->Texture()->getSize().Height)));
	*/
	
	m_Node->getMaterial(0).setTextureMatrix(0, m_Matrix);
}

void AnimatedSprite::CreateNode(float width, float height)
{
	ISceneNode* placementNode = GetIrrlicht::Smgr()->addEmptySceneNode();
	placementNode->setPosition(vector3df(0, height / 2, 0));
	m_Node = GetIrrlicht::Smgr()->addBillboardSceneNode(placementNode, dimension2d<float>(width, height), vector3df(0, 0, 0));
	m_Node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	m_Node->setMaterialFlag(irr::video::EMF_BILINEAR_FILTER, false);
    m_Node->setMaterialFlag(irr::video::EMF_TRILINEAR_FILTER, false);
    m_Node->setMaterialFlag(irr::video::EMF_ANISOTROPIC_FILTER, false);
	m_Node->setMaterialType(irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF);
}

void AnimatedSprite::NodeSize(float width, float height)
{
	m_Node->getParent()->setPosition(vector3df(0, height / 2, 0));
	m_Node->setSize(dimension2df(width, height));
}

