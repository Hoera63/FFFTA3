#include "../Header/AnimatingObject.h"

AnimatingObject::AnimatingObject()
{
	//ctor
}

AnimatingObject::~AnimatingObject()
{
	//dtor
}

void AnimatingObject::PlayAnimation(stringw name, bool mirrored, bool loop)
{
	for(vector<Animation>::iterator i = m_Animations.begin(); i != m_Animations.end(); ++i) 
	{
		if ((*i).Name() == name)
		{
			m_Sprite.CurrentAnimation(&(*i), mirrored, loop);
			return;
		}
	}
}

void AnimatingObject::PlayAnimation(int number, bool mirrored, bool loop)
{
	m_Sprite.CurrentAnimation(&m_Animations[number], mirrored, loop);
}

void AnimatingObject::Update(float deltaTime)
{
	m_Sprite.Update(deltaTime);
}

// XML

void AnimatingObject::ReadXML(IXMLReader* xml)
{
	m_Animations.clear();
	do 
	{
		switch (xml->getNodeType())
		{
			case irr::io::EXN_ELEMENT:
			{
				if (AnimatingObjectTag.equals_ignore_case(xml->getNodeName()))
				{
					break;
				} else if (AnimationTag.equals_ignore_case(xml->getNodeName()))
				{
					m_Animations.push_back(Animation());
					(m_Animations.end() - 1)->ReadXML(xml);
				} else return;
				break;
			}
			case irr::io::EXN_ELEMENT_END:
				if (AnimatingObjectTag.equals_ignore_case(xml->getNodeName()))
				{
					//PlayAnimation(0);
					return;
				} 
				break;
			default:
				break;			
		}
	} while (xml->read());
}

void AnimatingObject::WriteXML(IXMLWriter* xml)
{
	xml->writeElement(AnimatingObjectTag.c_str(), false);
	xml->writeLineBreak();
	for(vector<Animation>::iterator i = m_Animations.begin(); i != m_Animations.end(); ++i) 
	{
		i->WriteXML(xml);
	}
	xml->writeClosingTag(AnimatingObjectTag.c_str());
	xml->writeLineBreak();
}
