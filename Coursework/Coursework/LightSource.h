#pragma once
#include "DXF.h"
class LightSource :
    public Light
{
    
public:
    enum class LType
    {
        DIRECTIONAL = 0, POINT = 1, SPOTLIGHT = 2
    };


    void setSpecularPower(float specularPower) { m_specularPower = specularPower; }
    void setConstantFactor(float constantFactor) { m_constantFactor = constantFactor; }
    void setLinearFactor(float linearFactor) { m_linearFactor = linearFactor; }
    void setQuadraticFactor(float quadraticFactor) { m_quadraticFactor = quadraticFactor; }
    void setLightType(LType type) { m_type = type; }
    
    float getSpecularPower() { return m_specularPower; }
    float getConstantFactor() { return m_constantFactor; }
    float getLinearfactor() { return m_linearFactor; }
    float getQuadraticFactor() { return m_quadraticFactor; }
    int getLightType();
private:
    float
        m_specularPower,
        m_constantFactor,
        m_linearFactor,
        m_quadraticFactor;
    LType m_type;
};

