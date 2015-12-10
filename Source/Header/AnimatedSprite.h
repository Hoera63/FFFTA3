#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H

#include <vector>

#include "GetIrrlicht.h"
#include "Animation.h"

using std::vector;
using irr::core::vector2df;
using irr::core::vector3df;
using irr::core::dimension2d;
using irr::core::matrix4;
using irr::core::stringw;
using irr::scene::IBillboardSceneNode;
using irr::scene::ISceneNode;
using irr::video::ITexture;

class AnimatedSprite
{
	public:
		AnimatedSprite();
		virtual ~AnimatedSprite();
		Animation* CurrentAnimation() { return m_CurrentAnimation; }
		void CurrentAnimation(Animation* newAnimation, bool mirrored = false, bool loop = true);
		void RestoreAnimationData() { m_CurrentFrame = 0.0f; RestoreFPS(); m_Mirrored = false; }
		float CurrentFrame() { return m_CurrentFrame; }
		void CurrentFrame(float val) { m_CurrentFrame = val; }
		float CurrentFPS() { return m_FPS; }
		void CurrentFPS(float val) { m_FPS = val; }
		bool Mirrored() { return m_Mirrored; }
		void Mirrored(bool val) { m_Mirrored = val; }
		bool Looping() { return m_Loop; }
		void Looping(bool val) { m_Loop = val; }
		void RestoreFPS() { m_FPS = m_CurrentAnimation->FPS(); }
		IBillboardSceneNode* Node() { return m_Node; }
		void Node(IBillboardSceneNode* val) { m_Node = val; }
		void Update(float deltaTime);
		void Pause(float pauseTime) { m_PauseTime = pauseTime; }
		float Pause() { return m_PauseTime; }
		void CreateNode(float width = 17.f*3, float height = 31.f*3);
		void NodeSize(float width = 17.f*3, float height = 31.f*3);
	protected:
	private:
		Animation* m_CurrentAnimation;
		float m_CurrentFrame;
		float m_PauseTime;
		float m_FPS;
		IBillboardSceneNode* m_Node;
		matrix4 m_Matrix;
		bool m_Mirrored;
		bool m_Loop;
};

#endif // ANIMATEDSPRITE_H
