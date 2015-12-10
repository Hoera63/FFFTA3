#ifndef UNIT_H
#define UNIT_H

#include <vector>
#include <algorithm>

#include "BattleFieldPosition.h"
#include "NamedObject.h"
#include "AnimatingObject.h"

using std::vector;

enum FacingSide
{
	North = 0,
	East = 1,
	West = 2,
	South = 3,
};

struct HitData
{
	int HitRate;
	int Damage;
};

static const stringw UnitTag(L"Unit");

class Unit : public AnimatingObject, public NamedObject
{
	public:
		Unit(int hp = 50, int mp = 25, int move = 4, int jump = 2, int evade = 10, int baseAttack = 120, int baseDefense = 100, int baseMagicAttack = 120, int baseMagicDefense = 100, int jp = 0, int level = 1, int xp = 0, bool controlAI = false);
		virtual ~Unit();
		BattleFieldPosition BFPosition() { return m_pos; };
		void BFPosition(BattleFieldPosition BFPos) { m_pos = BFPos; };
		int CurrentHP() { return m_currentHP; };
		void CurrentHP(int value) { m_currentHP = value; };
		int CurrentMP() { return m_currentMP; };
		void CurrentMP(int value) { m_currentMP = value; };
		int HP() { return m_hp; };
		void HP(int value) { m_hp = value; };
		int MP() { return m_mp; };
		void MP(int value) { m_mp = value; };
		int Move() { return m_move; };
		void Move(int value) { m_move = value; };
		int Jump() { return m_jump; };
		void Jump(int value) { m_jump = value; };
		int Evade() { return m_evade; };
		void Evade(int value) { m_evade = value; };
		int BaseAttack() { return m_baseAttack; };
		void BaseAttack(int value) { m_baseAttack = value; };
		int BaseDefense() { return m_baseDefense; };
		void BaseDefense(int value) { m_baseDefense = value; };
		int BaseMagicAttack() { return m_baseMagicAttack; };
		void BaseMagicAttack(int value) { m_baseMagicAttack = value; };
		int BaseMagicDefense() { return m_baseMagicDefense; };
		void BaseMagicDefense(int value) { m_baseMagicDefense = value; };
		int Speed() { return m_speed; };
		void Speed(int value) { m_speed = value; };
		int JP() { return m_jp; };
		void JP(int value) { m_jp = value; };
		int Level() { return m_level; };
		void Level(int value) { m_level = value; };
		int XP() { return m_xp; };
		void XP(int value) { m_xp = value; };
		void Update(float deltaTime);
		void NextMovingStep();
		int TurnCounter() const { return m_turnCounter; };
		void TurnCounter(int value) { m_turnCounter = value; };
		void ReserveFill(int value);
		void TurnPassed(bool walk, bool action);
		bool ClockTick();
		bool AIControlled() { return m_AIControl; };
		void AIControlled(bool controlAI) { m_AIControl = controlAI; };
		void OrderMove(const vector<vector3di>& thePath);
		FacingSide Facing();
		void Facing(FacingSide face, stringw animationName = L"Walking");
		bool Walking() { return m_walking; };
		void Walking(bool walking) { m_walking = walking; };
		void Attack();
		void Attacked(HitData theData);
		bool Attacking() { return m_attacking; }
		void Attacking(bool attacking) { m_attacking = attacking; };
		bool Hurting() { return m_hurting; }
		void Hurting(bool hurting) { m_hurting = hurting; };
		HitData GetDamageData(int action, Unit* theUnit);
		//vector<> GetActionList();
		vector<stringw> GetActionListAsString();
		
// XML
		void WriteXML(IXMLWriter* xml);
		void ReadXML(IXMLReader* xml);
		static float moveSpeed;
	protected:
	private:
		int m_currentHP;
		int m_currentMP;
		int m_hp;
		int m_mp;
		int m_move;
		int m_jump;
		int m_evade;
		int m_baseAttack;
		int m_baseDefense;
		int m_baseMagicAttack;
		int m_baseMagicDefense;
		int m_speed;
		int m_jp;
		int m_level;
		int m_xp;
		BattleFieldPosition m_pos;
		int m_turnCounter;
		int m_reserve;
		bool m_AIControl;
		bool m_walking;
		bool m_attacking;
		bool m_hurting;
		vector<vector3di> m_path;
		
};

#endif // UNIT_H
