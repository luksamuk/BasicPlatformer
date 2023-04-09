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

Level::Level(dword id, dword zone)
{
    this->m_path = gen_level_folder(id);
    this->m_zone = zone;
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
    load_map_data(m_zone);
}

void Level::UnloadContent()
{
    delete m_sheet;
    m_sheet = nullptr;
    OficinaFramework::RenderingSystem::TexturePool::DisposeTexture(t_chunks);
    t_chunks = nullptr;

    for(size_t i = 0; i < this->m_data.m_tiles.collision.size(); i++) {
        if(this->m_data.m_tiles.collision[i] != nullptr) {
            for(auto collision : *this->m_data.m_tiles.collision[i]) {
                delete collision;
            }
            delete this->m_data.m_tiles.collision[i];
            this->m_data.m_tiles.collision[i] = nullptr;
        }
    }

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
    
    rapidxml::xml_document<> *doc = new rapidxml::xml_document<>;
    doc->parse<0>(file_contents);
    TileData data;

    // Read tileset tag
    auto tileset_node = doc->first_node("tileset");

    auto attr = tileset_node->first_attribute("version");
    data.version = attr->value();

    attr = tileset_node->first_attribute("tiledversion");
    data.tiledversion = attr->value();

    attr = tileset_node->first_attribute("name");
    data.name = attr->value();

    attr = tileset_node->first_attribute("tilewidth");
    data.tilewidth = std::stoi(attr->value());
    
    attr = tileset_node->first_attribute("tileheight");
    data.tileheight = std::stoi(attr->value());
    
    attr = tileset_node->first_attribute("tilecount");
    data.tilecount = std::stoi(attr->value());
    
    attr = tileset_node->first_attribute("columns");
    data.columns = std::stoi(attr->value());

    // Initialize collision for all tiles
    data.collision.reserve(data.tilecount);
    for(dword i = 0; i < data.tilecount; i++) {
        data.collision.push_back(nullptr);
    }

    // Iterate over children of tileset with "tile" tag.
    for(auto tile = tileset_node->first_node(); tile != nullptr; tile = tile->next_sibling()) {
        if(!strcmp(tile->name(), "tile")) {
            // tile has an id attribute. We'll use it for indexing.
            attr = tile->first_attribute("id");
            // REMEMBER TO CONSIDER THAT TILE 0 IS ACTUALLY TILE 1 ON THE MAP, ETC!
            dword tile_id = std::atoi(attr->value());
            data.collision[tile_id] = new CollisionArray;
            
            // These tags contain an "objectgroup" child with a list of objects.
            auto group = tile->first_node("objectgroup");
            if(group == nullptr) {
                continue;
            }
            
            for(auto obj = group->first_node(); obj != nullptr; obj = obj->next_sibling()) {
                // An object has an id, x and y positions, and may have a width and a height.
                vec2 position;
                vec2 size;
                
                attr = obj->first_attribute("x");
                position.x = std::stof(attr->value());

                attr = obj->first_attribute("y");
                position.y = std::stof(attr->value());

                attr = obj->first_attribute("width");
                if(attr != nullptr) {
                    size.x = std::stof(attr->value());
                }

                attr = obj->first_attribute("height");
                if(attr != nullptr) {
                    size.y = std::stof(attr->value());
                }
            
                // Each object may or may not contain a child.
                // - if it has no children, it is an AABB
                auto child = obj->first_node();
                if(child == nullptr) {
                    std::cout << "tile id " << tile_id << " has an AABB" << std::endl;
                    data.collision[tile_id]->push_back(new AABB(position, size));
                } else if(!strcmp(child->name(), "ellipse")) {
                    std::cout << "tile id " << tile_id << " has an ellipse" << std::endl;
                    // - if it has a child named ellipse, it is an ellipse (refer to object params)
                    data.collision[tile_id]->push_back(new Ellipse(position, size));
                } else if(!strcmp(child->name(), "point")) {
                    std::cout << "tile id " << tile_id << " has a point" << std::endl;
                    // - if it has a child named point, it is a point (refer to object params)
                    data.collision[tile_id]->push_back(new Point(position));
                } else if(!strcmp(child->name(), "polygon")) {
                    std::cout << "tile id " << tile_id << " has a polygon" << std::endl;
                    // - if it has a child named polygon, it is a polygon delimited by the given
                    //   points in its points attribute, which should be parsed
                    std::vector<vec2> acc_points;
                    attr = child->first_attribute("points");
                    std::string points = attr->value();
                    
                    std::istringstream pairs(points);
                    while(!pairs.eof()) {
                        std::string point;
                        pairs >> point;
                        vec2 p;

                        std::string number;
                        std::istringstream ss(point);
                        std::getline(ss, number, ',');
                        p.x = std::stof(number);
                        std::getline(ss, number);
                        p.y = std::stof(number);

                        p += position;
                        std::cout << p.toString() << std::endl;
                        acc_points.push_back(p);
                    }

                    auto polygon = new Polygon(acc_points);
                    std::cout << "  Triangles:" << std::endl;
                    for(auto tr : polygon->getTriangles()) {
                        std::cout << "     [" << tr.getA() << ", " << tr.getB()
                                  << ", " << tr.getC() << "]" << std::endl;
                    }
                    data.collision[tile_id]->push_back(polygon);
                }
            }
        }
    }

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
                        // if(tile == 0) {
                        //     std::cout << "__";
                        // } else {
                        //     std::cout << std::setw(2) << std::setfill('0')
                        //               << std::hex << tile << std::dec
                        //               << ' ';
                        // }
                    }
                }
                //std::cout << std::endl;
            }
            
            data.layers.push_back(layer);
        }
    }

    this->m_data.maps.push_back(data);
    delete doc;
}

