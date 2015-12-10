#include "../Header/Job.h"

Job::Job()
{
	//ctor
}

Job::~Job()
{
	//dtor
}

// XML
void Job::ReadXML(IXMLReader* xml)
{
	do 
	{
		switch (xml->getNodeType())
		{
			case irr::io::EXN_ELEMENT:
			{
				if (JobTag.equals_ignore_case(xml->getNodeName()))
				{
					break;
				} else if (NameTag.equals_ignore_case(xml->getNodeName()))
				{
					NamedObject::ReadXML(xml);
				} else return;
				break;
			}
			case irr::io::EXN_ELEMENT_END:
				if (JobTag.equals_ignore_case(xml->getNodeName())) return;
				break;
			default:
				break;			
		}
	} while (xml->read());
}

void Job::WriteXML(IXMLWriter* xml)
{
	
}
