#pragma once

#include <OficinaFramework/OficinaTypes.hpp>
#include <OficinaFramework/EntitySystem.hpp>
#include <vector>

class Grid
{
private:
	vec2 cellSize;
	vec2dw gridSize;
public:
	Grid(vec2);
	~Grid();

	vec2 getCellSize();
	vec2 getGridSize();
};