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
#include "ParticleShader.h"
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
	static constexpr int MAX_DEPTH_MAPS_PER_LIGHT = 6;
	static constexpr int MAX_DEPTH_MAPS = 24;
	SceneObject* waterMesh;
	ModelObject* boat;
	AModel* boatModel;
	WaterShader* waterShader;
	WaterDepthShader* waterDepthShader;
	ModelShader* modelShader;
	DepthShader* depthShader;
	TextureShader* textureShader;
	ColourShader* colourShader;
	OrthoMesh* orthoMesh, *orthoMesh2;
	Water* water;
	ParticleShader* particleShader;
	SphereMesh* sphere;
	LightSource* lights[MAX_LIGHTS];
	ComputeDownSample* computeDownSample[3];
	ComputeBrightness* computeBrightness;
	ComputeUpSample* computeUpSample[3];
	ComputeBlend* computeBlend;
	HorizontalBlurShader* horizonalBlurShader[4];
	VerticalBlurShader* verticalBlurShader[4];
	RenderTexture* renderTexture, *renderTexture2;
	float lightOneColour[MAX_LIGHTS];
	float lightTwoColour[MAX_LIGHTS];
	float lightThreeColour[MAX_LIGHTS];
	float lightFourColour[MAX_LIGHTS];
	float ambientColour[MAX_LIGHTS];

	XMFLOAT3 directions[MAX_DEPTH_MAPS_PER_LIGHT];
	ShadowMap* shadowMaps[MAX_DEPTH_MAPS];
	bool displayShadowMaps;
	
	int viewMode;
	
	float timeInSeconds;

	// GUI variables
	float lightdir[3];
	float waveOneDir[2];
	float waveTwoDir[2];
	float waveThreeDir[2];
	float waveFourDir[2];
	
	float bloomIntensity, bloomThreshold, bloomGamma, waterTessellation;
};

#endif