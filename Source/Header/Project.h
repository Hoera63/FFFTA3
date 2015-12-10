#ifndef PROJECT_H
#define PROJECT_H

#include "BattleField.h"
#include "JobRace.h"

static const stringw ProjectTag(L"FFFTA3Project");
static const stringw JobListTag(L"JobList");
static const stringw RaceListTag(L"RaceList");
static const stringw JobRaceListTag(L"JobRaceList");
static const stringw BattleFieldListTag(L"BattleFieldList");
static const stringw GUIListTag(L"GUIList");
static const stringw TestTag(L"Test");

class Project : XMLObject
{
	public:
		Project();
		virtual ~Project();
		void LoadData();
		Job* GetJob(stringw name);
		vector<Job>* JobList() { return &m_JobList; }
		Race* GetRace(stringw name);
		vector<Race>* RaceList() { return &m_RaceList; }
		vector<JobRace>* JobRaceList() { return &m_JobRaceList; }
		Selector GetSelector() { return m_Selector; }
		SelectableTile GetSelectableTile() { return m_SelectableTile; }
// XML
		void ReadXML(IXMLReader* xml);
		void WriteXML(IXMLWriter* xml);
	protected:
	private:
		stringw m_JobListFile;
		stringw m_RaceListFile;
		stringw m_JobRaceListFile;
		stringw m_BattlefieldListFile;
		stringw m_GUIListFile;
		stringw m_TestFile;
		Selector m_Selector;
		SelectableTile m_SelectableTile;
		vector<Job> m_JobList;
		vector<Race> m_RaceList;
		vector<JobRace> m_JobRaceList;
		vector<BattleField> m_BattlefieldList;
};

#endif // PROJECT_H
