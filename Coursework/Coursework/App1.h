// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"
#include "SceneObject.h"
#include "structsforuse.h"
#include "Water.h"
#include "WaterShader.h"
#include "DepthShader.h"
#include "ModelObject.h"
#include "LightSource.h"
#include "TextureShader.h"
#include "TessellationPlane.h"
#include "WaterDepthShader.h"
#include "ComputeDownSample.h"
#include "ComputeBrightness.h"
#include "HorizontalBlurShader.h"
#include "VerticalBlurShader.h"
#include "ComputeUpSample.h"
#include "ComputeBlend.h"
class App1 : public BaseApplication
{
public:
	
	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	void initShadowMaps();
	void initLighting();
	void initTextures();
	void initSceneObjects(int* sceneWidth, int* sceneHeight);
	void initShaders(HWND hwnd);
	void initGUI();
	bool frame();

	void updateInput();
protected:
	bool render();
	void gui();
	void basepass();
	void depthpass();
	void computepass();
	void brightnesspass();

	void upsample();
	void finalpass();
private:
	static constexpr int MAX_LIGHTS = 4;

	SceneObject* waterMesh;
	ModelObject* boat;
	AModel* boatModel;
	WaterShader* waterShader;
	WaterDepthShader* waterDepthShader;
	ModelShader* modelShader;
	DepthShader* depthShader;
	TextureShader* textureShader;
	
	OrthoMesh* orthoMesh, *orthoMesh2;
	Water* water;
	SceneObject* cube;
	SphereMesh* sphere;
	LightSource* lights[MAX_LIGHTS];
	ComputeDownSample* computeDownSample;
	ComputeBrightness* computeBrightness;
	ComputeUpSample* computeUpSample;
	ComputeBlend* computeBlend;
	HorizontalBlurShader* horizonalBlurShader;
	VerticalBlurShader* verticalBlurShader;
	RenderTexture* renderTexture, *renderTexture2;
	float lightOneColour[MAX_LIGHTS];
	float lightTwoColour[MAX_LIGHTS];
	float lightThreeColour[MAX_LIGHTS];
	float lightFourColour[MAX_LIGHTS];
	float ambientColour[MAX_LIGHTS];

	ShadowMap* shadowMaps[4];
	bool displayShadowMaps;

	
	float timeInSeconds;

	// GUI variables
	float lightdir[3];
	float waveOneDir[2];
	float waveTwoDir[2];
	float waveThreeDir[2];
	float waveFourDir[2];
	
};

#endif