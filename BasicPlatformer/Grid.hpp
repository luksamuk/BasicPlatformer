#pragma once

#include <OficinaFramework\OficinaTypes.hpp>
#include <OficinaFramework\EntitySystem.hpp>
#include <vector>

class Grid
{
private:
	vec2 cellSize;
	vec2dw gridSize;
	std::vector<OficinaFramework::EntitySystem::Entity*>** gridMatrix;

	void _insert(OficinaFramework::EntitySystem::Entity*, int x, int y);
public:
	Grid(vec2);
	~Grid();

	void Populate(std::vector<OficinaFramework::EntitySystem::Entity*>);
	void Populate(OficinaFramework::EntitySystem::EntityCollection*);
	void Populate(OficinaFramework::EntitySystem::DrawableEntityCollection*);
	void Move(OficinaFramework::EntitySystem::Entity*, vec2 oldPosition, vec2 newPosition);

	std::vector<OficinaFramework::EntitySystem::Entity*>* GetNearest(vec2 Position);

	vec2 getCellSize();
	vec2 getGridSize();
};