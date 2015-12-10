#include "../Header/Selector.h"

Selector::Selector()
{
	/*m_selectedUnit = NULL;
	m_selecting = false;
	Animation selectorAnimation;
	
	vector<u32> vec;
	vec.push_back(0);
	vec.push_back(1);
	selectorAnimation.FrameOrder(vec);
	selectorAnimation.FPS(4.0f);
	selectorAnimation.FramePosition(vector2df(0, 0));
	selectorAnimation.FrameSize(vector2df(200, 400));
	
	selectorAnimation.Texture(image);
	selectorAnimation.Name(L"Standard");
	Animations().push_back(selectorAnimation);
	video::ITexture* image = GetIrrlicht::Driver()->getTexture("../../Data/Simple Selector.png");
	Animations().clear();
	Animations().push_back(Animation());
	(Animations().end() - 1)->FPS(4.0f);
	(Animations().end() - 1)->FramePosition(vector2df(0, 0));
	(Animations().end() - 1)->FrameSize(vector2df(200, 400));
	(Animations().end() - 1)->Texture(image);
	(Animations().end() - 1)->Name(L"Standard");
	(Animations().end() - 1)->FramePosition(vector2df(0, 0));
	(Animations().end() - 1)->FrameOrder().clear();
	(Animations().end() - 1)->FrameOrder().push_back(0);
	(Animations().end() - 1)->FrameOrder().push_back(1);
	PlayAnimation(0);*/
}

Selector::~Selector()
{
	//dtor
}

void Selector::WriteXML(IXMLWriter* xml)
{
	
}

void Selector::ReadXML(IXMLReader* xml)
{
	do
	{
		switch (xml->getNodeType())
		{
			case irr::io::EXN_ELEMENT:
			{
				if (AnimatingObjectTag.equals_ignore_case(xml->getNodeName()))
				{
					AnimatingObject::ReadXML(xml);
				} else if (SelectorTag.equals_ignore_case(xml->getNodeName()))
				{
					
				}
				break;
			}
			case irr::io::EXN_ELEMENT_END:
			{
				if (SelectorTag.equals_ignore_case(xml->getNodeName()))
				{
					return;
				}
				break;
			}
			default:
				break;
		}
	} while (xml->read());
}
