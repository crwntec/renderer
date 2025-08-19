#include "math.hpp"
#include <random>


namespace MATH {
    // bool PointOnRightSide(float2 a, float2 b, float2 p) {
    //     //Vec a->p
    //     float2 ap = p - a;
    //     //Vec a->b rotated 90°
    //     float2 abOrth = orth(b-a);
    //     //Dot product sign tells direction of vec ap in relation to ab. Positive is right side after rotation. 
    //     return dot(ap, abOrth) >= 0; 
    // }
    float SignedTriangleArea(float2 a, float2 b, float2 c) {
        float2 ac = c - a;
        float2 abOrth = orth(b-a);
        return dot(ac, abOrth) / 2;
    }
    bool PointInTriangle(float2 a, float2 b, float2 c, float2 p, float3* weights) {
        float areaABP = SignedTriangleArea(a, b, p);
        float areaBCP = SignedTriangleArea(b, c, p);
        float areaCAP = SignedTriangleArea(c, a, p);

        float invAreaSum = 1 / (areaABP + areaBCP + areaCAP);
        float weightA = areaBCP * invAreaSum;
        float weightB = areaCAP * invAreaSum;
        float weightC = areaABP * invAreaSum;
        if (weights) {
            *weights = float3{weightA, weightB, weightC};
        }
        
        return (areaABP >=0 && areaBCP >=0 && areaCAP >=0);
    }
    
}
float3 random_color() {
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    return {dist(rng), dist(rng), dist(rng)};
}
