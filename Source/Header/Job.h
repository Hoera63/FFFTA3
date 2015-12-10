#ifndef JOB_H
#define JOB_H

#include "NamedObject.h"

static const stringw JobTag(L"Job");

class Job : public NamedObject
{
	public:
		Job();
		virtual ~Job();
// XML
		void ReadXML(IXMLReader* xml);
		void WriteXML(IXMLWriter* xml);
	protected:
	private:
};

#endif // JOB_H
