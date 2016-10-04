#include "OFSDebugUtil.hpp"
#include <OficinaFramework/RenderingSystem.hpp>

namespace OFSDebugUtil
{
    static bool fillState     = true;
    static bool strokeState   = true;
    static Color4 fillColor   = Color4::MaskToColor4(WHITE);
    static Color4 strokeColor = Color4::MaskToColor4(WHITE);

    void noFill()   { fillState = false;   }
    void noStroke() { strokeState = false; }

    void stroke(Color4 color) {
        strokeColor = color;
        strokeState = true;
    }
    void stroke(float r, float g, float b, float a) {
        strokeColor = Color4(r, g, b, a);
        strokeState = true;
    }

    void strokeWeight(int val) {
        glLineWidth(val);
    }

    void fill(Color4 color) {
        fillColor = color;
        fillState = true;
    }
    void fill(float r, float g, float b, float a) {
        fillColor = Color4(r, g, b, a);
        fillState = true;
    }



    // Helpers
    void translate(float x, float y) { glTranslatef(x, y, 0.0f); }
    void translate(vec2 v) { translate(v.x, v.y); }

    void trueEllipse(float x, float y, float width, float height)
    {
        glPushMatrix();
            translate(x, y);
            glBegin(GL_POLYGON);
            for (int i = 0; i < 360; i++) {
                float rad = degtorad(i);
                glVertex2f(cos(rad) * (width / 2.0f), sin(rad) * (height / 2.0f));
            }
            glEnd();
        glPopMatrix();
    }

    void trueArcStroke(float x, float y, float width, float height, float start, float stop, DrawProperties mode) {
        if (stop < start) return;

        glPushMatrix();

            translate(x, y);
            int degInit = start;
            int degEnd = stop;

            glBegin((mode == OPEN ? GL_LINE_STRIP : GL_LINE_LOOP));
            for (int i = degInit; i < degEnd; i++)
            {
                float rad = degtorad(i);
                glVertex2f(cos(rad) * (width / 2.0f), sin(rad) * (height / 2.0f));
            }

            // In case of a pie, we actually need to add a
            // final vertex.
            if (mode == PIE) glVertex2f(0.0f, 0.0f);

            glEnd();

        glPopMatrix();
    }

    void trueArc(float x, float y, float width, float height, float start, float stop, DrawProperties mode) {
        if (stop < start) return;

        glPushMatrix();
        
            translate(x, y);
            int degInit = start;
            int degEnd = stop;
            switch (mode)
            {
            case OPEN:
            case CHORD: // Convex polygon
                glBegin(GL_POLYGON);
                for (int i = degInit; i < degEnd; i++)
                {
                    float rad = degtorad(i);
                    glVertex2f(cos(rad) * (width / 2.0f), sin(rad) * (height / 2.0f));
                }
                glVertex2f(0.0f, 0.0f);
                glEnd();
                break;
            case PIE: // Concave polygon
                glBegin(GL_TRIANGLE_FAN);
                glVertex2f(0.0f, 0.0f);
                for (int i = degInit; i < degEnd; i++)
                {
                    float rad = degtorad(i);
                    glVertex2f(cos(rad) * (width / 2.0f), sin(rad) * (height / 2.0f));
                }
                glEnd();
                break;
            default: break;
            }

        glPopMatrix();
    }

    void trueRect(float x, float y, float w, float h)
    {
        glPushMatrix();
            translate(x, y);
            glBegin(GL_QUADS);
                glVertex2f(0.0f, 0.0f);
                glVertex2f(w, 0.0f);
                glVertex2f(w, h);
                glVertex2f(0.0f, h);
            glEnd();
        glPopMatrix();
    }

    void trueTriangle(const float* a, const float* b, const float* c)
    {
        glPushMatrix();
            glBegin(GL_TRIANGLES);
                glVertex2fv(a);
                glVertex2fv(b);
                glVertex2fv(c);
            glEnd();
        glPopMatrix();
    }



