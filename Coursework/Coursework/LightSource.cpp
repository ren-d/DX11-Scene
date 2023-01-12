#include "LightSource.h"
int LightSource::getLightType() {
    switch (m_type)
    {
    case LightSource::LType::DIRECTIONAL:
        return 0;
        break;
    case LightSource::LType::POINT:
        return 1;
        break;
    case LightSource::LType::SPOTLIGHT:
        return 2;
        break;
    };
}

float* LightSource::getPositionFloatArray()
{
    m_position[0] = getPosition().x;
    m_position[1] = getPosition().y;
    m_position[2] = getPosition().z;

    return m_position;
}

float* LightSource::getDirectionFloatArray()
{
    m_direction[0] = getDirection().x;
    m_direction[1] = getDirection().y;
    m_direction[2] = getDirection().z;

    return m_direction;
}

float* LightSource::getDiffuseColourFloatArray()
{
    m_diffuse[0] = getDiffuseColour().x;
    m_diffuse[1] = getDiffuseColour().y;
    m_diffuse[2] = getDiffuseColour().z;
    m_diffuse[3] = getDiffuseColour().w;

    return m_diffuse;
}
float* LightSource::getSpecularColourFloatArray()
{
    m_specular[0] = getSpecularColour().x;
    m_specular[1] = getSpecularColour().y;
    m_specular[2] = getSpecularColour().z;
    m_specular[3] = getSpecularColour().w;

    return m_specular;
}

float* LightSource::getAmbientColourFloatArray()
{
    m_ambient[0] = getAmbientColour().x;
    m_ambient[1] = getAmbientColour().y;
    m_ambient[2] = getAmbientColour().z;
    m_ambient[3] = getAmbientColour().w;

    return m_ambient;
}

void LightSource::init()
{
    getPositionFloatArray();
    getDirectionFloatArray();
    getDiffuseColourFloatArray();
    getAmbientColourFloatArray();
    getSpecularColourFloatArray();
}
void LightSource::update()
{
    // updates actual lighting values to GUI input via float arrays
    setPosition(m_position[0], m_position[1], m_position[2]);
    setDirection(m_direction[0], m_direction[1], m_direction[2]);
    setDiffuseColour(m_diffuse[0], m_diffuse[1], m_diffuse[2], m_diffuse[3]);
    setAmbientColour(m_ambient[0], m_ambient[1], m_ambient[2], m_ambient[3]);
    setSpecularColour(m_specular[0], m_specular[1], m_specular[2], m_specular[3]);
}
