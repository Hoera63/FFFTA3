#ifndef SELECTOR_H
#define SELECTOR_H

#include "Unit.h"

static const stringw SelectorTag(L"Selector");

class Selector : public AnimatingObject
{
	public:
		Selector();
		virtual ~Selector();
		BattleFieldPosition BFPosition() { return m_pos; };
		void BFPosition(BattleFieldPosition BFPos) { m_pos = BFPos; };
		Unit* SelectedUnit() { return m_selectedUnit; };
		void SelectedUnit(Unit* unit) { m_selectedUnit = unit; m_pos = unit->BFPosition(); };
// XML
		void WriteXML(IXMLWriter* xml);
		void ReadXML(IXMLReader* xml);
	protected:
	private:
		BattleFieldPosition m_pos;
		Unit* m_selectedUnit;
		bool m_selecting;
};

#endif // SELECTOR_H
