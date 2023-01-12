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
#include "ColourShader.h"

// main application
class App1 : public BaseApplication
{
public:
	
	App1();
	~App1();

	// initialising
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

	// render passes
	void basepass();
	void depthpass();
	void computepass();
	void renderDepthScene(XMMATRIX lightViewMatrix, XMMATRIX lightProjectionMatrix, XMMATRIX worldMatrix);
	void finalpass();

private:


	// Models and Objects
	AModel* boatModel, * crateModel, * barrelModel, * woodenBoxModel, * kegModel;
	SphereMesh* sphere;
	Water* water;
	ModelObject* boat, *crate, *barrel, *woodenBox, * keg;
	

	// Shaders
	WaterShader* waterShader;
	WaterDepthShader* waterDepthShader;
	ModelShader* modelShader;
	DepthShader* depthShader;
	TextureShader* textureShader;
	ColourShader* colourShader;

	// Compute Shaders
	ComputeDownSample* computeDownSample[3];
	ComputeBrightness* computeBrightness;
	ComputeUpSample* computeUpSample[3];
	ComputeBlend* computeBlend;
	HorizontalBlurShader* horizonalBlurShader[4];
	VerticalBlurShader* verticalBlurShader[4];


	// Lighting
	static constexpr int MAX_LIGHTS = 4;
	static constexpr int MAX_DEPTH_MAPS_PER_LIGHT = 6;
	static constexpr int MAX_DEPTH_MAPS = 24;
	XMFLOAT3 directions[MAX_DEPTH_MAPS_PER_LIGHT];

	ShadowMap* shadowMaps[MAX_DEPTH_MAPS];

	LightSource* lights[MAX_LIGHTS];



	RenderTexture* renderTexture;


	// GUI variables
	float timeInSeconds;
	bool calmWaters, isToggled;
	float bloomIntensity, bloomThreshold, bloomGamma, waterTessellation;
	OrthoMesh* shadowOrthos[MAX_DEPTH_MAPS], * mainScene;
	int viewMode;
	float lightdir[3];
	float waveOneDir[2];
	float waveTwoDir[2];
	float waveThreeDir[2];
	float waveFourDir[2];
	bool displayShadowMaps[MAX_LIGHTS];
	

};

#endif