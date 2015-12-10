#include "../Header/Animation.h"

Animation::Animation()
{
	//ctor
}

Animation::~Animation()
{
	//dtor
}

// XML

void Animation::ReadXML(IXMLReader* xml)
{
	m_FrameOrder.clear();
	do 
	{
		switch (xml->getNodeType())
		{
			case irr::io::EXN_ELEMENT:
			{
				if (NameTag.equals_ignore_case(xml->getNodeName()))
				{
					NamedObject::ReadXML(xml);
				} else if (AnimationTag.equals_ignore_case(xml->getNodeName()))
				{
					m_Texture = GetIrrlicht::Driver()->getTexture(xml->getAttributeValueSafe(L"Texture"));
					m_FPS = xml->getAttributeValueAsFloat(L"FPS");
					m_FramePosition.X = xml->getAttributeValueAsFloat(L"FramePositionX");
					m_FramePosition.Y = xml->getAttributeValueAsFloat(L"FramePositionY");
					m_FrameSize.X = xml->getAttributeValueAsFloat(L"FrameSizeWidth");
					m_FrameSize.Y = xml->getAttributeValueAsFloat(L"FrameSizeHeight");
				} else if (AnimationFrameTag.equals_ignore_case(xml->getNodeName()))
				{
					m_FrameOrder.push_back(xml->getAttributeValueAsInt(L"Order"));
				} else return;
				break;
			}
			case irr::io::EXN_ELEMENT_END:
				if (AnimationTag.equals_ignore_case(xml->getNodeName())) return;
				break;
			default:
				break;			
		}
	} while (xml->read());
}

void Animation::WriteXML(IXMLWriter* xml)
{
	irr::core::array<stringw> nameArray;
	nameArray.push_back(L"Texture");
	nameArray.push_back(L"FPS");
	nameArray.push_back(L"FramePositionX");
	nameArray.push_back(L"FramePositionY");
	nameArray.push_back(L"FrameSizeWidth");
	nameArray.push_back(L"FrameSizeHeight");
	irr::core::array<stringw> valueArray;
	valueArray.push_back(stringw(GetIrrlicht::Device()->getFileSystem()->getRelativeFilename(m_Texture->getName().getInternalName(), GetIrrlicht::Device()->getFileSystem()->getWorkingDirectory()).c_str()));
	valueArray.push_back(stringw(m_FPS));
	valueArray.push_back(stringw(m_FramePosition.X));
	valueArray.push_back(stringw(m_FramePosition.Y));
	valueArray.push_back(stringw(m_FrameSize.X));
	valueArray.push_back(stringw(m_FrameSize.Y));
	xml->writeElement(AnimationTag.c_str(), false, nameArray, valueArray);
	
	for(vector<unsigned int>::iterator i = m_FrameOrder.begin(); i != m_FrameOrder.end(); ++i) 
	{
		xml->writeLineBreak();
		xml->writeElement(AnimationFrameTag.c_str(), true, L"Order", stringw(*i).c_str());
	}
	xml->writeLineBreak();
	NamedObject::WriteXML(xml);
	xml->writeClosingTag(AnimationTag.c_str());
	xml->writeLineBreak();
}
