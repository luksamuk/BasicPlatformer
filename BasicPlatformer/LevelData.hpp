#pragma once
#include <OficinaFramework/OficinaTypes.hpp>
#include <OficinaFramework/RenderingSystem.hpp>
#include <vector>
#include <string>
#include <optional>
#include "CollisionShape.hpp"

// LEVEL FOLDER HIERARCHY:
// -> assets/levels/rx
//    Level (round) folder for Level ID X
//    -> chunks (PNG texture)
//       Image data for tiles
//    -> tiles (TSX tile data)
//       Tile and collision data
//    -> zoneY (TMX map data)
//       Level map for Act Y

typedef std::vector<CollisionShape*> CollisionArray;

// Data scraped from .tsx files.
struct TileData {
    std::string version;
    std::string tiledversion;
    std::string name;
    dword tilewidth;
    dword tileheight;
    dword tilecount;
    dword columns;
    std::vector<CollisionArray*> collision;
};

struct LayerData {
    dword id;
    std::string name;
    dword width;
    dword height;
    std::vector<dword_s> data;
};

struct MapData {
    std::string version;
    std::string tiledversion;
    std::string orientation;
    std::string renderorder;
    dword width;
    dword height;
    dword tilewidth;
    bool infinite;
    dword nextlayerid;
    dword nextobjectid;
    dword n_layers = 0;
    std::vector<LayerData> layers;
};

struct LevelData
{
    TileData              m_tiles;
    std::vector<MapData>  maps;
};

class Level
{
private:
    OficinaFramework::RenderingSystem::Texture     *t_chunks;
    OficinaFramework::RenderingSystem::SpriteSheet *m_sheet;
    
    LevelData   m_data;
    std::string m_path;
    dword m_zone;

    void load_tile_data();
    void load_map_data(dword id);
    void draw_collision(vec2 position, CollisionArray *collision);
public:
    Level(dword id, dword zone);
    void LoadContent();
    void UnloadContent();
    void Draw();

    std::vector<CollisionArray*> getSurroundingCollision(vec2 playerPos, std::string layer = "fg");
};
