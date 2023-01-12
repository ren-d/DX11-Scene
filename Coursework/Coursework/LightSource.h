// Basic Light Source Class
// wrapper class used for easier lighting application

#pragma once
#include "DXF.h"
class LightSource :
    public Light
{
    
public:
    // used for checking light types
    enum class LType
    {
        DIRECTIONAL = 0, POINT = 1, SPOTLIGHT = 2
    };

    void update();
    void init();

    // getters
    void setSpecularPower(float specularPower) { m_specularPower = specularPower; }
    void setConstantFactor(float constantFactor) { m_constantFactor = constantFactor; }
    void setLinearFactor(float linearFactor) { m_linearFactor = linearFactor; }
    void setQuadraticFactor(float quadraticFactor) { m_quadraticFactor = quadraticFactor; }
    void setInnerSpotlightConeInDegrees(float innerCone) { m_innerCone = innerCone; }
    void setOuterSpotlightConeInDegrees(float outerCone) { m_outerCone = outerCone; }
    void setLightType(LType type) { m_type = type; }
    
    // setters
    float* getSpecularPower() { return &m_specularPower; }
    float* getConstantFactor() { return &m_constantFactor; }
    float* getLinearfactor() { return &m_linearFactor; }
    float* getQuadraticFactor() { return &m_quadraticFactor; }
    float* getInnerCone() { return &m_innerCone; }
    float* getOuterCone() { return &m_outerCone; }
    int getLightType();

    // create and get float arrays for GUI implemention
    float* getPositionFloatArray();
    float* getDirectionFloatArray();
    float* getDiffuseColourFloatArray();
    float* getSpecularColourFloatArray();
    float* getAmbientColourFloatArray();


private:
    float
        m_specularPower,
        m_constantFactor,
        m_linearFactor,
        m_quadraticFactor,
        m_innerCone,
        m_outerCone;
    LType m_type;

    float m_direction[3];
    float m_position[3];
    float m_diffuse[4];
    float m_specular[4];
    float m_ambient[4];
};

