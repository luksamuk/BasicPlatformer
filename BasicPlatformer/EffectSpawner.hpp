#pragma once
#include <OficinaFramework/EntitySystem.hpp>
#include <OficinaFramework/RenderingSystem.hpp>
#include "Effect.hpp"

class EffectSpawner
{
private:
	OficinaFramework::EntitySystem::DrawableEntityCollection* m_effectCollection;
	OficinaFramework::RenderingSystem::Texture* m_effects;
	OficinaFramework::RenderingSystem::SpriteSheet* m_sheet;
public:
	EffectSpawner();

	void LoadContent();
	void UnloadContent();

	void Create(EffectType, vec2, float alpha = 1.0f);
	void Create(EffectType, OficinaFramework::EntitySystem::Entity*, vec2 hotspot = vec2::Zero(), float alpha = 1.0f);

	void setCollection(OficinaFramework::EntitySystem::DrawableEntityCollection*);
};