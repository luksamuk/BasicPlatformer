#pragma once
#include <OficinaFramework/EntitySystem.hpp>
#include <OficinaFramework/RenderingSystem.hpp>
#include "Effect.hpp"
#include <functional>

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

	OficinaFramework::EntitySystem::DrawableEntity*
		Create(EffectType, vec2, float alpha = 1.0f,
			std::function<void(vec2&, vec2&, bool&)> behavior = [](vec2& pos, vec2& vel, bool& destroy) {});

	OficinaFramework::EntitySystem::DrawableEntity*
		Create(EffectType, OficinaFramework::EntitySystem::Entity*, vec2 hotspot = vec2::Zero(), float alpha = 1.0f,
			std::function<void(vec2&, vec2&, bool&)> behavior = [](vec2& relativePos, vec2& vel, bool& destroy) {});

	void setCollection(OficinaFramework::EntitySystem::DrawableEntityCollection*);
};