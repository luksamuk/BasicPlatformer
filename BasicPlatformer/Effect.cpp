#include "Effect.hpp"
using namespace OficinaFramework;

Effect::Effect(EffectType type, RenderingSystem::SpriteSheet* sheet, EntitySystem::Entity* follow)
{
	m_type = type;
	m_sheet = sheet;
	m_anim = nullptr;
	m_follow = follow;

	switch(m_type) {
	default: break;
	case FX_SPLASH:  SetName("SplashEffect");  break;
	case FX_SPARK:   SetName("SparkEffect");   break;
	case FX_SHIELD:  SetName("ShieldEffect");  break;
	}
}

void Effect::Initialize()
{
}

void Effect::Update()
{
	if(m_follow)
		SetPosition(m_follow->GetPosition());
	m_anim->update();

	switch(m_type)
	{
	default: break;
	case FX_SPLASH:
		if(m_anim->GetCurrentFrame() == 6)
		{
			RemoveMe();
		}
		break;
	case FX_SPARK:
		if(m_anim->GetCurrentFrame() == 10)
		{
			RemoveMe();
		}
		break;
	}
}

void Effect::LoadContent()
{
	m_anim = new RenderingSystem::Animation(m_sheet);
	m_anim->RegisterAnimation("SPLASH", RenderingSystem::Animation::AnimationSpecs( 0,  6, 0.035f));
	m_anim->RegisterAnimation( "SPARK", RenderingSystem::Animation::AnimationSpecs( 7, 10, 0.03f));
	m_anim->RegisterAnimation("SHIELD", RenderingSystem::Animation::AnimationSpecs(13, 15, 0.04f));
	m_anim->SetSyncToFramerate(true);

	switch(m_type) {
	default: break;
	case FX_SPLASH:  m_anim->SetAnimation("SPLASH");  break;
	case FX_SPARK:   m_anim->SetAnimation("SPARK");   break;
	case FX_SHIELD:  m_anim->SetAnimation("SHIELD");  break;
	}
}

void Effect::UnloadContent()
{
	m_anim->dispose();
	delete m_anim;
}

void Effect::Draw()
{
	m_anim->Draw(m_position, 1.0f, Color4::MaskToColor4(WHITE));
}
