#include "EffectSpawner.hpp"
using namespace OficinaFramework;

EffectSpawner::EffectSpawner()
{
	m_effectCollection = nullptr;
}

void EffectSpawner::LoadContent()
{
	m_effects = RenderingSystem::TexturePool::LoadTexture("sprites/effects");
	m_sheet = new RenderingSystem::SpriteSheet(vec2dw(48u), vec2b::One(), vec2::Zero());
	m_sheet->AppendTexture(m_effects);
}

void EffectSpawner::UnloadContent()
{
	delete m_sheet;
	RenderingSystem::TexturePool::DisposeTexture(m_effects);
}

OficinaFramework::EntitySystem::DrawableEntity*
	EffectSpawner::Create(EffectType fxt, vec2 pos, float alpha, std::function<void(vec2&, vec2&, bool&)> behavior)
{
	if(m_effectCollection == nullptr)
	{
		OF_Log(OF_LOG_LVL_ERROR, "EffectSpawner's collection not defined!\n");
		return nullptr;
	}
	Effect* fx = new Effect(fxt, m_sheet, nullptr, vec2::Zero(), alpha, behavior);
	fx->SetPosition(pos);
	m_effectCollection->Add(fx);
	return fx;
}

OficinaFramework::EntitySystem::DrawableEntity*
	EffectSpawner::Create(EffectType fxt, EntitySystem::Entity* follow, vec2 hotspot, float alpha, std::function<void(vec2&, vec2&, bool&)> behavior)
{
	if(m_effectCollection == nullptr)
	{
		OF_Log(OF_LOG_LVL_ERROR, "EffectSpawner's collection not defined!\n");
		return nullptr;
	}
	Effect* fx = new Effect(fxt, m_sheet, follow, hotspot, alpha, behavior);
	if(follow)
		fx->SetPosition(follow->GetPosition() + hotspot);
	m_effectCollection->Add(fx);
	return fx;
}

void EffectSpawner::setCollection(EntitySystem::DrawableEntityCollection* col)
{
	m_effectCollection = col;
}
