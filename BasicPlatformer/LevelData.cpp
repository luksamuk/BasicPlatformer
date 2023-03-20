#include "LevelData.hpp"
#include <sstream>
#include <cstdio>
#include <sstream>
#include <iomanip>
#include <rapidxml/rapidxml.hpp>
#include <OficinaFramework/IOSystem.hpp>
#include <OficinaFramework/OficinaTypes.hpp>

std::string
gen_level_folder(dword id)
{
    std::ostringstream oss;
    oss.clear();
    oss << "levels/r" << id;
    return oss.str();
}

std::string
gen_map_file(std::string path, dword id)
{
    std::ostringstream oss;
    oss.clear();
    oss << path << "/zone" << id;
    return oss.str();
}

Level::Level(dword id)
{
    this->m_path = gen_level_folder(id);
}

void Level::LoadContent()
{
    // Load texture
    this->t_chunks = OficinaFramework::RenderingSystem::TexturePool::LoadTexture(this->m_path + "/chunks");
    
    // Load .tsx file
    load_tile_data();

    // Load sprite sheet based on .tsx data
    this->m_sheet = new OficinaFramework::RenderingSystem::SpriteSheet(
        vec2dw(this->m_data.m_tiles.tilewidth, this->m_data.m_tiles.tileheight),
        vec2b::Zero(),
        vec2::Zero());
    this->m_sheet->AppendTexture(t_chunks);

    // Check for .tmx files (zone 0, zone 1, zone 2)
    load_map_data(0);
}

void Level::UnloadContent()
{
    delete m_sheet;
    m_sheet = nullptr;
    OficinaFramework::RenderingSystem::TexturePool::DisposeTexture(t_chunks);
    t_chunks = nullptr;

    // if(m_data.m_maps != nullptr) {
    //     for(dword i = 0; i < m_data.n_maps; i++) {
    //         for(dword j = 0; j < m_data.m_maps[i].n_layers; j++) {
    //             if(m_data.m_maps[i].layers[j].data != nullptr) {
    //                 delete [] m_data.m_maps[i].layers[j].data;
    //                 m_data.m_maps[i].layers[j].data = nullptr;
    //             }
    //         }
    //         if(m_data.m_maps[i].layers != nullptr) {
    //             delete [] m_data.m_maps[i].layers;
    //             m_data.m_maps[i].layers = nullptr;
    //         }
    //     }
    //     delete [] m_data.m_maps;
    //     m_data.m_maps = nullptr;
    // }
}

void Level::load_tile_data()
{
    // .TSX file:
    // => tag: tileset
    //    - version
    //    - tiledversion
    //    - name (tileset name)
    //    - tilewidth
    //    - tileheight
    //    - tilecount
    //    - columns
    // => tag: image
    //    - source (filename)
    //    - trans (transparent color)
    //    - width (image width)
    //    - height (image height)

    // Load .tsx file
    qword_s file_size;
    char *file_contents = (char*)OficinaFramework::IOSystem::Load(this->m_path + "/tiles", file_size);
    file_contents[file_size - 1] = '\0';
    
    rapidxml::xml_document<> *doc = new rapidxml::xml_document<>;
    doc->parse<0>(file_contents);
    TileData data;

    // Read tileset tag
    auto tileset_node = doc->first_node("tileset");

    auto attr = tileset_node->first_attribute("version");
    data.version = attr->value();
    OF_Log(OF_LOG_LVL_INFO, data.version.c_str());
    OF_Log(OF_LOG_LVL_INFO, "\n");

    attr = tileset_node->first_attribute("tiledversion");
    data.tiledversion = attr->value();
    OF_Log(OF_LOG_LVL_INFO, data.tiledversion.c_str());
    OF_Log(OF_LOG_LVL_INFO, "\n");

    attr = tileset_node->first_attribute("name");
    data.name = attr->value();
    OF_Log(OF_LOG_LVL_INFO, data.name.c_str());
    OF_Log(OF_LOG_LVL_INFO, "\n");

    attr = tileset_node->first_attribute("tilewidth");
    data.tilewidth = std::stoi(attr->value());
    OF_Log(OF_LOG_LVL_INFO, attr->value());
    OF_Log(OF_LOG_LVL_INFO, "\n");
    
    attr = tileset_node->first_attribute("tileheight");
    data.tileheight = std::stoi(attr->value());
    OF_Log(OF_LOG_LVL_INFO, attr->value());
    OF_Log(OF_LOG_LVL_INFO, "\n");
    
    attr = tileset_node->first_attribute("tilecount");
    data.tilecount = std::stoi(attr->value());
    OF_Log(OF_LOG_LVL_INFO, attr->value());
    OF_Log(OF_LOG_LVL_INFO, "\n");
    
    attr = tileset_node->first_attribute("columns");
    data.columns = std::stoi(attr->value());
    OF_Log(OF_LOG_LVL_INFO, attr->value());
    OF_Log(OF_LOG_LVL_INFO, "\n");

    this->m_data.m_tiles = data;

    delete doc;
    //delete [] file;
}

