#include "../Header/BattleFieldPosition.h"

BattleFieldPosition::BattleFieldPosition()
{
	//position = vector3df(0, 0, 0);
}

BattleFieldPosition::BattleFieldPosition(float x, float y, float z)
{
	position = vector3df(x, y, z);
}

BattleFieldPosition::BattleFieldPosition(vector3df pos)
{
	position = pos;
}

BattleFieldPosition::~BattleFieldPosition()
{
	//dtor
}

void BattleFieldPosition::ReadXML(IXMLReader* xml)
{
	do 
	{
		switch (xml->getNodeType())
		{
			case irr::io::EXN_ELEMENT:
			{
				if (BattleFieldPositionTag.equals_ignore_case(xml->getNodeName()))
				{
					position.X = xml->getAttributeValueAsFloat(L"X");
					position.Y = xml->getAttributeValueAsFloat(L"Y");
					position.Z = xml->getAttributeValueAsFloat(L"Z");
					return;
				} else return;
				break;
			}
			case irr::io::EXN_ELEMENT_END:
				if (BattleFieldPositionTag.equals_ignore_case(xml->getNodeName())) return;
				break;
			default:
				break;			
		}
	} while (xml->read());
}

void BattleFieldPosition::WriteXML(IXMLWriter* xml)
{
	xml->writeElement(BattleFieldPositionTag.c_str(), true, L"X", stringw(position.X).c_str(), L"Y", stringw(position.Y).c_str(), L"Z", stringw(position.Z).c_str());
	xml->writeLineBreak();
}
