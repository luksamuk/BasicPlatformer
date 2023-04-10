#pragma once
#include <optional>
#include <algorithm>
#include <OficinaFramework/EntitySystem.hpp>

const float EPSILON = 0.001f;

// None of these were thought for performance.
// Let's hope i dont have to refactor this

class CollisionShape
{
public:
    virtual ~CollisionShape() {};

    //virtual std::optional<vec2> Intersects(vec2 Position, vec2 size) = 0;
    virtual std::optional<vec2> Linecast(vec2 position, vec2 direction, float size) = 0;
};


class AABB : public CollisionShape
{
private:
    vec2 position; // position relative to tile
    vec2 size;
public:
    AABB(vec2 position, vec2 size)
        : position(position), size(size) {};

    float Left();
    float Right();
    float Bottom();
    float Top();
    vec2 Middle();

    virtual std::optional<vec2> Linecast(vec2 position, vec2 direction, float size);
};

class Ellipse : public CollisionShape
{
private:
    vec2 position; // position relative to tile
    vec2 radii;
public:
    Ellipse(vec2 position, vec2 radii)
        : position(position), radii(radii) {};

    virtual std::optional<vec2> Linecast(vec2 position, vec2 direction, float size);
};

class Point : public CollisionShape
{
private:
    vec2 position; // position relative to tile
public:
    Point(vec2 position) : position(position) {};

    virtual std::optional<vec2> Linecast(vec2 position, vec2 direction, float size);
};

class Line : public CollisionShape
{
private:
    vec2 start;
    vec2 end;

    static std::optional<vec2>
    _intersectsVertical(vec2 vpstart, vec2 vpend, const Line& l);
public:
    Line(vec2 start, vec2 end);

    vec2 getStart() const;
    vec2 getEnd() const;

    std::optional<vec2> intersectsLine(const Line& l) const;

    virtual std::optional<vec2> Linecast(vec2 position, vec2 direction, float size);
};

class Triangle : public CollisionShape
{
private:
    vec2 a;
    vec2 b;
    vec2 c;
public:
    Triangle(vec2 a, vec2 b, vec2 c) : a(a), b(b), c(c) {}

    vec2 getA() const;
    vec2 getB() const;
    vec2 getC() const;

    float area() const;
    
    bool containsPoint(const vec2& p) const;
    bool containsLine(const Line& l) const;
    std::optional<vec2> intersectsLine(const Line& l) const;

    virtual std::optional<vec2> Linecast(vec2 position, vec2 direction, float size);
};

class Polygon : public CollisionShape
{
private:
    // This class is purely representative!
    // A polygon should always be divided into a set
    // of triangles.
    
    // all points represent a convex polygon
    // all points should be relative to tile
    std::vector<Triangle> triangles;

    std::vector<Triangle> generateTriangles(std::vector<vec2>);
public:
    Polygon(std::vector<vec2> points);

    std::vector<Triangle> getTriangles() const;

    virtual std::optional<vec2> Linecast(vec2 position, vec2 direction, float size);
};
