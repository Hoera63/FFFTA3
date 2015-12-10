#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>

#include "NamedObject.h"
#include "GetIrrlicht.h"

using std::vector;
using irr::video::ITexture;
using irr::core::vector2df;

static const stringw AnimationTag(L"Animation");
static const stringw AnimationFrameTag(L"Frame");

class Animation : public NamedObject
{
	public:
		Animation();
		virtual ~Animation();
		ITexture* Texture() { return m_Texture; }
		void Texture(ITexture* val) { m_Texture = val; }
		float FPS() { return m_FPS; }
		void FPS(float val) { m_FPS = val; }
		vector2df FramePosition() { return m_FramePosition; }
		void FramePosition(vector2df val) { m_FramePosition = val; }
		vector2df FrameSize() { return m_FrameSize; }
		void FrameSize(vector2df val) { m_FrameSize = val; }
		vector<unsigned int>& FrameOrder() { return m_FrameOrder; }
		void FrameOrder(vector<unsigned int> val) { m_FrameOrder = val; }
		unsigned int TotalFrames() { return m_FrameOrder.size(); }
// XML
		void ReadXML(IXMLReader* xml);
		void WriteXML(IXMLWriter* xml);
	protected:
	private:
		ITexture* m_Texture;
		float m_FPS;
		vector2df m_FramePosition;
		vector2df m_FrameSize;
		vector<unsigned int> m_FrameOrder;
};

#endif // ANIMATION_H
