#pragma once
#include <OficinaFramework\EntitySystem.hpp>

class EffectSpawner : public OficinaFramework::EntitySystem::Entity
{
private:
public:
	// Inherited via Entity
	virtual void Initialize() override;
	virtual void Update() override;

	void LoadContent();
	void UnloadContent();
};