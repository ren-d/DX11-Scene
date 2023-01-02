#pragma once
#include "DXF.h"
class LightSource :
    public Light
{
    
public:
    enum class LType
    {
        DIRECTIONAL, POINT, SPOTLIGHT
    };

    void setSpecularPower(float specularPower) { m_specularPower = specularPower; }
    void setLightType(LType type) { m_type = type; }

    float getSpecularPower() { return m_specularPower; }
    LType getLightType() { return m_type; }
private:
    float m_specularPower;
    LType m_type;
};

