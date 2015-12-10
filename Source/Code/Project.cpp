#include "../Header/Project.h"

Project::Project()
{
	//ctor
}

Project::~Project()
{
	//dtor
}

void Project::LoadData()
{
	m_JobList.clear();
	m_RaceList.clear();
	m_JobRaceList.clear();
	m_BattlefieldList.clear();
	
	// Job loading
	IXMLReader* xml = GetIrrlicht::Device()->getFileSystem()->createXMLReader(m_JobListFile.c_str());
	if (!xml)
			;
	else
	{
		while(xml->read())
		{
			switch (xml->getNodeType())
			{
				case irr::io::EXN_ELEMENT:
				{
					if (JobTag.equals_ignore_case(xml->getNodeName()))
					{
						m_JobList.push_back(Job());
						(m_JobList.end() - 1)->ReadXML(xml);
					}
					break;
				}
				default:
					break;
			}
		}
		xml->drop();
	}
	// Race loading
	xml = GetIrrlicht::Device()->getFileSystem()->createXMLReader(m_RaceListFile.c_str());
	if (!xml)
			;
	else
	{
		while(xml->read())
		{
			switch (xml->getNodeType())
			{
				case irr::io::EXN_ELEMENT:
				{
					if (RaceTag.equals_ignore_case(xml->getNodeName()))
					{
						m_RaceList.push_back(Race());
						(m_RaceList.end() - 1)->ReadXML(xml);
					}
					break;
				}
				default:
					break;
			}
		}
		xml->drop();
	}
	// JobRace loading
	xml = GetIrrlicht::Device()->getFileSystem()->createXMLReader(m_JobRaceListFile.c_str());
	if (!xml)
			;
	else
	{
		while(xml->read())
		{
			switch (xml->getNodeType())
			{
				case irr::io::EXN_ELEMENT:
				{
					if (JobRaceTag.equals_ignore_case(xml->getNodeName()))
					{
						m_JobRaceList.push_back(JobRace());
						(m_JobRaceList.end() - 1)->SetProject(this);
						(m_JobRaceList.end() - 1)->ReadXML(xml);
					}
					break;
				}
				default:
					break;
			}
		}
		xml->drop();
	}
	// GUI loading
	xml = GetIrrlicht::Device()->getFileSystem()->createXMLReader(m_GUIListFile.c_str());
	if (!xml)
			;
	else
	{
		while(xml->read())
		{
			switch (xml->getNodeType())
			{
				case irr::io::EXN_ELEMENT:
				{
					if (SelectorTag.equals_ignore_case(xml->getNodeName()))
					{
						m_Selector.ReadXML(xml);
					} else if (SelectableTileTag.equals_ignore_case(xml->getNodeName()))
					{
						m_SelectableTile.ReadXML(xml);
					}
					break;
				}
				default:
					break;
			}
		}
		xml->drop();
	}
	// Battlefield loading
	xml = GetIrrlicht::Device()->getFileSystem()->createXMLReader(m_BattlefieldListFile.c_str());
	if (!xml)
			;
	else
	{
		while(xml->read())
		{
			switch (xml->getNodeType())
			{
				case irr::io::EXN_ELEMENT:
				{
					if (BattleFieldTag.equals_ignore_case(xml->getNodeName()))
					{
						m_BattlefieldList.push_back(BattleField());
						(m_BattlefieldList.end() - 1)->SetProject(this);
						(m_BattlefieldList.end() - 1)->ReadXML(xml);
					}
				}
				default:
					break;
			}
		}
		xml->drop();
	}
}

Job* Project::GetJob(stringw name)
{
	vector<Job>::iterator i = std::find(m_JobList.begin(), m_JobList.end(), name);
	if (i != m_JobList.end())
	{
		return &(*i);
	} else return NULL;
}

Race* Project::GetRace(stringw name)
{
	vector<Race>::iterator i = std::find(m_RaceList.begin(), m_RaceList.end(), name);
	if (i != m_RaceList.end())
	{
		return &(*i);
	} else return NULL;
}

// XML
void Project::ReadXML(IXMLReader* xml)
{
	do 
	{
		switch (xml->getNodeType())
		{
			case irr::io::EXN_ELEMENT:
			{
				if (ProjectTag.equals_ignore_case(xml->getNodeName()))
				{
					break;
				} else if (JobListTag.equals_ignore_case(xml->getNodeName()))
				{
					m_JobListFile = xml->getAttributeValueSafe(L"FileName");
				} else if (RaceListTag.equals_ignore_case(xml->getNodeName()))
				{
					m_RaceListFile = xml->getAttributeValueSafe(L"FileName");
				} else if (BattleFieldListTag.equals_ignore_case(xml->getNodeName()))
				{
					m_BattlefieldListFile = xml->getAttributeValueSafe(L"FileName");
				}  else if (JobRaceListTag.equals_ignore_case(xml->getNodeName()))
				{
					m_JobRaceListFile = xml->getAttributeValueSafe(L"FileName");
				} else if (GUIListTag.equals_ignore_case(xml->getNodeName()))
				{
					m_GUIListFile = xml->getAttributeValueSafe(L"FileName");
				} else return;
				break;
			}
			case irr::io::EXN_ELEMENT_END:
				if (ProjectTag.equals_ignore_case(xml->getNodeName())) return;
				break;
			default:
				break;			
		}
	} while (xml->read());
}

void Project::WriteXML(IXMLWriter* xml)
{
	
}
