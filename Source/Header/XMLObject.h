#ifndef XMLOBJECT_H
#define XMLOBJECT_H

#include "irrlicht.h"

using irr::io::IXMLReader;
using irr::io::IXMLWriter;

class XMLObject
{
	public:
		XMLObject();
		virtual ~XMLObject();
		virtual void ReadXML(IXMLReader* xml) = 0;
		virtual void WriteXML(IXMLWriter* xml) = 0;
	protected:
	private:
};

#endif // XMLOBJECT_H