void Level::Draw()
{
    // Draw art
    for(auto map : m_data.maps) {
        for(auto layer : map.layers) {
            for(dword y_pos = 0; y_pos < layer.height; y_pos++) {
                for(dword x_pos = 0; x_pos < layer.width - 1; x_pos++) {
                    dword_s tile = layer.data[(y_pos * layer.width) + x_pos];
                    if(tile >= 0) {
                        vec2 pos;
                        // Calculate position
                        pos.x = m_data.m_tiles.tilewidth * x_pos;
                        pos.y = m_data.m_tiles.tileheight * y_pos;

                        pos.x += m_data.m_tiles.tilewidth / 2.0;
                        pos.y += m_data.m_tiles.tileheight / 2.0;

                        // If out of the camera: don't even finish drawing.
                        auto vwp_pos = OficinaFramework::RenderingSystem::GetViewportPosition();
                        auto vwp_sz = OficinaFramework::RenderingSystem::GetViewportSize();
                        if((pos.x < vwp_pos.x - 128.0) ||
                           (pos.x > vwp_pos.x + vwp_sz.x) ||
                           (pos.y < vwp_pos.y - 128.0) ||
                           (pos.y > vwp_pos.y + vwp_sz.y)) {
                            continue;
                        }

                        m_sheet->DrawFrame(pos, (dword)tile);
                    }
                }
            }
        }
    }

    // Draw collision
    for(auto map : m_data.maps) {
        for(auto layer : map.layers) {
            for(dword y_pos = 0; y_pos < layer.height; y_pos++) {
                for(dword x_pos = 0; x_pos < layer.width - 1; x_pos++) {
                    dword_s tile = layer.data[(y_pos * layer.width) + x_pos];
                    if(tile >= 0) {
                        vec2 pos;
                        // Calculate position
                        pos.x = m_data.m_tiles.tilewidth * x_pos;
                        pos.y = m_data.m_tiles.tileheight * y_pos;

                        pos.x += m_data.m_tiles.tilewidth / 2.0;
                        pos.y += m_data.m_tiles.tileheight / 2.0;

                        // If out of the camera: don't even finish drawing.
                        auto vwp_pos = OficinaFramework::RenderingSystem::GetViewportPosition();
                        auto vwp_sz = OficinaFramework::RenderingSystem::GetViewportSize();
                        if((pos.x < vwp_pos.x - 128.0) ||
                           (pos.x > vwp_pos.x + vwp_sz.x) ||
                           (pos.y < vwp_pos.y - 128.0) ||
                           (pos.y > vwp_pos.y + vwp_sz.y)) {
                            continue;
                        }

                        // Draw tile collision
                        auto collision = m_data.m_tiles.collision[tile];
                        if(collision != nullptr) {
                            //std::cout << "drawing collision for tile " << tile << std::endl;
                            this->draw_collision(pos, collision);
                        }
                    }
                }
            }
        }
    }
}

