#pragma once
#include <OficinaFramework/EntitySystem.hpp>
#include <OficinaFramework/RenderingSystem.hpp>
#include <functional>

enum EffectType
{
	FX_SPLASH,
	FX_SPARK,
	FX_SMALLBUBBLE,
	FX_NORMALSHIELD,
	FX_BUBBLESHIELD//,
	//FX_SKIDDING,
	//FX_SPINDASH
};

class Effect : public OficinaFramework::EntitySystem::DrawableEntity
{
private:
	EffectType m_type;
	OficinaFramework::RenderingSystem::SpriteSheet* m_sheet;
	OficinaFramework::RenderingSystem::Animation*   m_anim;
	OficinaFramework::EntitySystem::Entity*         m_follow;
	vec2                                            m_hotspot;
	float                                           m_alpha;
	bool                                            m_destroy = false;
	vec2                                            m_velocity;

	std::function<void(vec2& Position, vec2& Velocity, bool& DestructionFlag)> m_particlebehavior;
public:
	Effect(EffectType type, OficinaFramework::RenderingSystem::SpriteSheet*,
	                  OficinaFramework::EntitySystem::Entity* follow = nullptr,
					  vec2 hotspot = vec2::Zero(),
					  float alpha = 1.0f,
					  std::function<void(vec2&, vec2&, bool&)> behavior =
									[](vec2& pos, vec2& vel, bool& destroy){});

	// Inherited via DrawableEntity
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void LoadContent() override;
	virtual void UnloadContent() override;
	virtual void Draw() override;
};