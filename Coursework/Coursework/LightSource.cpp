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