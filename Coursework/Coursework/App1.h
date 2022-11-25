// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"
#include "SceneObject.h"
#include "BasicShader.h"
#include "HeightMappedObject.h"
#include "HeightMapShader.h"
#include "ModelObject.h"
class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	bool frame();

protected:
	bool render();
	void gui();

private:
	SceneObject* obj;
	ModelObject* house;
	AModel* houseModel;
	BasicShader* basicShader;
	HeightMapShader* heightShader;
	ModelShader* modelShader;
	SphereMesh* sphere;
	HeightMappedObject* heightMapObj;
	Light* sun;

	float lightdir[3];
};

#endif