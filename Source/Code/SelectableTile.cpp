#include "../Header/SelectableTile.h"

SelectableTile::SelectableTile()
{
	
}

SelectableTile::~SelectableTile()
{
	
}

void SelectableTile::Init()
{
	/*
	Animation animation;
	vector<u32> vec;
	vec.push_back(0);
	vec.push_back(1);
	vec.push_back(2);
	vec.push_back(3);
	vec.push_back(4);
	vec.push_back(5);
	vec.push_back(6);
	vec.push_back(7);
	vec.push_back(8);
	vec.push_back(9);
	animation.FrameOrder(vec);
	animation.FPS(8.0f);
	animation.FramePosition(vector2df(0.0f, 0.0f));
	animation.FrameSize(vector2df(128.0f, 64.0f));
	video::ITexture* image = NULL;
	image = GetIrrlicht::Driver()->getTexture("../../Data/Moveable Space.png");
	animation.Texture(image);
	animation.Name(L"Blue Space");
	Animations().push_back(animation);
	image = NULL;
	image = GetIrrlicht::Driver()->getTexture("../../Data/Attackable Space.png");
	animation.Texture(image);
	animation.Name(L"Red Space");
	Animations().push_back(animation);
	PlayAnimation(0);
	*/
}

void SelectableTile::WriteXML(IXMLWriter* xml)
{
	
}

void SelectableTile::ReadXML(IXMLReader* xml)
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
				} else if (SelectableTileTag.equals_ignore_case(xml->getNodeName()))
				{
					
				}
				break;
			}
			case irr::io::EXN_ELEMENT_END:
			{
				if (SelectableTileTag.equals_ignore_case(xml->getNodeName()))
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
