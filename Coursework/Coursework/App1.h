// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"
#include "SceneObject.h"
#include "BasicShader.h"
#include "Water.h"
#include "WaterShader.h"
#include "ModelObject.h"
#include "LightSource.h"
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
	void basepass();
private:
	SceneObject* waterMesh;
	ModelObject* house;
	AModel* houseModel;
	BasicShader* basicShader;
	WaterShader* waterShader;
	ModelShader* modelShader;
	SphereMesh* sphere;
	Water* water;
	LightSource* lights[4];

	float deltaTime;
	float lightdir[3];
	float waveOneDir[2];
	float waveTwoDir[2];
	float waveThreeDir[2];
	float waveFourDir[2];
};

#endif