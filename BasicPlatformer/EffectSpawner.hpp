#pragma once
#include <OficinaFramework\EntitySystem.hpp>
#include "Effect.hpp"

class EffectSpawner : public OficinaFramework::EntitySystem::Entity
{
private:
	OficinaFramework::EntitySystem::DrawableEntityCollection* m_effectCollection;
public:
	EffectSpawner();
	// Inherited via Entity
	virtual void Initialize() override;
	virtual void Update() override;

	void LoadContent();
	void UnloadContent();

	void Create(EffectType, vec2);
	//void Create(EffectType, OficinaFramework::EntitySystem::Entity*);
};