#ifndef JOBRACE_H
#define JOBRACE_H

#include <vector>

#include "Job.h"
#include "Race.h"
#include "Animation.h"

using std::vector;

static const stringw JobRaceTag(L"JobRace");

class Project;

class JobRace : public XMLObject
{
	public:
		JobRace();
		virtual ~JobRace();
		const Job* GetJob() const { return m_Job; }
		void SetJob(Job* val) { m_Job = val; }
		const Race* GetRace() const { return m_Race; }
		void SetRace(Race* val) { m_Race = val; }
		void SetProject(Project* theProject);
		void WriteXML(IXMLWriter* xml);
		void ReadXML(IXMLReader* xml);
	protected:
	private:
		Job* m_Job;
		Race* m_Race;
		Project* m_Project;
		vector<Animation> m_AnimationList;
};

#endif // JOBRACE_H
