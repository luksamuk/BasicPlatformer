#include "LevelData.hpp"
#include <sstream>
#include <cstdio>

LVL_LevelTool::LVL_LevelTool()
{
    m_n_tiles  = 0x0000u;
    m_n_chunks =   0x00u;
    m_tiles    = nullptr;
    m_chunks   = nullptr;
}

LVL_LevelTool::LVL_LevelTool(byte level, byte scene)
{

}

LVL_LevelTool::~LVL_LevelTool()
{

}

void LVL_LevelTool::loadCollision(byte level)
{
    std::ostringstream oss;
    oss.clear();
    oss << "levels/level" << (level < 10u ? "0" : "") << level
        << "/";
    std::string tilesPath  = oss.str() + "tiles.bin";
    std::string chunksPath = oss.str() + "chunks.bin";
    // Load tile data
    FILE* f;
    f = fopen(tilesPath.c_str(), "rb");
    
    // Load chunk data
}

void LVL_LevelTool::loadLayout(byte scene)
{

}