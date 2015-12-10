#include "../Header/JobRace.h"
#include "../Header/Project.h"

JobRace::JobRace()
{
	m_Project = NULL;
}

JobRace::~JobRace()
{
	//dtor
}

void JobRace::SetProject(Project* theProject)
{
	m_Project = theProject;
}

void JobRace::WriteXML(IXMLWriter* xml)
{
	
}

void JobRace::ReadXML(IXMLReader* xml)
{
	if (m_Project == NULL) return;
	bool insideJob = false;
	bool insideRace = false;
	do 
	{
		switch (xml->getNodeType())
		{
			case irr::io::EXN_ELEMENT:
			{
				if (JobRaceTag.equals_ignore_case(xml->getNodeName()))
				{
					break;
				} else if (JobTag.equals_ignore_case(xml->getNodeName()))
				{
					insideJob = true;
				} else if (RaceTag.equals_ignore_case(xml->getNodeName()))
				{
					insideRace = true;
				} else if (NameTag.equals_ignore_case(xml->getNodeName()))
				{
					if (insideJob) m_Job = m_Project->GetJob(xml->getAttributeValueSafe(L"Value"));
					else if (insideRace) m_Race = m_Project->GetRace(xml->getAttributeValueSafe(L"Value"));
				} else if (AnimationTag.equals_ignore_case(xml->getNodeName()))
				{
					m_AnimationList.push_back(Animation());
					(m_AnimationList.end() - 1)->ReadXML(xml);
				} else return;
				break;
			}
			case irr::io::EXN_ELEMENT_END:
				if (JobRaceTag.equals_ignore_case(xml->getNodeName()))
				{
					return;
				} else if (JobTag.equals_ignore_case(xml->getNodeName()))
				{
					insideJob = false;
				} else if (RaceTag.equals_ignore_case(xml->getNodeName()))
				{
					insideRace = false;
				}
				break;
				
			default:
				break;			
		}
	} while (xml->read());
}
