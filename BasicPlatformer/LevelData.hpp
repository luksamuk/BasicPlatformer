#pragma once
#include <OficinaFramework/OficinaTypes.hpp>
#include <vector>
#include <string>

// LEVEL FOLDER HIERARCHY:
// -> LEVELX
//    -> tiles.bin
//       - Description: Level tile data.
//       - Max size:   2 MB (2097152 B)
//    -> chunks.bin
//       - Description: Level chunk data.
//       - Max size: 510 KB  (522240 B)
//    -> scene0.bin
//       - Description: Level layout data (Act 1 / Zone).
//       - Max size:  64 KB   (65539 B)
//    -> scene1.bin (Act 2)
//       - Description: Level layout data (Act 2).
//       - Max size:  64 KB   (65539 B)

// 32B
// Amount of tiles on file deduced
// by file size
typedef dword LVL_tile;

// 2048B / 2KB
// Amount of chunks on file deduced
// by file size
struct LVL_chunk
{
    word layer0[64];
    word layer1[64];
};

// Levels have three representations.
// DO NOT save any of those directly to disk.
// Needless to say, level Y size is automatically
// computed.

// Default
struct LVL_level
{
    byte* chunks;
    byte sizeX;
    size_t chunkAmount;
};

// Editor
struct LVL_level_ed
{
    std::vector<byte> chunks;
    byte sizeX;
};

// File
// Here's how a level is supposed to be saved
// to disk.
// 1. X SIZE
// 2. CHUNK AMOUNT
// 3. DATA (each chunk byte)



// Important operations and macros

// RETRIEVING HEIGHTS FROM TILES
// This is kind of hard to explain so bear with me.
// Be the tile a height mask, formed of four modes.
// Each mode has 8 bytes (64 bits), because each
// column of each mode can have a height ranging from
// 0 to F (15).
// Each mode is disposed on the tile data as such:
//      0x[RIGHT][LEFT][BOTTOM][TOP]
// So it is easy to infer that
enum
{
    LVL_TILEM_TOP = 0x00u,
    LVL_TILEM_BOT = 0x01u,
    LVL_TILEM_LEF = 0x02u,
    LVL_TILEM_RIG = 0x03u
};

// Also, the given sensor has a relative position to the
// tile. Tile collision visually grows from left to right
// but, on our masks, it grows FROM RIGHT TO LEFT.
// So basically we compensate by walking 64 bits (8 bytes) per mode,
// and 4 bits (1/2 byte) per position relative to the left side of the
// height mask.
#define LVL_TILE_HEIGHT(MODE, RELX, TILE) (((TILE >> (MODE * 64u)) >> (RELX * 4u)) & 0x0F)

// There's also an INVERSE situation: whenever you OPPOSITE sensor
// (top sensor from a not-rotated player perspective, for example)
// needs to check on a specific tile, you must provide with an inverse
// approach...
#define LVL_TILE_HEIGHT_INV(MODE, RELX, TILE) (((TILE >> (MODE * 64u)) >> ((16u - RELX) * 4u)) & 0x0F)


// Class for loading level data.
class LVL_LevelTool
{
private:
    // Tiles
    word          m_n_tiles;
    LVL_tile*     m_tiles;
    // Chunks
    byte          m_n_chunks;
    LVL_chunk*    m_chunks;
    // Level general
    LVL_level_ed  m_level;
public:
    LVL_LevelTool();
    LVL_LevelTool(byte level, byte scene = 0u);
    ~LVL_LevelTool();

    void loadCollision(byte level);
    void loadLayout(byte scene = 0u);

    // Linecasting
    // 
};