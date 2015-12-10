#ifndef HITDATABLOCK_H
#define HITDATABLOCK_H

#include "Unit.h"

class HitDataBlock
{
	public:
		HitDataBlock();
		virtual ~HitDataBlock();
		void SetData(HitData theData);
		void Visible(bool visible);
	protected:
	private:
		IBillboardTextSceneNode* m_DamageText;
		IBillboardTextSceneNode* m_HitChanceText;
};

#endif // HITDATABLOCK_H
