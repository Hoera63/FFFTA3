#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include "UnitDataBlock.h"
#include "Selector.h"
#include "ActionMenuBlock.h"
#include "PathFinder.h"
#include "SelectableTile.h"
#include "HitDataBlock.h"
#include <iostream>
#include <sstream>
#include <algorithm>

using irr::core::vector2di;
using irr::core::vector3di;
using irr::core::vector3df;
using irr::core::dimension2df;
using std::vector;

static const stringw BattleFieldTag(L"Battlefield");
static const stringw HeightMapTag(L"HeightMap");
static const stringw OverlayTag(L"Overlay");
static const stringw OverlayZTag(L"Z");

class Project;

enum ControlState
{
	SelectorControl = 0,
	UnitControl = 1,
	TargetControl = 2,
	NoControl = 3,
	TextControl = 4,
	AIControl = 5
};

enum PlayerTurnState
{
	Start = 0,
	MoveControl = 1,
	MoveWaiting = 2,
	ActionChoose = 3,
	ActionAim = 4,
	ActionConfirm = 5,
	ActionWaiting = 6,
	ReactionWaiting = 7,
	MenuControl = 8,
	End = 9
};

class BattleField : public AnimatingObject, public NamedObject
{
	public:
		BattleField();
		virtual ~BattleField();
		void Init(vector3di tileSize, vector2di bottomTilePosition, Animation animation, vector< vector<int> > heightMap, vector2di spriteSize);
		void Activate();
		void Update(float deltaTime);
		void AddUnit(Unit unit);
		void RemoveUnit(Unit unit);
		void RemoveUnit(stringw unitName);
		vector<Unit>& Units() { return m_Units; };
		vector3df BFPositionToPosition(BattleFieldPosition battleFieldPosition, bool unit = true);
		int GetHeightByPos(int x, int y);
		Unit* GetUnitByPos(int x, int y);
		void AddOverlay(Animation overlay, float z);
		float GetScaleX();
		float GetScaleY();
		Selector& BFSelector() { return m_Selector; };
		unsigned int Rows() { return m_HeightMap[0].size(); };
		unsigned int Columns() { return m_HeightMap.size(); };
		static bool UnitSort(const Unit& i, const Unit& j);
		void SetProject(Project* theProject);
		Project* GetProject();
// XML
		void WriteXML(IXMLWriter* xml);
		void ReadXML(IXMLReader* xml);
	protected:
	private:
		void CheckSelectorMovement();
		vector3di m_TileSize;
		vector2di m_BottomTilePosition;
		vector2di m_TextureSize;
		vector< vector<int> > m_HeightMap;
		vector<Unit> m_Units;
		vector<AnimatingObject*> m_Overlayers;
		Selector m_Selector;
		UnitDataBlock m_LeftDataBlock;
		UnitDataBlock m_RightDataBlock;
		ControlState m_ControlState;
		Unit* m_ActiveUnit;
		BattleFieldPosition m_UnitOldPos;
		FacingSide m_UnitOldFac;
		bool m_UnitMoved;
		bool m_UnitActed;
		PlayerTurnState m_PlayerTurnState;
		ActionMenuBlock m_ActionMenuBlock;
		PathFinder m_PathFinder;
		vector<SelectableTile> m_SelectableTiles;
		HitData m_Hitdata;
		HitDataBlock m_HitDataBlock;
		vector<float> m_OverlayZs;
		Project* m_Project;
};

#endif // BATTLEFIELD_H
