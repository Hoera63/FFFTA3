#include "../Header/NamedObject.h"

NamedObject::NamedObject(stringw name)
{
	m_Name = name;
}

NamedObject::~NamedObject()
{
	//dtor
}

bool NamedObject::operator<(const NamedObject& other) const
{
	if (*this == other) return false;
	for (unsigned int i = 0; i < m_Name.size(); i++)
	{
		if (m_Name[i] < other.Name()[i])
		{
			return true;
		} else if (m_Name[i] > other.Name()[i])
		{
			return false;
		}
	}
	return false;
}

bool NamedObject::operator>(const NamedObject& other) const
{
	if (*this == other) return false;
	for (unsigned int i = 0; i < m_Name.size(); i++)
	{
		if (m_Name[i] > other.Name()[i])
		{
			return true;
		} else if (m_Name[i] < other.Name()[i])
		{
			return false;
		}
	}
	return false;
}

bool NamedObject::operator==(const NamedObject& other) const
{
	return m_Name.equals_ignore_case(other.Name());
}

bool NamedObject::operator==(const stringw other) const
{
	return m_Name.equals_ignore_case(other);
}

bool NamedObject::operator!=(const NamedObject& other) const
{
	return !m_Name.equals_ignore_case(other.Name());
}

void NamedObject::WriteXML(IXMLWriter* xml)
{
	xml->writeElement(NameTag.c_str(), true, L"Value", m_Name.c_str());
	xml->writeLineBreak();
}
void NamedObject::ReadXML(IXMLReader* xml)
{
	do 
	{
		switch (xml->getNodeType())
		{
			case irr::io::EXN_ELEMENT:
			{
				if (NameTag.equals_ignore_case(xml->getNodeName()))
				{
					m_Name = xml->getAttributeValueSafe(L"Value");
					return;
				} else return;
				break;
			}
			case irr::io::EXN_ELEMENT_END:
				if (NameTag.equals_ignore_case(xml->getNodeName())) return;
				break;
			default:
				break;			
		}
	} while (xml->read());
}
