#ifndef NAMEDOBJECT_H
#define NAMEDOBJECT_H

#include "XMLObject.h"

using irr::core::stringw;

static const stringw NameTag(L"Name");

class NamedObject : public XMLObject
{
	public:
		NamedObject(stringw name = "");
		virtual ~NamedObject();
		const stringw Name() const { return m_Name; }
		void Name(stringw val) { m_Name = val; }
		bool operator<(const NamedObject& other) const;
		bool operator>(const NamedObject& other) const;
		bool operator==(const NamedObject& other) const;
		bool operator==(const stringw other) const;
		bool operator!=(const NamedObject& other) const;
		void WriteXML(IXMLWriter* xml);
		void ReadXML(IXMLReader* xml);
	protected:
	private:
		stringw m_Name;
};

#endif // NAMEDOBJECT_H
