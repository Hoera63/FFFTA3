#ifndef BATTLEFIELDPOSITION_H
#define BATTLEFIELDPOSITION_H

#include "XMLObject.h"

using irr::core::vector3df;
using irr::core::stringw;

static const stringw BattleFieldPositionTag(L"BattleFieldPosition");

class BattleFieldPosition : XMLObject
{
	public:
		BattleFieldPosition();
		BattleFieldPosition(float x, float y, float z);
		BattleFieldPosition(vector3df pos);
		virtual ~BattleFieldPosition();
		float X() { return position.X; }
		void X(float val) { position.X = val; }
		float Y() { return position.Y; }
		void Y(float val) { position.Y = val; }
		float Z() { return position.Z; }
		void Z(float val) { position.Z = val; }
		vector3df Position() { return position; };
		void Position(vector3df pos) { position = pos; };
		void ReadXML(IXMLReader* xml);
		void WriteXML(IXMLWriter* xml);
	protected:
	private:
		vector3df position;
};

#endif // BATTLEFIELDPOSITION_H