void Level::load_map_data(dword id)
{
    // Load .tmx file
    auto map_filename = gen_map_file(this->m_path, id);
    qword_s file_size;
    char *file_contents = (char*)OficinaFramework::IOSystem::Load(map_filename, file_size);
    file_contents[file_size - 1] = '\0';

    rapidxml::xml_document<> *doc = new rapidxml::xml_document<>;
    doc->parse<0>(file_contents);
    MapData data;

    // .TMX file:
    // => tag: map
    //    - version
    //    - tiledversion
    //    - orientation
    //    - renderorder
    //    - width
    //    - height
    //    - tilewidth
    //    - infinite (int)
    //    - nextlayerid (?)
    //    - nextobjectid (?)
    auto map = doc->first_node("map");
    
    auto attr = map->first_attribute("version");
    data.version = attr->value();

    attr = map->first_attribute("tiledversion");
    data.tiledversion = attr->value();

    attr = map->first_attribute("orientation");
    data.orientation = attr->value();

    attr = map->first_attribute("renderorder");
    data.renderorder = attr->value();

    attr = map->first_attribute("width");
    data.width = std::stoi(attr->value());
    
    attr = map->first_attribute("height");
    data.height = std::stoi(attr->value());

    attr = map->first_attribute("tilewidth");
    data.tilewidth = std::stoi(attr->value());

    attr = map->first_attribute("infinite");
    data.infinite = (bool)std::stoi(attr->value());

    // The following are now children!
    // => child: tileset (we're going to ignore this one)
    //    - firstgid (?)
    //    - source
    
    for(auto child = map->first_node(); child != nullptr; child = child->next_sibling()) {
        if(!strcmp(child->name(), "layer")) {
            LayerData layer;
            // => tag(s): layer
            //    - id
            //    - name
            //    - width
            //    - height
            //    => child: data
            //       - encoding: csv
            //       => contents: multiline CSV, respects columns
            attr = child->first_attribute("id");
            layer.id = std::stoi(attr->value());

            attr = child->first_attribute("name");
            layer.name = attr->value();

            attr = child->first_attribute("width");
            layer.width = std::stoi(attr->value());

            attr = child->first_attribute("height");
            layer.height = std::stoi(attr->value());

            layer.data.reserve(layer.width * layer.height);
            auto csv_child = child->first_node("data");
            
            std::istringstream csv(csv_child->value());
            std::string line;
            while(std::getline(csv, line)) {
                char comma_eater;
                dword_s tile;
                std::istringstream ss(line);
                while(!ss.eof()) {
                    ss >> tile;
                    if(!ss.eof()) {
                        ss >> comma_eater;
                        layer.data.push_back(tile - 1);
                        if(tile == 0) {
                            std::cout << "__";
                        } else {
                            std::cout << std::setw(2) << std::setfill('0') << std::hex << tile << std::dec << ' ';
                        }
                    }
                }
                std::cout << std::endl;
            }
            
            data.layers.push_back(layer);
        }
    }

    this->m_data.maps.push_back(data);
    delete doc;
}

void Level::Draw()
{
    for(auto map : m_data.maps) {
        for(auto layer : map.layers) {
            for(dword y_pos = 0; y_pos < layer.height; y_pos++) {
                for(dword x_pos = 0; x_pos < layer.width; x_pos++) {
                    dword_s tile = layer.data[(y_pos * layer.width) + x_pos];
                    if(tile >= 0) {
                        vec2 pos;
                        // Calculate position
                        pos.x = m_data.m_tiles.tilewidth * x_pos;
                        pos.y = m_data.m_tiles.tileheight * y_pos;

                        pos.x += m_data.m_tiles.tilewidth / 2.0;
                        pos.y += m_data.m_tiles.tileheight / 2.0;

                        m_sheet->DrawFrame(pos, (dword)tile);
                    }
                }
            }
        }
    }
}

