#ifndef SELECTABLETILE_H
#define SELECTABLETILE_H

#include "BattleFieldPosition.h"
#include "AnimatingObject.h"

static const stringw SelectableTileTag(L"SelectableTile");

class SelectableTile : public AnimatingObject
{
	public:
		SelectableTile();
		virtual ~SelectableTile();
		void Init();
		BattleFieldPosition BFPosition() { return m_Pos; };
		void BFPosition(BattleFieldPosition BFPos) { m_Pos = BFPos; };
// XML
		void WriteXML(IXMLWriter* xml);
		void ReadXML(IXMLReader* xml);
	protected:
	private:
		BattleFieldPosition m_Pos;
};

#endif // SELECTABLETILE_H
