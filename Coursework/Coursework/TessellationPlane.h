// copied plane class from PlaneMesh and changed topology to work with tessellation
#pragma once
#include "DXF.h"
class TessellationPlane :
    public BaseMesh
{
public:
    TessellationPlane(ID3D11Device* device);
    ~TessellationPlane();

    void sendData(ID3D11DeviceContext* deviceContext, D3D_PRIMITIVE_TOPOLOGY top = D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);

private:
    void initBuffers(ID3D11Device* device);
};

