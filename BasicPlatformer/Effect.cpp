#include "Effect.hpp"
using namespace OficinaFramework;

Effect::Effect(EffectType type, RenderingSystem::SpriteSheet* sheet, EntitySystem::Entity* follow,
	 vec2 hotspot, float alpha, std::function<void(vec2&, vec2&, bool&)> behavior)
{
	m_type = type;
	m_sheet = sheet;
	m_anim = nullptr;
	m_follow = follow;
	m_hotspot = hotspot;
	m_alpha = alpha;
	m_particlebehavior = behavior;

	switch(m_type) {
	default: break;
	case FX_SPLASH:        SetName("SplashEffect");      break;
	case FX_SPARK:         SetName("SparkEffect");       break;
	case FX_SMALLBUBBLE:   SetName("SmallBubbleEffect"); break;
	case FX_NORMALSHIELD:
	case FX_BUBBLESHIELD:
		SetName("ShieldEffect");
		break;
	}
}

void Effect::Initialize()
{
}

void Effect::Update()
{
	if (m_follow) {
		vec2 m_relativepos;
		m_particlebehavior(m_relativepos, m_velocity, m_destroy);
		SetPosition(m_follow->GetPosition() + (m_hotspot + m_relativepos));
	}
	else m_particlebehavior(m_position, m_velocity, m_destroy);

	m_anim->update();

	switch(m_type)
	{
	default: break;
	case FX_SPLASH:
		if (m_anim->GetCurrentFrame() == 6)
			m_destroy = true;
		break;
	case FX_SPARK:
		if (m_anim->GetCurrentFrame() == 10)
			m_destroy = true;
		break;
	}
	if (m_destroy) RemoveMe();
}

void Effect::LoadContent()
{
	m_anim = new RenderingSystem::Animation(m_sheet);
	m_anim->RegisterAnimation("SPLASH", RenderingSystem::Animation::AnimationSpecs( 0,  6, 0.035f));
	m_anim->RegisterAnimation( "SPARK", RenderingSystem::Animation::AnimationSpecs( 7, 10, 0.03f));
	m_anim->RegisterAnimation("SMALLBUBBLE", RenderingSystem::Animation::AnimationSpecs(34, 36, 36, 0.04f));
	m_anim->RegisterAnimation("NORMALSHIELD", RenderingSystem::Animation::AnimationSpecs(13, 15, 0.04f));
	m_anim->RegisterAnimation("BUBBLESHIELD", RenderingSystem::Animation::AnimationSpecs(16, 33, 0.04f));
	m_anim->SetSyncToFramerate(true);
	m_anim->SetAlpha(m_alpha);

	switch(m_type) {
	default: break;
	case FX_SPLASH:  m_anim->SetAnimation("SPLASH");  break;
	case FX_SPARK:   m_anim->SetAnimation("SPARK");   break;
	case FX_SMALLBUBBLE:   m_anim->SetAnimation("SMALLBUBBLE");   break;
	case FX_NORMALSHIELD:  m_anim->SetAnimation("NORMALSHIELD");  break;
	case FX_BUBBLESHIELD:  m_anim->SetAnimation("BUBBLESHIELD");  break;
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
