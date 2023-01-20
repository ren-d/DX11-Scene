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
	std::shared_ptr<AModel> boatModel,  crateModel,  barrelModel,  woodenBoxModel,  kegModel;
	std::unique_ptr<SphereMesh> sphere;
	std::unique_ptr<Water> water;
	std::unique_ptr<ModelObject> boat, crate, barrel, woodenBox, keg;
	
	
	// Shaders
	WaterShader* waterShader;
	WaterDepthShader* waterDepthShader;
	ModelShader* modelShader;
	DepthShader* depthShader;
	TextureShader* textureShader;
	ColourShader* colourShader;

	// Compute Shaders
	std::unique_ptr<ComputeBlend> computeBlend;
	std::unique_ptr<ComputeBrightness> computeBrightness;

	std::vector<std::unique_ptr<ComputeDownSample>> computeDownSample;
	std::vector <std::unique_ptr<ComputeUpSample>> computeUpSample;
	std::vector<std::unique_ptr<HorizontalBlurShader>> horizonalBlurShader;
	std::vector<std::unique_ptr<VerticalBlurShader>> verticalBlurShader;



	// Lighting
	static constexpr int MAX_LIGHTS = 4;
	static constexpr int MAX_DEPTH_MAPS_PER_LIGHT = 6;
	static constexpr int MAX_DEPTH_MAPS = 24;
	XMFLOAT3 directions[MAX_DEPTH_MAPS_PER_LIGHT];

	ShadowMap* shadowMaps[MAX_DEPTH_MAPS];

	LightSource* lights[MAX_LIGHTS];



	std::unique_ptr <RenderTexture> renderTexture;


	// GUI variables
	float timeInSeconds;
	bool calmWaters, isToggled;
	float bloomIntensity, bloomThreshold, bloomGamma, waterTessellation;
	std::unique_ptr<OrthoMesh> shadowOrthos[MAX_DEPTH_MAPS], mainScene;
	int viewMode;

	bool displayShadowMaps[MAX_LIGHTS];
	

};

#endif