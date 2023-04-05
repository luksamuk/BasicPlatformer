#pragma once
#include <optional>
#include <algorithm>
#include <OficinaFramework/EntitySystem.hpp>

// None of these were thought for performance.
// Let's hope i dont have to refactor this

class CollisionShape :
	public OficinaFramework::EntitySystem::DrawableEntity
{
public:
    virtual ~CollisionShape() {};

    // Inherited via DrawableEntity
    virtual void Initialize() override {}
    virtual void Update() override {}
    virtual void LoadContent() override {}
    virtual void UnloadContent() override {}
    virtual void Draw() override {}

    //virtual std::optional<vec2> Intersects(vec2 Position, vec2 size) = 0;
    //virtual bool Linecast(vec2 position, vec2 direction, float size) = 0;
};

class AABB : public CollisionShape
{
private:
    vec2 position; // position relative to tile
    vec2 size;
public:
    AABB(vec2 position, vec2 size)
        : position(position), size(size) {};
};

class Ellipse : public CollisionShape
{
private:
    vec2 position;
    vec2 radii;
public:
    Ellipse(vec2 position, vec2 radii)
        : position(position), radii(radii) {};
};

class Point : public CollisionShape
{
private:
    vec2 position;
public:
    Point(vec2 position) : position(position) {};
};

class Polygon : public CollisionShape
{
private:
    vec2 position;
    std::vector<vec2> points; // should be convex...
public:
    Polygon(vec2 position, std::vector<vec2> points)
        : position(position), points(points)
        {
            std::sort(
                this->points.begin(),
                this->points.end(),
                [](vec2 const& a, vec2 const& b) {
                    return a.x < b.x;
                });
        };
};
