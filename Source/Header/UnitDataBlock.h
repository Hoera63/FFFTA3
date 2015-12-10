#ifndef UNITDATABLOCK_H
#define UNITDATABLOCK_H

#include "Unit.h"

using irr::gui::IGUIFont;
using irr::core::dimension2df;

class UnitDataBlock : public AnimatingObject
{
	public:
		UnitDataBlock();
		virtual ~UnitDataBlock();
		Unit* DataUnit() { return m_Unit; };
		void DataUnit(Unit* theUnit, bool blue = true, bool left = true);
		void UpdateData(bool left = true);
		void Update(float deltaTime);
	protected:
	private:
		Unit* m_Unit;
		IBillboardTextSceneNode* m_NameText;
		IBillboardTextSceneNode* m_HPText;
		IBillboardTextSceneNode* m_MPText;
		IBillboardTextSceneNode* m_JPText;
		IBillboardTextSceneNode* m_LevelText;
		AnimatingObject m_UnitPicture;
};

#endif // UNITDATABLOCK_H
