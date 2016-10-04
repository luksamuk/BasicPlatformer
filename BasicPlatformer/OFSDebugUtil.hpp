#pragma once
#include <OficinaFramework/OficinaTypes.hpp>
namespace OFSDebugUtil
{
    // Helpers
    static constexpr float PI         = M_PI;
    static constexpr float TAU        = M_PI * 2.0f;
    static constexpr float TWO_PI     = M_PI * 2.0f;
    static constexpr float HALF_PI    = M_PI / 2.0f;
    static constexpr float QUARTER_PI = M_PI / 4.0f;
    
    // Draw method
    enum DrawProperties
    {
        OPEN,
        CHORD,
        PIE,
        LEFT,
        CENTER,
        RIGHT,
        TOP,
        BOTTOM,
        BASELINE
    };

    // Color
    void noFill();
    void noStroke();

    void stroke(Color4);
    void stroke(float, float, float, float a = 0.0f);
    //void stroke(const float*);
    void strokeWeight(int);
    void fill(Color4);
    void fill(float, float, float, float a = 0.0f);
    //void fill(const float*);


    // Figure
    void line(vec2 a, vec2 b);
    void ellipse(vec2 position, vec2 diameters);
    void arc(vec2 position, vec2 size,
                float startAngle, float stopAngle,
                DrawProperties mode = OPEN);
    void rect(vec2 position, vec2 size);
    void rect(vec2 position, vec2 size, float cornerRadius);
    void rect(vec2 position, vec2 size,
                                        float crTL, float crTR,
                                        float crBL, float crBR);
    void triangle(vec2 a, vec2 b, vec2 c);
}