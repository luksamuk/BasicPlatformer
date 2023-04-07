#include "CollisionShape.hpp"

float
Triangle::area() const
{
    auto x1 = this->a.x;
    auto y1 = this->a.y;
    auto x2 = this->b.x;
    auto y2 = this->b.y;
    auto x3 = this->c.x;
    auto y3 = this->c.y;

    return ((x1 * (y2 - y3)) +
            (x2 * (y3 - y1)) +
            (x3 * (y1 - y2))) / 2.0;
}

bool
Triangle::containsPoint(const vec2& p) const
{
    // THEOREM: Given a triangle ABC (this), and given a point P,
    // the sum of the areas of the triangles PAB, PBC and PAC is
    // equal to the area of ABC, if and only if P is contained
    // within ABC.(b2 - b1) / (a2 - a1)
    auto pab = Triangle(p, this->a, this->b);
    auto pbc = Triangle(p, this->b, this->c);
    auto pac = Triangle(p, this->a, this->c);

    return (this->area() - (pab.area() + pbc.area() + pac.area())) < EPSILON;
}

bool
Triangle::containsLine(const Line& l) const
{
    return this->containsPoint(l.getStart()) &&
        this->containsPoint(l.getEnd());
}

vec2 Line::getStart() const { return this->start; }
vec2 Line::getEnd() const { return this->end; }

std::optional<vec2>
Line::intersectsLine(const Line& l) const
{
    auto p1 = this->getStart();
    auto p2 = this->getEnd();
    auto p3 = l.getStart();
    auto p4 = l.getEnd();
    
    // Calculate intervals containing each line
    auto i1 = vec2(std::min(p1.x, p2.x), std::max(p1.x, p2.x));
    auto i2 = vec2(std::min(p3.y, p4.y), std::max(p3.y, p4.y));
    auto ia = vec2(std::max(i1.x, i2.x), std::min(i1.y, i2.y));

    if(i1.y < i2.x) {
        // Interval does not exist
        return std::nullopt;
    }

    // Check whether segments are parallel
    auto a1 = (p1.y - p2.y) / (p1.x - p2.x);
    auto a2 = (p3.y - p4.y) / (p3.x - p4.x);
    auto b1 = p1.y - (a1 * p1.x);
    auto b2 = p3.y - (a2 * p3.x);

    if((a1 - a2) < EPSILON) {
        // Linear coefficient is equal, so they're parallel
        // or superposing
        return std::nullopt;
    }

    // calculate hypotetical intersecting point
    auto pa_x = (b2 - b1) / (a2 - a1);
    auto pa = vec2(pa_x, ((a1 * pa_x) + b1));

    // Check whether pa is within the interval ia
    if((pa.x < ia.x) || (pa.x > ia.y)) {
        return std::nullopt;
    }

    return pa;
}

std::optional<vec2>
Triangle::intersectsLine(const Line& l) const
{
    if (this->containsLine(l)) {
        return l.getStart();
    }
    
    auto ab = Line(this->a, this->b);
    auto bc = Line(this->b, this->c);
    auto ac = Line(this->a, this->c);

    auto intersection = l.intersectsLine(ab);
    if(intersection) return intersection;

    intersection = l.intersectsLine(bc);
    if(intersection) return intersection;

    intersection = l.intersectsLine(ac);
    return intersection;
}

std::vector<Triangle>
Polygon::generateTriangles(std::vector<vec2> points)
{
    std::vector<Triangle> triangles;
    if (points.size() < 3) {
        return triangles;
    }

    // Assume points are in order. Take the first.
    vec2 leftmost = points[0];
    std::vector<vec2> below;
    std::vector<vec2> above;

    // For every other point, divide it into above or below.
    bool first = true;
    for(auto point : points) {
        if(!first) {
            if((point.y - leftmost.y) > 0) {
                above.push_back(point);
            } else {
                below.push_back(point);
            }
        }
        first = false;
    }

    // Reorder above/below points.
    // Points below should be ordered by x position,
    // points above should be ordered by x but reversed
    
    // std::sort(below.begin(), below.end(),
    //           [](vec2 const& a, vec2 const& b) {
    //               return a.x < b.x;
    //           });
    // std::sort(above.begin(), above.end(),
    //           [](vec2 const& a, vec2 const& b) {
    //               return a.x > b.x;
    //           });
    // below.insert(below.end(), above.begin(), above.end());

    // Everything is already ordered, so insert above in
    // reverse order at the end
    below.insert(below.end(), above.rbegin(), above.rend());

    // Get reordered points (in below vector) two by two.
    // This guarantees that, given that leftmost is a pivot,
    // we can now build a triangle fan around it
    for(size_t i = 0; i < below.size() - 1; i++) {
        auto a = leftmost;
        auto b = below[i];
        auto c = below[i + 1];
        triangles.push_back(Triangle(a, b, c));
    }

    return triangles;
}

vec2 Triangle::getA() const { return a; }
vec2 Triangle::getB() const { return b; }
vec2 Triangle::getC() const { return c; }

Polygon::Polygon(std::vector<vec2> points)
{
    std::sort(points.begin(), points.end(),
              [](vec2 const& a, vec2 const& b) {
                  return a.x < b.x;
              });
    this->triangles = this->generateTriangles(points);
};

std::vector<Triangle>
Polygon::getTriangles() const
{
    return triangles;
}
