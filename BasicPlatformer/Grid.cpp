#include "Grid.hpp"
#include <cmath>
#include <algorithm>
using namespace OficinaFramework;

void Grid::_insert(OficinaFramework::EntitySystem::Entity* entity, int x, int y)
{
	if (!gridMatrix) {
		OF_Log(OF_LOG_LVL_ERROR, "Cannot add entity to grid: grid is null\n");
		return;
	}
	gridMatrix[x][y].push_back(entity);
	//printf("Inserting at %d, %d\n", x, y);
}

Grid::Grid(vec2 cellsize) {
	cellSize = cellsize;
	gridMatrix = nullptr;
}

Grid::~Grid() {
	if (gridMatrix) {
		for (dword i = 0u; i < gridSize.x; i++)
			delete[] gridMatrix[i];
		delete[] gridMatrix;
	}
}

void Grid::Populate(std::vector<OficinaFramework::EntitySystem::Entity*> entitycollection)
{
	if (!gridMatrix) {
		vec2 max;
		for (auto entity : entitycollection) {
			vec2 pos = entity->GetPosition();
			if (pos.x > max.x) max.x = pos.x;
			if (pos.y > max.y) max.y = pos.y;

			gridSize.x = dword(ceil(max.x));
			gridSize.y = dword(ceil(max.y));
		}

		gridMatrix = new std::vector<EntitySystem::Entity*>*[gridSize.x];
		for (dword i = 0u; i < gridSize.x; i++)
			gridMatrix[i] = new std::vector<EntitySystem::Entity*>[gridSize.y];
	}

	for (auto entity : entitycollection) {
		vec2 pos = entity->GetPosition();
		_insert(entity, int(floor(pos.x / cellSize.x)), int(floor(pos.y / cellSize.y)));
	}
}

void Grid::Populate(OficinaFramework::EntitySystem::EntityCollection* entitycollection)
{
	if (!gridMatrix) {
		vec2 max;
		for (auto entity : *entitycollection) {
			vec2 pos = entity->GetPosition();
			if (pos.x > max.x) max.x = pos.x;
			if (pos.y > max.y) max.y = pos.y;

			gridSize.x = dword(ceil(max.x));
			gridSize.y = dword(ceil(max.y));
		}

		gridMatrix = new std::vector<EntitySystem::Entity*>*[gridSize.x];
		for (dword i = 0u; i < gridSize.x; i++)
			gridMatrix[i] = new std::vector<EntitySystem::Entity*>[gridSize.y];
	}

	for (auto entity : *entitycollection) {
		vec2 pos = entity->GetPosition();
		_insert(entity, int(floor(pos.x / cellSize.x)), int(floor(pos.y / cellSize.y)));
	}
}

void Grid::Populate(OficinaFramework::EntitySystem::DrawableEntityCollection* entitycollection)
{
	if (!gridMatrix) {
		vec2 max;
		for (auto entity : *entitycollection) {
			vec2 pos = entity->GetPosition();
			if (pos.x > max.x) max.x = pos.x;
			if (pos.y > max.y) max.y = pos.y;

			gridSize.x = dword(ceil(max.x));
			gridSize.y = dword(ceil(max.y));
		}

		gridMatrix = new std::vector<EntitySystem::Entity*>*[gridSize.x];
		for (dword i = 0u; i < gridSize.x; i++)
			gridMatrix[i] = new std::vector<EntitySystem::Entity*>[gridSize.y];
	}

	for (auto entity : *entitycollection) {
		vec2 pos = entity->GetPosition();
		_insert(entity, int(floor(pos.x / cellSize.x)), int(floor(pos.y / cellSize.y)));
	}
}

void Grid::Move(OficinaFramework::EntitySystem::Entity* entity, vec2 oldPosition, vec2 newPosition) {
	if (!gridMatrix) {
		OF_Log(OF_LOG_LVL_ERROR, "Cannot move object: grid is null\n");
		return;
	}
	
	auto oldVec = GetNearest(oldPosition);
	if (!oldVec) {
		OF_Log(OF_LOG_LVL_ERROR, "Cannot move object: object is not on grid\n");
		return;
	}

	auto it = std::find(oldVec->begin(), oldVec->end(), entity);
	if (it == oldVec->end()) {
		OF_Log(OF_LOG_LVL_ERROR, "Cannot move object: object is not on grid\n");
		return;
	}

	auto newVec = GetNearest(newPosition);
	if (!newVec) {
		OF_Log(OF_LOG_LVL_ERROR, "Cannot move object: object is outside of grid\n");
		return;
	}

	oldVec->erase(it);
	newVec->push_back(entity);
}

std::vector<OficinaFramework::EntitySystem::Entity*>* Grid::GetNearest(vec2 Position)
{
	if (!gridMatrix) {
		OF_Log(OF_LOG_LVL_ERROR, "Cannot get nearest entities: grid is null\n");
		return nullptr;
	}
	vec2dw nearest;
	nearest.x = dword(floor(Position.x / cellSize.x));
	nearest.y = dword(floor(Position.y / cellSize.y));

	if (nearest.x >= gridSize.x || nearest.y >= gridSize.y) {
		OF_Log(OF_LOG_LVL_ERROR, "Cannot get nearest entities: object is outside of grid\n");
		return nullptr;
	}

	//printf("Returning nearest at %d, %d\n", nearest.x, nearest.y);

	// TODO: A fix here would be nice. The way Sonic's sensors works, they can be
	// outside of a cell and not detect anything.
	// Also, Sonic can be between two cells! This will depend on Sonic's sizes,
	// in the end.
	return &gridMatrix[nearest.x][nearest.y];
}

vec2 Grid::getCellSize() {
	return cellSize;
}

vec2 Grid::getGridSize() {
	return gridSize.toVec2();
}
