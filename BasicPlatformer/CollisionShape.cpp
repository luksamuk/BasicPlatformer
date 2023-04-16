#include "CollisionShape.hpp"
#include <limits>

vec2 AABB::getPosition() const
{
    return position;
}

vec2 AABB::getSize() const
{
    return size;
}


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

Line::Line(vec2 start, vec2 end) {
    if(end.x < start.x) {
        this->start = end;
        this->end = start;
    } else {
        this->start = start;
        this->end = end;
    }
}

vec2 Line::getStart() const { return this->start; }
vec2 Line::getEnd() const { return this->end; }

std::optional<vec2>
Line::_intersectsVertical(vec2 vpstart, vec2 vpend, const Line& l)
{
    // First we evaluate if X of both vpstart/vpend are within the line's
    // X range
    if((vpstart.x - l.getStart().x < 0.0) || (l.getEnd().x - vpstart.x < 0.0)) {
        return std::nullopt;
    }

    // Now we take X of vpstart/vpend and calculate Y on this X coordinate
    auto m = (l.getEnd().y - l.getStart().y) / (l.getEnd().x - l.getStart().x);
    auto y = (m * (vpstart.x - l.getStart().x)) - l.getStart().y;

    // If the calculated Y is within the Y range of [vpstart.y, vpend.y], then
    // the line segments intersect at that point
    if((y > vpstart.y) && (y < vpend.y)) {
        return vec2(vpstart.x, y);
    }

    return std::nullopt;
}

std::optional<vec2>
Line::intersectsLine(const Line& l) const
{
    auto p1 = this->getStart();
    auto p2 = this->getEnd();
    auto p3 = l.getStart();
    auto p4 = l.getEnd();

    // Check if any of them is a vertical line, since it could not
    // be represented by a function. If so, we need to handle special cases
    auto this_vertical = (p1.x - p2.x) < EPSILON;
    auto l_vertical = (p3.x - p4.x) < EPSILON;
    
    if(this_vertical && l_vertical) {
        // Parallel vertical lines
        return std::nullopt;
    }

    // If there is only one vertical line, though, we need to use
    // a projection of the lines on the Y axis to determine if and
    // where they intersect
    if(this_vertical) {
        return Line::_intersectsVertical(p1, p2, l);
    } else {
        return Line::_intersectsVertical(p3, p4, *this);
    }
    
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
    std::vector<vec2> ordered_points;
    ordered_points.push_back(leftmost);
    ordered_points.insert(ordered_points.end(), below.begin(), below.end());
    ordered_points.insert(ordered_points.end(), above.rbegin(), above.rend());

    // Get reordered points (in below vector) three by three, jumping one by one.
    // This guarantees that, given that leftmost is a pivot,
    // we can now build a triangle fan around it
    for(size_t i = 0; i < ordered_points.size() - 2; i += 2) {
        auto a = ordered_points[i];
        auto b = ordered_points[i + 1];
        auto c = ordered_points[i + 2];
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


/* Collision interface */

float AABB::Left()
{
    return position.x;
}

float AABB::Right()
{
    return position.x + size.x;
}

float AABB::Bottom()
{
    return position.y + size.y;
}

float AABB::Top()
{
    return position.y - size.y;
}

vec2 AABB::Middle()
{
    return vec2(position.x + (size.x / 2.0f),
		position.y + (size.y / 2.0f));
}

std::optional<vec2>
AABB::Linecast(vec2 position, vec2 direction, float size)
{
    vec2 end(position + (direction * size));
    
    float lineLeft = fmin(position.x, end.x),
        lineRight = fmax(position.x, end.x),
        lineTop = fmin(position.y, end.y),
        lineBottom = fmax(position.y, end.y);

    // In case of no intersection between the AABBs, then
    // return false
    if (lineRight < Left() || lineLeft > Right() ||
        lineTop > Bottom() || lineBottom < this->position.y)
        return std::nullopt;

    vec2 nearestPoint = position;
    clamp(nearestPoint.x, Left(), Right());
    clamp(nearestPoint.y, Top(), Bottom());

    // vec2 nearestPointDirection = nearestPoint - position;
    // nearestPointDirection.x /= abs(nearestPointDirection.x);
    // nearestPointDirection.y /= abs(nearestPointDirection.y);

    // if (nearestPointDirection != direction)
    //     return std::nullopt;

    if(vec2::squareDistance(position, end) >= vec2::squareDistance(position, nearestPoint)) {
        return nearestPoint;
    }

    return std::nullopt;
}

std::optional<vec2>
Ellipse::Linecast(vec2 position, vec2 direction, float size)
{
    throw std::logic_error("Ellipse linecast not implemented");
}

std::optional<vec2>
Point::Linecast(vec2 position, vec2 direction, float size)
{
    throw std::logic_error("Point linecast not implemented");
}

std::optional<vec2>
Line::Linecast(vec2 position, vec2 direction, float size)
{
    // Linecast line into line by checking where said line
    // intercepts the current one
    vec2 b = position + (direction * size);
    return this->intersectsLine(Line(position, b));
}

std::optional<vec2>
Triangle::Linecast(vec2 position, vec2 direction, float size)
{
    // Linecasting into a triangle is basically retrieving
    // the point where the given line intercepts the triangle.
    // vec2 b = position + (direction * size);
    // return this->intersectsLine(Line(position, b));
    return std::nullopt;
}

std::optional<vec2>
Polygon::Linecast(vec2 position, vec2 direction, float size)
{
    // Iterate over every triangle.
    // Take the point closest to position.
    std::optional<vec2> intersection = std::nullopt;
    float sqDist = std::numeric_limits<float>::infinity();
    
    for(auto triangle : triangles) {
        // Linecast and get collision point if any.
        auto current = triangle.Linecast(position, direction, size);
        float currSqDist = std::numeric_limits<float>::infinity();
        if(current) {
            currSqDist = vec2::squareDistance(position, current.value());
        }

        // If this is the first detected collision or is the closest to
        // position so far, then select it for return
        if((!intersection) || (currSqDist < sqDist)) {
            intersection = current;
            sqDist = currSqDist;
        }
    }
    
    return intersection;
}
