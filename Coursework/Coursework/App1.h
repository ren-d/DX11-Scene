// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"
#include "SceneObject.h"

#include "Water.h"
#include "WaterShader.h"
#include "DepthShader.h"
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
	void depthpass();
private:
	static constexpr int MAX_LIGHTS = 4;

	SceneObject* waterMesh;
	ModelObject* boat;
	AModel* boatModel;
	WaterShader* waterShader;
	ModelShader* modelShader;
	DepthShader* depthShader;

	Water* water;

	LightSource* lights[MAX_LIGHTS];
	float lightOneColour[MAX_LIGHTS];
	float lightTwoColour[MAX_LIGHTS];
	float lightThreeColour[MAX_LIGHTS];
	float lightFourColour[MAX_LIGHTS];
	float ambientColour[MAX_LIGHTS];

	ShadowMap* shadowMaps[4];
	bool displayShadowMaps;

	
	float deltaTime;

	// GUI variables
	float lightdir[3];
	float waveOneDir[2];
	float waveTwoDir[2];
	float waveThreeDir[2];
	float waveFourDir[2];
	
};

#endif