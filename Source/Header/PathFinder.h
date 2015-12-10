#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "Unit.h"

class BattleField;

enum PathData
{
	MoveImpossible = 0,
	MoveNorth,
	MoveNorthDouble,
	MoveEast,
	MoveEastDouble,
	MoveSouth,
	MoveSouthDouble,
	MoveWest,
	MoveWestDouble
};

class PathFinder
{
	public:
		PathFinder();
		virtual ~PathFinder();
		void SetBattleField(BattleField* theBattleField) { m_BattleField = theBattleField; };
		void FindPaths(Unit* theUnit);
		vector< vector<PathData> >& GetPathData() { return m_PathData; };
	protected:
	private:
		bool MoveCheck(int xOrigin, int yOrigin, int xTarget, int yTarget);
		BattleField* m_BattleField;
		Unit* m_Unit;
		vector< vector<PathData> > m_PathData;
		vector< vector<int> > m_MoveData;
		int m_MoveLeft;
};

#endif // PATHFINDER_H
