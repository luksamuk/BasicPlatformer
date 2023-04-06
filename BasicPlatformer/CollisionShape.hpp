#pragma once
#include <optional>
#include <algorithm>
#include <OficinaFramework/EntitySystem.hpp>

const float EPSILON = 0.001f;

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
    vec2 position; // position relative to tile
    vec2 radii;
public:
    Ellipse(vec2 position, vec2 radii)
        : position(position), radii(radii) {};
};

class Point : public CollisionShape
{
private:
    vec2 position; // position relative to tile
public:
    Point(vec2 position) : position(position) {};
};

class Polygon : public CollisionShape
{
private:
    // This class is purely representative!
    // A polygon should always be divided into a set
    // of triangles.
    
    // all points represent a convex polygon
    // all points should be relative to tile
    std::vector<vec2> points;
public:
    Polygon(std::vector<vec2> points)
        : points(points)
        {
            std::sort(
                this->points.begin(),
                this->points.end(),
                [](vec2 const& a, vec2 const& b) {
                    return a.x < b.x;
                });
        };
};

class Line : public CollisionShape
{
private:
    vec2 start;
    vec2 end;
public:
    Line(vec2 start, vec2 end) : start(start), end(end) {}

    vec2 getStart() const;
    vec2 getEnd() const;

    std::optional<vec2> intersectsLine(const Line& l) const;
};

class Triangle : public CollisionShape
{
private:
    vec2 a;
    vec2 b;
    vec2 c;
public:
    Triangle(vec2 a, vec2 b, vec2 c) : a(a), b(b), c(c) {}

    float area() const;
    bool containsPoint(const vec2& p) const;
    bool containsLine(const Line& l) const;
    std::optional<vec2> intersectsLine(const Line& l) const;
};