    // Main functions
    void line(vec2 a, vec2 b)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        OficinaFramework::RenderingSystem::glColorM(strokeColor.GetMask(), strokeColor.a);
        glBegin(GL_LINES);
            glVertex2fv(a.v);
            glVertex2fv(b.v);
        glEnd();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    void ellipse(vec2 position, vec2 diameters)
    {
        if (fillState) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            OficinaFramework::RenderingSystem::glColorM(fillColor.GetMask(), fillColor.a);
            trueEllipse(position.x, position.y,
                        diameters.x, diameters.y);
        }
        if (strokeState) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            OficinaFramework::RenderingSystem::glColorM(strokeColor.GetMask(), strokeColor.a);
            trueEllipse(position.x, position.y,
                        diameters.x, diameters.y);
        }
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }


    

    void arc(vec2 position, vec2 size, float start, float stop, DrawProperties mode) {
        if ((stop < start) ||
            (mode != OPEN && mode != CHORD && mode != PIE)) return;
        if (fillState) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            OficinaFramework::RenderingSystem::glColorM(fillColor.GetMask(), fillColor.a);
            trueArc(position.x, position.y,
                    size.x, size.y, start, stop, mode);
        }
        if (strokeState) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            OficinaFramework::RenderingSystem::glColorM(strokeColor.GetMask(), strokeColor.a);
            trueArcStroke(position.x, position.y,
                          size.x, size.y, start, stop, mode);
        }
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    void rect(vec2 position, vec2 size)
    {
        if (fillState) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            OficinaFramework::RenderingSystem::glColorM(fillColor.GetMask(), fillColor.a);
            trueRect(position.x, position.y, size.x, size.y);
        }
        if (strokeState) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            OficinaFramework::RenderingSystem::glColorM(strokeColor.GetMask(), strokeColor.a);
            trueRect(position.x, position.y, size.x, size.y);
        }
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    void rect(vec2 pos, vec2 size, float r)
    {
        if (fillState) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            OficinaFramework::RenderingSystem::glColorM(fillColor.GetMask(), fillColor.a);

            // Internal rectangle
            trueRect(pos.x + r, pos.y + r, size.x - (2.0f * r), size.y - (2.0f * r));

            // Rectangles on borders
            trueRect(pos.x + r, pos.y + (r / 2.0f), size.x - (2.0f * r), r / 2.0f);
            trueRect(pos.x + r, pos.y + size.y - r, size.x - (2.0f * r), r / 2.0f);
            trueRect(pos.x + (r / 2.0f), pos.y + r, r / 2.0f, size.y - (2.0f * r));
            trueRect(pos.x + size.x - r, pos.y + r, r / 2.0f, size.y - (2.0f * r));
        }

        // Lines for the border rectangles
        if (strokeState) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            OficinaFramework::RenderingSystem::glColorM(strokeColor.GetMask(), strokeColor.a);
            line(vec2(pos.x + r, pos.y + (r / 2.0f)),
                 vec2(pos.x + size.x - r, pos.y + (r / 2.0f)));
            line(vec2(pos.x + r, pos.y - (r / 2.0f) + size.y),
                 vec2(pos.x + size.x - r, pos.y - (r / 2.0f) + size.y));
            line(vec2(pos.x + (r / 2.0f), pos.y + r),
                 vec2(pos.x + (r / 2.0f), pos.y + size.y - r));
            line(vec2(pos.x - (r / 2.0f) + size.x, pos.y + r),
                 vec2(pos.x - (r / 2.0f) + size.x, pos.y + size.y - r));
        }
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // Arcs
        arc(vec2(pos.x + r, pos.y + r), vec2(r), PI, 3 * HALF_PI, OPEN);
        arc(vec2(pos.x + (size.x - r), pos.y + r), vec2(r), 3 * HALF_PI, TAU, OPEN);
        arc(vec2(pos.x + r, pos.y + (size.y - r)), vec2(r), HALF_PI, PI, OPEN);
        arc(vec2(pos.x + (size.x - r), pos.y + (size.y - r)), vec2(r), 0, HALF_PI, OPEN);
    }

    void rect(vec2 pos, vec2 size, float tl, float tr, float bl, float br)
    {
        // Internal rectangles
        // we'll end up writing 5 rectangles in total, due to differences
        // on corner angles.
        if (fillState) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            OficinaFramework::RenderingSystem::glColorM(fillColor.GetMask(), fillColor.a);
            // First, we draw rectangles related to radius sizes.
            trueRect(pos.x, pos.y + (tl / 2.0f), fmax(tl, bl) / 2.0f, ceil(size.y - (bl / 2.0f) - (tl / 2.0f)) + 1.0f);
            trueRect(pos.x + (bl / 2.0f), pos.y + (size.y - (fmax(bl, br) / 2.0f)), ceil(size.x - (bl / 2.0f) - (br / 2.0f)) + 1.0f, fmax(bl, br) / 2.0f);
            trueRect(pos.x + (size.x - fmax(br, bl) / 2.0f), pos.y + (tr / 2.0f), fmax(br, bl) / 2.0f, ceil(size.y - (br / 2.0f) - (tr / 2.0f)) + 1.0f);
            trueRect(pos.x + (tl / 2.0f), pos.y, ceil(size.x - (tr / 2.0f) - (tl / 2.0f)) + 1.0f, fmax(tr, tl));

            // Now we draw our internal rectangle.
            trueRect(pos.x + fmax(tl, bl) / 2.0f, pos.y + fmax(tl, tr) / 2.0f,
                (size.x - fmax(tl, bl) / 2.0f - fmax(tr, br) / 2.0f),
                (size.y - fmax(tl, tr) / 2.0f - fmax(bl, br) / 2.0f));
        }

        // Lines around borders
        // We'll need to draw them for each of our rectangles,
        // except for the internal rectangle.
        if (strokeState) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            OficinaFramework::RenderingSystem::glColorM(strokeColor.GetMask(), strokeColor.a);
            line(vec2(pos.x, pos.y + tl / 2.0f),
                 vec2(pos.x, pos.y + size.y - bl / 2.0f));
            line(vec2(pos.x + bl / 2.0f, pos.y + size.y),
                 vec2(pos.x + size.x - br / 2.0f, pos.y + size.y));
            line(vec2(pos.x + size.x, pos.y + size.y - br / 2.0f),
                 vec2(pos.x + size.x, pos.y + tr / 2.0f));
            line(vec2(pos.x + tl / 2.0f, pos.y),
                 vec2(pos.x + size.x - tr / 2.0f, pos.y));

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        // Arcs
        arc(vec2(pos.x + tl / 2.0f, pos.y + tl / 2.0f), vec2(tl), PI, 3 * HALF_PI, OPEN);
        arc(vec2(pos.x + (size.x - tr / 2.0f), pos.y + tr / 2.0f), vec2(tr), 3 * HALF_PI, TAU, OPEN);
        arc(vec2(pos.x + bl / 2.0f, pos.y + (size.y - bl / 2.0f)), vec2(bl), HALF_PI, PI, OPEN);
        arc(vec2(pos.x + (size.x - br / 2.0f), pos.y + (size.y - br / 2.0f)), vec2(br), 0, HALF_PI, OPEN);
    }


    void triangle(vec2 a, vec2 b, vec2 c)
    {
        if (fillState) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            OficinaFramework::RenderingSystem::glColorM(fillColor.GetMask(), fillColor.a);
            trueTriangle(a.v, b.v, c.v);
        }
        if (strokeState) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            OficinaFramework::RenderingSystem::glColorM(strokeColor.GetMask(), strokeColor.a);
            trueTriangle(a.v, b.v, c.v);
        }
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }


}