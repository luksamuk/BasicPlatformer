#pragma once
#include <OficinaFramework/EntitySystem.hpp>

enum EffectType
{
	FX_SPLASH,
	FX_SKIDDING,
	FX_SPINDASH
};

class Effect : public OficinaFramework::EntitySystem::DrawableEntity
{
private:
	EffectType m_type;
public:
	Effect(EffectType type);

	// Inherited via DrawableEntity
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void LoadContent() override;
	virtual void UnloadContent() override;
	virtual void Draw() override;
};