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
}

void EffectSpawner::Create(EffectType fxt, vec2 pos)
{
	if(m_effectCollection == nullptr)
	{
		OF_Log(OF_LOG_LVL_ERROR, "EffectSpawner's collection not defined!\n");
		return;
	}
	Effect* fx = new Effect(fxt, m_sheet);
	fx->SetPosition(pos);
	m_effectCollection->Add(fx);
}

void EffectSpawner::setCollection(EntitySystem::DrawableEntityCollection* col)
{
	m_effectCollection = col;
}
