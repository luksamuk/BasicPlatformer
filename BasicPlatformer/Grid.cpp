#include "Grid.hpp"
#include <cmath>
#include <algorithm>
using namespace OficinaFramework;

Grid::Grid(vec2 cellsize) {
	cellSize = cellsize;
}

Grid::~Grid() {
}

vec2 Grid::getCellSize() {
	return cellSize;
}

vec2 Grid::getGridSize() {
	return gridSize.toVec2();
}
