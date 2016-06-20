#pragma once
#include <OficinaFramework/EntitySystem.hpp>
#include <OficinaFramework/RenderingSystem.hpp>

enum EffectType
{
	FX_SPLASH,
	FX_SPARK//,
	//FX_SHIELD,
	//FX_SKIDDING,
	//FX_SPINDASH
};

class Effect : public OficinaFramework::EntitySystem::DrawableEntity
{
private:
	EffectType m_type;
	OficinaFramework::RenderingSystem::SpriteSheet* m_sheet;
	OficinaFramework::RenderingSystem::Animation*   m_anim;
public:
	Effect(EffectType type, OficinaFramework::RenderingSystem::SpriteSheet*);

	// Inherited via DrawableEntity
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void LoadContent() override;
	virtual void UnloadContent() override;
	virtual void Draw() override;
};