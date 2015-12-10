#ifndef ANIMATINGOBJECT_H
#define ANIMATINGOBJECT_H

#include "UpdatingObject.h"
#include "AnimatedSprite.h"

using irr::core::stringw;

static const stringw AnimatingObjectTag(L"AnimatingObject");

class AnimatingObject : public UpdatingObject, XMLObject
{
	public:
		AnimatingObject();
		virtual ~AnimatingObject();
		AnimatedSprite& Sprite() { return m_Sprite; }
		void Sprite(AnimatedSprite val) { m_Sprite = val; }
		vector<Animation>& Animations() { return m_Animations; }
		void Animations(vector<Animation> val) { m_Animations = val; }
		void PlayAnimation(stringw name, bool mirrored = false, bool loop = true);
		void PlayAnimation(int number, bool mirrored = false, bool loop = true);
		void Update(float deltaTime);
// XML
		void WriteXML(IXMLWriter* xml);
		void ReadXML(IXMLReader* xml);
	protected:
	private:
		AnimatedSprite m_Sprite;
		vector<Animation> m_Animations;
};

#endif // ANIMATINGOBJECT_H
