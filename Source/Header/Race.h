#ifndef RACE_H
#define RACE_H

#include <vector>

#include "NamedObject.h"
#include "GetIrrlicht.h"

using std::vector;

static const stringw RaceTag(L"Race");
static const stringw NameListTag(L"NameList");

class Race : public NamedObject
{
	public:
		Race();
		virtual ~Race();
		vector<stringw>* NameList() { return &m_NameList; }
// XML
		void ReadXML(IXMLReader* xml);
		void WriteXML(IXMLWriter* xml);
	protected:
	private:
		stringw m_NameListFile;
		vector<stringw> m_NameList;
};

#endif // RACE_H