const ColorDef DebugShapeColor = ColorDef::ORANGERED;

void
Level::draw_collision(vec2 pos, CollisionArray *collision)
{
    for(CollisionShape *shape : *collision) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        if(AABB *aabb = dynamic_cast<AABB*>(shape)) {
            // draw box
            glPushMatrix();
            OficinaFramework::RenderingSystem::DrawRectangle(
                pos - vec2(64.0, 64.0),
                vec2(128, 128),
                0.0,
                DebugShapeColor,
                0.5);
            glPopMatrix();
        } else if(Ellipse *e = dynamic_cast<Ellipse*>(shape)) {
            // draw ellipse
            //std::cout << "draw ellipse" << std::endl;
        } else if(Polygon *p = dynamic_cast<Polygon*>(shape)) {
            for(auto tr : p->getTriangles()) {
                // draw triangles
                glPushMatrix();
                vec2 correct = vec2(64, 64);
                vec2 vertices[] = {tr.getA(), tr.getB(), tr.getC()};
                OficinaFramework::RenderingSystem::DrawTriangle(
                    pos - correct,
                    vertices,
                    0.0,
                    DebugShapeColor,
                    0.5);
                glPopMatrix();
            }
        } else if(Point *p = dynamic_cast<Point*>(shape)) {
            //std::cout << "point????" << std::endl;
        } else if(Line *l = dynamic_cast<Line*>(shape)) {
            //std::cout << "line????" << std::endl;
        } else {
            //std::cout << "unknown shape..." << std::endl;
        }
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

std::vector<CollisionArray*>
Level::getSurroundingCollision(vec2 playerPos, std::string layer_name)
{
    std::vector<CollisionArray*> collision;

    // Only consider first map...
    for(auto layer : this->m_data.maps[0].layers) {
        if(layer.name == layer_name) {
            // Calculate X and Y coordinate of current
            // tile where the player is
            dword_s x = std::trunc(
                playerPos.x / this->m_data.m_tiles.tilewidth);
            dword_s y = std::trunc(
                playerPos.y / this->m_data.m_tiles.tileheight);

            // Calculate coordinates of all surrounding tiles on map
            vec2sdw tcoords[] = {
                vec2sdw(x - 1, y - 1), // upper left
                vec2sdw(x, y - 1), // upper center
                vec2sdw(x + 1, y - 1), // upper right
                vec2sdw(x - 1, y), // center left
                vec2sdw(x, y), // center
                vec2sdw(x + 1, y), // center right
                vec2sdw(x - 1, y + 1), // bottom left
                vec2sdw(x, y + 1), // bottom center
                vec2sdw(x + 1, y + 1) // bottom right
            };

            auto fetch_collision = [&](vec2sdw tilepos) {
                // If the x and y coordinates are invalid, just
                // store a null pointer as there is no collision
                if(tilepos.x < 0 || tilepos.y < 0 ||
                   tilepos.x >= (dword_s)layer.width ||
                   tilepos.y >= (dword_s)layer.height) {
                    collision.push_back(nullptr);
                    return;
                }

                dword pos = (tilepos.y * layer.width) + tilepos.x;
                auto tile = layer.data[pos];
                
                if(tile < 0) {
                    collision.push_back(nullptr);
                    return;
                }
                
                auto collisionArr = this->m_data.m_tiles.collision[tile];
                collision.push_back(collisionArr);
            };

            // Fetch collision in order
            int i = 0;
            for(auto coord : tcoords) {
                fetch_collision(coord);
            }
            
            break;
        }
    }

    return collision;
}
