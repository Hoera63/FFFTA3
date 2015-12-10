#include "../Header/Race.h"

Race::Race()
{
	//ctor
}

Race::~Race()
{
	//dtor
}

// XML
void Race::ReadXML(IXMLReader* xml)
{
	do 
	{
		switch (xml->getNodeType())
		{
			case irr::io::EXN_ELEMENT:
			{
				if (RaceTag.equals_ignore_case(xml->getNodeName()))
				{
					break;
				} else if (NameTag.equals_ignore_case(xml->getNodeName()))
				{
					NamedObject::ReadXML(xml);
				} else if (NameListTag.equals_ignore_case(xml->getNodeName()))
				{
					m_NameListFile = xml->getAttributeValueSafe(L"FileName");
					// Namelist loading
					IXMLReader* xmlNames = GetIrrlicht::Device()->getFileSystem()->createXMLReader(m_NameListFile.c_str());
					if (!xmlNames)
							;
					else
					{
						while(xmlNames->read())
						{
							switch (xmlNames->getNodeType())
							{
								case irr::io::EXN_ELEMENT:
								{
									if (NameTag.equals_ignore_case(xmlNames->getNodeName()))
									{
										m_NameList.push_back(stringw(xmlNames->getAttributeValueSafe(L"Value")));
									}
								}
								default:
									break;
							}
						}
						xmlNames->drop();
					}
				} else return;
				break;
			}
			case irr::io::EXN_ELEMENT_END:
				if (RaceTag.equals_ignore_case(xml->getNodeName())) return;
				break;
			default:
				break;			
		}
	} while (xml->read());
}

void Race::WriteXML(IXMLWriter* xml)
{
	
}
