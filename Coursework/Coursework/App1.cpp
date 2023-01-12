// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

App1::App1()
{

}
App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// delete heap allocated Models & Objects
	delete boatModel;
	boatModel = 0;
	delete crateModel;
	crateModel = 0;
	delete barrelModel;
	barrelModel = 0;
	delete woodenBoxModel;
	woodenBoxModel = 0;
	delete kegModel;
	kegModel = 0;

	delete sphere;
	sphere = 0;
	delete water;
	water = 0;
	delete boat;
	boat = 0;
	delete crate;
	crate = 0;
	delete barrel;
	barrel = 0;
	delete keg;
	keg = 0;

	// delete heap allocated shaders
	delete waterShader;
	waterShader = 0;
	delete waterDepthShader;
	waterDepthShader = 0;
	delete modelShader;
	modelShader = 0;
	delete depthShader;
	depthShader = 0;
	delete textureShader;
	textureShader = 0;
	delete colourShader;
	colourShader = 0;

	// delete heap allocated Compute Shaders
	delete[] computeDownSample;
	delete[] computeUpSample;
	delete[] horizonalBlurShader;
	delete[] verticalBlurShader;
	delete computeBrightness;
	delete computeBlend;

	// delete heap allocated lighting data
	delete[] shadowMaps;
	delete[] lights;
	

	//GUI and others
	delete[] shadowOrthos;
	delete renderTexture;
}


void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);


	//init
	initShadowMaps();
	initLighting();
	initTextures();
	initSceneObjects(&screenWidth, &screenHeight);
	initShaders(hwnd);
	initGUI();

}

void App1::initShadowMaps()
{
	const int shadowmapWidth = 2048;
	const int shadowmapHeight = 2048;

	int maxShadowMaps = MAX_LIGHTS * MAX_DEPTH_MAPS_PER_LIGHT;
	for (int i = 0; i < maxShadowMaps; i++)
	{
		shadowMaps[i] = new ShadowMap(renderer->getDevice(), shadowmapWidth, shadowmapHeight);
	}

	// directions to be used in point light shadowmap calculations
	directions[0] = XMFLOAT3(1, 0, 0);
	directions[1] = XMFLOAT3(-1, 0, 0);
	directions[2] = XMFLOAT3(0, 1, 0);
	directions[3] = XMFLOAT3(0, -1, 0);
	directions[4] = XMFLOAT3(0, 0, 1);
	directions[5] = XMFLOAT3(0, 0, -1);
}

void App1::initLighting()	// Initalise scene lighting.
{

	const int sceneWidth = 200;
	const int sceneHeight = 200;

	lights[0] = new LightSource();
	lights[0]->setLightType(LightSource::LType::DIRECTIONAL);
	lights[0]->setAmbientColour(0.2, 0.2, 0.2, 1.0f);
	lights[0]->setPosition(59.f, 36.f, 65.0f);
	lights[0]->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
	lights[0]->setDiffuseColour(0.2f, 0.2f, 0.3f, 1.0f);
	lights[0]->setDirection(-0.450f, -0.7f, 0.0f);
	lights[0]->setSpecularPower(100.0f);
	lights[0]->setConstantFactor(1.0f);
	lights[0]->setLinearFactor(0.14f);
	lights[0]->setQuadraticFactor(0.07f);
	lights[0]->setInnerSpotlightConeInDegrees(25);
	lights[0]->setOuterSpotlightConeInDegrees(35);
	lights[0]->generateOrthoMatrix((float)sceneWidth, (float)sceneHeight, 0.1f, 200.0f);
	lights[0]->init();

	lights[1] = new LightSource();
	lights[1]->setLightType(LightSource::LType::POINT);
	lights[1]->setPosition(22, 4.0, 65);
	lights[1]->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
	lights[1]->setAmbientColour(0.2, 0.2, 0.2, 1.0f);
	lights[1]->setDiffuseColour(0.0f, 0.9f, 1.0f, 1.0f);
	lights[1]->setDirection(0.45f, -0.5f, 0.75f);
	lights[1]->setSpecularPower(100.0f);
	lights[1]->setConstantFactor(1.0f);
	lights[1]->setLinearFactor(0.14f);
	lights[1]->setQuadraticFactor(0.07f);
	lights[1]->setInnerSpotlightConeInDegrees(25);
	lights[1]->setOuterSpotlightConeInDegrees(35);
	lights[1]->generateOrthoMatrix((float)sceneWidth, (float)sceneHeight, 0.1f, 200.0f);
	lights[1]->init();

	lights[2] = new LightSource();
	lights[2]->setLightType(LightSource::LType::POINT);
	lights[2]->setPosition(59.7, 4, 38);
	lights[2]->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
	lights[2]->setAmbientColour(0.2, 0.2, 0.2, 1.0f);
	lights[2]->setDiffuseColour(0.0f, 1.0f, 0.0f, 1.0f);
	lights[2]->setSpecularPower(100.0f);
	lights[2]->setDirection(0.45f, -0.5f, 0.75f);
	lights[2]->setConstantFactor(1.0f);
	lights[2]->setLinearFactor(0.14f);
	lights[2]->setQuadraticFactor(0.07f);
	lights[2]->setInnerSpotlightConeInDegrees(25);
	lights[2]->setOuterSpotlightConeInDegrees(35);
	lights[2]->generateOrthoMatrix((float)sceneWidth, (float)sceneHeight, 0.1f, 200.0f);
	lights[2]->init();

	lights[3] = new LightSource();
	lights[3]->setLightType(LightSource::LType::SPOTLIGHT);
	lights[3]->setPosition(30, 13, 33);
	lights[3]->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
	lights[3]->setAmbientColour(0.2, 0.2, 0.2, 1.0f);
	lights[3]->setDiffuseColour(1.0f, 1.0f, 0.0f, 1.0f);
	lights[3]->setDirection(0.0f, -1.0f, 0.132f);
	lights[3]->setSpecularPower(100.0f);
	lights[3]->setConstantFactor(1.0f);
	lights[3]->setLinearFactor(0.014f);
	lights[3]->setQuadraticFactor(0.0007f);
	lights[3]->setInnerSpotlightConeInDegrees(25);
	lights[3]->setOuterSpotlightConeInDegrees(35);
	lights[3]->generateOrthoMatrix((float)sceneWidth, (float)sceneHeight, 0.1f, 200.0f);
	lights[3]->init();

}	

void App1::initTextures() // load textures
{
	
	textureMgr->loadTexture(L"water", L"res/water.png");
	textureMgr->loadTexture(L"normal1", L"res/models/waternormal1.png");
	textureMgr->loadTexture(L"normal2", L"res/models/waternormal2.png");
	textureMgr->loadTexture(L"boatDiffuse", L"res/models/boatColor.png");
	textureMgr->loadTexture(L"boatNormal", L"res/models/boatNormal.png");
	textureMgr->loadTexture(L"boatSpec", L"res/models/boatMetallic.png");

	textureMgr->loadTexture(L"crateDiffuse", L"res/models/Crate_color.png");
	textureMgr->loadTexture(L"crateNormal", L"res/models/Crate_normal.png");
	textureMgr->loadTexture(L"crate", L"res/models/boatMetallic.png");


	textureMgr->loadTexture(L"woodBarrelDiffuse", L"res/models/wooden_crate_barrel_BaseColor.jpg");
	textureMgr->loadTexture(L"woodBarrelNormal", L"res/models/wooden_crate_barrel_Normal.jpg");
	textureMgr->loadTexture(L"woodBarrelSpec", L"res/models/wooden_crate_barrel_Metallic.jpg");
}

void App1::initSceneObjects(int* screenWidth, int* screenHeight) // initialise scene objects
{
	// shadowmap orthos definitions
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		int index = (i * MAX_DEPTH_MAPS_PER_LIGHT);
		shadowOrthos[index] = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), *screenWidth / 8, *screenHeight / 8, -*screenWidth / 2.2, *screenHeight / 2.2);
		shadowOrthos[index+1] = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), *screenWidth / 8, *screenHeight / 8, -*screenWidth / 3.0, *screenHeight / 2.2);
		shadowOrthos[index+2] = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), *screenWidth / 8, *screenHeight / 8, -*screenWidth / 4.78, *screenHeight / 2.2);
		shadowOrthos[index+3] = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), *screenWidth / 8, *screenHeight / 8, -*screenWidth / 11.18, *screenHeight / 2.2);
		shadowOrthos[index+4] = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), *screenWidth / 8, *screenHeight / 8, *screenWidth / 30.2, *screenHeight / 2.2);
		shadowOrthos[index+5] = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), *screenWidth / 8, *screenHeight / 8, *screenWidth / 6.5, *screenHeight / 2.2);
	}
	

	// main object & model definitions
	mainScene = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), *screenWidth, *screenHeight);
	sphere = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());
	water = new Water(renderer->getDevice(), renderer->getDeviceContext(), textureMgr->getTexture(L"water"), textureMgr->getTexture(L"normal1"), textureMgr->getTexture(L"normal2"));
	water->setMesh(new TessellationPlane(renderer->getDevice()));

	boatModel = new AModel(renderer->getDevice(), "res/models/boat.fbx");
	boat = new ModelObject(renderer->getDevice(), renderer->getDeviceContext(), textureMgr->getTexture(L"boatDiffuse"), textureMgr->getTexture(L"boatNormal"), textureMgr->getTexture(L"boatSpec"));
	boat->setModel(boatModel);

	renderTexture = new RenderTexture(renderer->getDevice(), *screenWidth, *screenHeight, SCREEN_NEAR, SCREEN_DEPTH);

	crateModel = new AModel(renderer->getDevice(), "res/models/Crate.fbx");
	crate = new ModelObject(renderer->getDevice(), renderer->getDeviceContext(), textureMgr->getTexture(L"crateDiffuse"), textureMgr->getTexture(L"crateNormal"), textureMgr->getTexture(L"crate"));
	crate->setModel(crateModel);

	barrelModel = new AModel(renderer->getDevice(), "res/models/barrel.fbx");
	barrel = new ModelObject(renderer->getDevice(), renderer->getDeviceContext(), textureMgr->getTexture(L"woodBarrelDiffuse"), textureMgr->getTexture(L"woodBarrelNormal"), textureMgr->getTexture(L"woodBarrelSpec"));
	barrel->setModel(barrelModel);

	woodenBoxModel = new AModel(renderer->getDevice(), "res/models/woodenBox.fbx");
	woodenBox = new ModelObject(renderer->getDevice(), renderer->getDeviceContext(), textureMgr->getTexture(L"woodBarrelDiffuse"), textureMgr->getTexture(L"woodBarrelNormal"), textureMgr->getTexture(L"woodBarrelSpec"));
	woodenBox->setModel(woodenBoxModel);

	kegModel = new AModel(renderer->getDevice(), "res/models/keg.fbx");
	keg = new ModelObject(renderer->getDevice(), renderer->getDeviceContext(), textureMgr->getTexture(L"woodBarrelDiffuse"), textureMgr->getTexture(L"woodBarrelNormal"), textureMgr->getTexture(L"woodBarrelSpec"));
	keg->setModel(kegModel);
}

void App1::initShaders(HWND hwnd)
{

	waterShader = new WaterShader(renderer->getDevice(), hwnd);
	waterDepthShader = new WaterDepthShader(renderer->getDevice(), hwnd);
	modelShader = new ModelShader(renderer->getDevice(), hwnd);
	depthShader = new DepthShader(renderer->getDevice(), hwnd);
	textureShader = new TextureShader(renderer->getDevice(), hwnd);
	colourShader = new ColourShader(renderer->getDevice(), hwnd);


	// compute shaders  
	// (sWidth / x) and (sHeight / x) 
	// is in regards to the output texture size

	computeBrightness = new ComputeBrightness(renderer->getDevice(), hwnd, sWidth, sHeight);

	computeDownSample[0] = new ComputeDownSample(renderer->getDevice(), hwnd, ceil(sWidth/2) , ceil(sHeight/2));
	computeDownSample[1] = new ComputeDownSample(renderer->getDevice(), hwnd, ceil(sWidth / 4), ceil(sHeight / 4));
	computeDownSample[2] = new ComputeDownSample(renderer->getDevice(), hwnd, ceil(sWidth / 6), ceil(sHeight / 6));

	computeUpSample[0] = new ComputeUpSample(renderer->getDevice(), hwnd, ceil(sWidth / 4), ceil(sHeight / 4));
	computeUpSample[1] = new ComputeUpSample(renderer->getDevice(), hwnd, ceil(sWidth / 2), ceil(sHeight / 2));
	computeUpSample[2] = new ComputeUpSample(renderer->getDevice(), hwnd, sWidth, sHeight);

	horizonalBlurShader[0] = new HorizontalBlurShader(renderer->getDevice(), hwnd, ceil(sWidth / 6), ceil(sHeight / 6));
	verticalBlurShader[0] = new VerticalBlurShader(renderer->getDevice(), hwnd, ceil(sWidth / 6), ceil(sHeight / 6));
	horizonalBlurShader[1] = new HorizontalBlurShader(renderer->getDevice(), hwnd, ceil(sWidth / 4), ceil(sHeight / 4));
	verticalBlurShader[1] = new VerticalBlurShader(renderer->getDevice(), hwnd, ceil(sWidth / 4), ceil(sHeight / 4));
	horizonalBlurShader[2] = new HorizontalBlurShader(renderer->getDevice(), hwnd, ceil(sWidth / 2), ceil(sHeight / 2));
	verticalBlurShader[2] = new VerticalBlurShader(renderer->getDevice(), hwnd, ceil(sWidth / 2), ceil(sHeight / 2));
	horizonalBlurShader[3] = new HorizontalBlurShader(renderer->getDevice(), hwnd, sWidth, sHeight);
	verticalBlurShader[3] = new VerticalBlurShader(renderer->getDevice(), hwnd, sWidth, sHeight);


	computeBlend = new ComputeBlend(renderer->getDevice(), hwnd, sWidth, sHeight);


}

void App1::initGUI() // Setup GUI Variables
{
	

	waveOneDir[0] = water->getWave(0)->direction.x;
	waveOneDir[1] = water->getWave(0)->direction.y;
	waveTwoDir[0] = water->getWave(1)->direction.x;
	waveTwoDir[1] = water->getWave(1)->direction.y;
	waveThreeDir[0] = water->getWave(2)->direction.x;
	waveThreeDir[1] = water->getWave(2)->direction.y;
	waveFourDir[0] = water->getWave(3)->direction.x;
	waveFourDir[1] = water->getWave(3)->direction.y;

	bloomIntensity = 0.8f;
	bloomThreshold = 0.45f;
	bloomGamma = 1.1f;
	waterTessellation = 1.0f;
	viewMode = 1;

	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		displayShadowMaps[i] = false;
	}

	calmWaters = false;
}



void App1::updateInput() // update variables based on the GUI input
{
	water->setWaveDir(0, XMFLOAT2(waveOneDir[0], waveOneDir[1]));
	water->setWaveDir(1, XMFLOAT2(waveTwoDir[0], waveTwoDir[1]));
	water->setWaveDir(2, XMFLOAT2(waveThreeDir[0], waveThreeDir[1]));
	water->setWaveDir(3, XMFLOAT2(waveFourDir[0], waveFourDir[1]));

}

bool App1::frame()
{
	
	bool result;
	
	updateInput();
	
	result = BaseApplication::frame();

	timeInSeconds += timer->getTime();

	if (!result)
	{
		return false;
	}
	
	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool App1::render()
{

	// Wave Settings
	if (calmWaters)
	{
		water->getWave(0)->steepness = 0.0f;
		water->getWave(1)->steepness = 0.0f;
		water->getWave(2)->steepness = 0.0f;
		isToggled = false;
	}
	else if(!isToggled && !calmWaters)
	{

		water->getWave(0)->steepness = 0.264f;

		water->getWave(1)->steepness = 0.236f;

		water->getWave(2)->steepness = 0.402f;
		isToggled = true;

	}


	// viewmode ensures that switching between shaders and non shaders is easy (helps with wireframe)
	switch (viewMode)
	{
	case 1:
		depthpass();
		renderTexture->setRenderTarget(renderer->getDeviceContext());
		renderTexture->clearRenderTarget(renderer->getDeviceContext(), 0.0f, 0.0f, 0.0f, 0.0f);
		basepass();
		computepass();
		finalpass();

		break;
	default:
		renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);
		depthpass();
		basepass();
		
		break;
	}

	gui();


	renderer->endScene();


	return true;
}


void App1::renderDepthScene(XMMATRIX lightViewMatrix, XMMATRIX lightProjectionMatrix, XMMATRIX worldMatrix)
{
	// renders depth values
	water->renderDepth(worldMatrix, lightViewMatrix, lightProjectionMatrix, waterDepthShader, timeInSeconds, camera); // render water

	worldMatrix *= XMMatrixTranslation(20, 1, 70);
	crate->renderDepth(worldMatrix, lightViewMatrix, lightProjectionMatrix, depthShader); // render crate

	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixScaling(0.1 * 0.5, 0.1 * 0.5, 0.1 * 0.5);
	worldMatrix *= XMMatrixTranslation(30, 1, 40);
	barrel->renderDepth(worldMatrix, lightViewMatrix, lightProjectionMatrix, depthShader); // render barrel

	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixScaling(0.1 * 0.5, 0.1 * 0.5, 0.1 * 0.5);
	worldMatrix *= XMMatrixTranslation(50, 2, 20);
	woodenBox->renderDepth(worldMatrix, lightViewMatrix, lightProjectionMatrix, depthShader); // render woodbox

	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixScaling(0.1 * 0.5, 0.1 * 0.5, 0.1 * 0.5);
	worldMatrix *= XMMatrixTranslation(70, 2, 70);
	keg->renderDepth(worldMatrix, lightViewMatrix, lightProjectionMatrix, depthShader); // render keg

	worldMatrix = renderer->getWorldMatrix();
	worldMatrix *= XMMatrixScaling(2, 2, 2);
	worldMatrix *= XMMatrixRotationY(XMConvertToRadians(-90));
	worldMatrix *= XMMatrixTranslation(-10, 1, 60);
	woodenBox->renderDepth(worldMatrix, lightViewMatrix, lightProjectionMatrix, depthShader); // render woodbox (LARGE)

	worldMatrix = renderer->getWorldMatrix();
	worldMatrix *= XMMatrixScaling(2, 2, 2);
	worldMatrix *= XMMatrixRotationY(XMConvertToRadians(90));
	worldMatrix *= XMMatrixTranslation(110, 1, 60);
	woodenBox->renderDepth(worldMatrix, lightViewMatrix, lightProjectionMatrix, depthShader); // render woodbox (LARGE)

	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixScaling(0.1 * 0.5, 0.1 * 0.5, 0.1 * 0.5);
	worldMatrix *= XMMatrixTranslation(60, 1, 40);
	boat->renderDepth(worldMatrix, lightViewMatrix, lightProjectionMatrix, depthShader); // render boat
}


void App1::depthpass()
{
	// renders depthmap to depth buffer based on light type

	// NOTE: i*MAX_DEPTHS_PER_LIGHT refers to the max amount of depht maps per light
	// this means that each light is set to have 6 depth maps
	// the stored depth map is fragmented so that
	// light[0] = depthmap[0]...[5]
	// light[1] = depthmap[6]...[11]
	// light[2] = depthmap[12]...[17]
	// light[3] = depthmap[18]...[23]


	// directional lights: 1 depth map
	// point lights: 6 depth maps
	// spot lights: 1 depth map
	XMMATRIX lightViewMatrix, lightProjectionMatrix, worldMatrix;
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		switch (static_cast<LightSource::LType>(lights[i]->getLightType()))
		{
		case LightSource::LType::DIRECTIONAL:
			shadowMaps[i * MAX_DEPTH_MAPS_PER_LIGHT]->BindDsvAndSetNullRenderTarget(renderer->getDeviceContext());
			lights[i]->generateViewMatrix();

			lightViewMatrix = lights[i]->getViewMatrix();
			lightProjectionMatrix = lights[i]->getOrthoMatrix();
			worldMatrix = renderer->getWorldMatrix();

			renderDepthScene(lightViewMatrix, lightProjectionMatrix, worldMatrix);

			renderer->setBackBufferRenderTarget();
			renderer->resetViewport();

			break;
		case LightSource::LType::POINT:
			for (int j = 0; j < MAX_DEPTH_MAPS_PER_LIGHT; j++) // renders to 6 depth maps for point light cubemap
			{
				int indexshadow = (i * MAX_DEPTH_MAPS_PER_LIGHT ) + j;
				shadowMaps[indexshadow]->BindDsvAndSetNullRenderTarget(renderer->getDeviceContext());

				lights[i]->setDirection(directions[j].x, directions[j].y, directions[j].z);
				lights[i]->generateViewMatrix();
				lights[i]->generateProjectionMatrix(1.0f, 100.f);
				
				lightViewMatrix = lights[i]->getViewMatrix();
				lightProjectionMatrix = lights[i]->getProjectionMatrix();
				worldMatrix = renderer->getWorldMatrix();

				renderDepthScene(lightViewMatrix, lightProjectionMatrix, worldMatrix);

				renderer->setBackBufferRenderTarget();
				renderer->resetViewport();
			}
			
			break;
		case LightSource::LType::SPOTLIGHT:
                         
			shadowMaps[i * MAX_DEPTH_MAPS_PER_LIGHT]->BindDsvAndSetNullRenderTarget(renderer->getDeviceContext());
			lights[i]->generateViewMatrix();
			float fov = XMConvertToRadians(*lights[i]->getOuterCone()) * 2.0f; // recalculate fov based on the cone of the spotlight

			lightViewMatrix = lights[i]->getViewMatrix();
			lightProjectionMatrix = XMMatrixPerspectiveFovLH(fov, 1, 5.0f, 100.f);
			worldMatrix = renderer->getWorldMatrix();

			renderDepthScene(lightViewMatrix, lightProjectionMatrix, worldMatrix);

			renderer->setBackBufferRenderTarget();
			renderer->resetViewport();
			break;
		}
	}
}

void App1::computepass() // compute shaders used for post processing
{
	float groupSizeX;
	float groupSizeY;

	// gets brightest pixels and renders them to a SRV
	computeBrightness->setShaderParameters(renderer->getDeviceContext(), renderTexture->getShaderResourceView(), bloomThreshold);
	computeBrightness->compute(renderer->getDeviceContext(), ceil((float)sWidth/8), ceil((float)sHeight/8), 1); // groups are height / 8 to correspond to threads per group
	computeBrightness->unbind(renderer->getDeviceContext());


	// downscale image
	for (int i = 0; i < 3; i++)
	{
		
		if (i == 0)
		{
			computeDownSample[i]->setShaderParameters(renderer->getDeviceContext(), computeBrightness->getSRV());
			computeDownSample[i]->compute(renderer->getDeviceContext(), ceil((float)sWidth / 8), ceil((float)sHeight / 8), 1);
			computeDownSample[i]->unbind(renderer->getDeviceContext());
		}
		else
		{
			groupSizeX = ceil((float)(sWidth / (2 * i)) / 8);
			groupSizeY = ceil((float)(sHeight / (2 * i)) / 8);
			computeDownSample[i]->setShaderParameters(renderer->getDeviceContext(), computeDownSample[i-1]->getSRV());
			computeDownSample[i]->compute(renderer->getDeviceContext(), groupSizeX, groupSizeY, 1);
			computeDownSample[i]->unbind(renderer->getDeviceContext());
		}



	}

	// apply gaussian blur
	horizonalBlurShader[0]->setShaderParameters(renderer->getDeviceContext(), computeDownSample[2]->getSRV());
	horizonalBlurShader[0]->compute(renderer->getDeviceContext(), ceil((float)sWidth / 256.f), sHeight, 1); // groups are / 256 due to shader having 256 threads per group
	horizonalBlurShader[0]->unbind(renderer->getDeviceContext());

	verticalBlurShader[0]->setShaderParameters(renderer->getDeviceContext(), horizonalBlurShader[0]->getSRV());
	verticalBlurShader[0]->compute(renderer->getDeviceContext(), sWidth, ceil((float)sHeight / 256.f), 1);
	verticalBlurShader[0]->unbind(renderer->getDeviceContext());

	// upscale image
	for (int i = 0; i < 3; i++)
	{
		groupSizeX = ceil((float)(sWidth / (6 - (2 * i))) / 8); // hacky formula to work out up scaling division based on index
		groupSizeY = ceil((float)(sWidth / (6 - (2 * i))) / 8);

		computeUpSample[i]->setShaderParameters(renderer->getDeviceContext(), verticalBlurShader[i]->getSRV());
		computeUpSample[i]->compute(renderer->getDeviceContext(), groupSizeX, groupSizeY, 1);
		computeUpSample[i]->unbind(renderer->getDeviceContext());

		// apply gaussian blur
		horizonalBlurShader[i+1]->setShaderParameters(renderer->getDeviceContext(), computeUpSample[i]->getSRV());
		horizonalBlurShader[i+1]->compute(renderer->getDeviceContext(), ceil((float)sWidth / 256.f), sHeight, 1);
		horizonalBlurShader[i+1]->unbind(renderer->getDeviceContext());

		verticalBlurShader[i+1]->setShaderParameters(renderer->getDeviceContext(), horizonalBlurShader[i+1]->getSRV());
		verticalBlurShader[i+1]->compute(renderer->getDeviceContext(), sWidth, ceil((float)sHeight / 256.f), 1);
		verticalBlurShader[i+1]->unbind(renderer->getDeviceContext());



	}

	//blend final brightness image with orignal scene to create "bloom"
	computeBlend->setShaderParameters(renderer->getDeviceContext(), renderTexture->getShaderResourceView(), verticalBlurShader[3]->getSRV(), bloomIntensity, bloomGamma);
	computeBlend->compute(renderer->getDeviceContext(), ceil((float)sWidth / 8), ceil((float)sHeight / 8), 1);
	computeBlend->unbind(renderer->getDeviceContext());

}


void App1::basepass()
{

	
	camera->update();


	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();


	// render world
	water->render(worldMatrix, viewMatrix, projectionMatrix, waterShader, lights, shadowMaps, timeInSeconds, camera, waterTessellation, viewMode);
	
	worldMatrix *= XMMatrixTranslation(20, 1, 70);
	crate->render(worldMatrix, viewMatrix, projectionMatrix, modelShader, lights, camera, shadowMaps, viewMode);
	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixScaling(0.1 * 0.5, 0.1 * 0.5, 0.1 * 0.5);
	worldMatrix *= XMMatrixTranslation(30, 1, 40);
	barrel->render(worldMatrix, viewMatrix, projectionMatrix, modelShader, lights, camera, shadowMaps, viewMode);
	worldMatrix = renderer->getWorldMatrix();

	worldMatrix = XMMatrixScaling(0.1 * 0.5, 0.1 * 0.5, 0.1 * 0.5);
	worldMatrix *= XMMatrixTranslation(50, 2, 20);
	woodenBox->render(worldMatrix, viewMatrix, projectionMatrix, modelShader, lights, camera, shadowMaps, viewMode);
	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixScaling(0.1 * 0.5, 0.1 * 0.5, 0.1 * 0.5);
	worldMatrix *= XMMatrixTranslation(70, 2, 70);
	keg->render(worldMatrix, viewMatrix, projectionMatrix, modelShader, lights, camera, shadowMaps, viewMode);
	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixScaling(0.1 * 0.5, 0.1 * 0.5, 0.1 * 0.5);
	worldMatrix *= XMMatrixTranslation(60, 1, 40);

	boat->render(worldMatrix, viewMatrix, projectionMatrix, modelShader, lights, camera, shadowMaps, viewMode);
	worldMatrix = renderer->getWorldMatrix();
	worldMatrix *= XMMatrixScaling(2,2,2);
	worldMatrix *= XMMatrixRotationY(XMConvertToRadians(-90));
	worldMatrix *= XMMatrixTranslation(-10, 1, 60);
	woodenBox->render(worldMatrix, viewMatrix, projectionMatrix, modelShader, lights, camera, shadowMaps, viewMode);

	worldMatrix = renderer->getWorldMatrix();
	worldMatrix *= XMMatrixScaling(2, 2, 2);
	worldMatrix *= XMMatrixRotationY(XMConvertToRadians(90));
	worldMatrix *= XMMatrixTranslation(110, 1, 60);
	woodenBox->render(worldMatrix, viewMatrix, projectionMatrix, modelShader, lights, camera, shadowMaps, viewMode);

	// render visible spheres where lights are located
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		worldMatrix = XMMatrixTranslation(lights[i]->getPosition().x, lights[i]->getPosition().y, lights[i]->getPosition().z);
		sphere->sendData(renderer->getDeviceContext());
		colourShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, lights[i]->getDiffuseColour());
		colourShader->render(renderer->getDeviceContext(), sphere->getIndexCount());
	}

	
	// display shadow maps based on GUI
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		if (displayShadowMaps[i])
		{
			worldMatrix = renderer->getWorldMatrix();

			renderer->setZBuffer(false);
			XMMATRIX orthoMatrix = renderer->getOrthoMatrix();  // ortho matrix for 2D rendering
			XMMATRIX orthoViewMatrix = camera->getOrthoViewMatrix();	// Default camera position for orthographic rendering
			for (int j = 0; j < MAX_DEPTH_MAPS_PER_LIGHT; j++)
			{
				int index = (i * MAX_DEPTH_MAPS_PER_LIGHT) + j;
				shadowOrthos[index]->sendData(renderer->getDeviceContext());
				textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, shadowMaps[index]->getDepthMapSRV());
				textureShader->render(renderer->getDeviceContext(), shadowOrthos[index]->getIndexCount());
			}

			renderer->setZBuffer(true);
		}
	}

	
	renderer->setBackBufferRenderTarget();

}


void App1::finalpass()
{
	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// RENDER THE RENDER TEXTURE SCENE
	// Requires 2D rendering and an ortho mesh.
	renderer->setZBuffer(false);
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX orthoMatrix = renderer->getOrthoMatrix();  // ortho matrix for 2D rendering
	XMMATRIX orthoViewMatrix = camera->getOrthoViewMatrix();	// Default camera position for orthographic rendering

	mainScene->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, computeBlend->getSRV());
	textureShader->render(renderer->getDeviceContext(), mainScene->getIndexCount());
	renderer->setZBuffer(true);


}


void App1::gui()
{
	// Force turn off unnecessary shader stages.
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);

	// Build UI
	ImGui::Text("FPS: %.2f", timer->getFPS());
	ImGui::Checkbox("Wireframe mode", &wireframeToggle);
	if (wireframeToggle)
	{
		viewMode = 0;
	}

	// drop down list for how to render scene
	// changes "viewMode" integer which is used in multiple state machines
	const char* LIST_ITEMS[] = { "Base", "Shaders", "UV", "Normals" };

	ImGui::ListBox("View Mode", &viewMode, LIST_ITEMS, IM_ARRAYSIZE(LIST_ITEMS), 4);

	// Water GUI
	if (ImGui::CollapsingHeader("water"))
	{
		ImGui::Checkbox("calm waters", &calmWaters);
		ImGui::SliderFloat("Tessellation", &waterTessellation, 0, 20);
		if (ImGui::CollapsingHeader("wave 1"))
		{
			ImGui::SliderFloat2("Direction1", waveOneDir, -1.0f, 1.0f);
			ImGui::SliderFloat("Steepness1", &water->getWave(0)->steepness, 0.0f, 1.0f);
			ImGui::SliderFloat("Wave Length1", &water->getWave(0)->waveLength, 0.0f, 100);
		}
		
		if (ImGui::CollapsingHeader("wave 2"))
		{
			ImGui::SliderFloat2("Direction2", waveTwoDir, -1.0f, 1.0f);
			ImGui::SliderFloat("Steepness2", &water->getWave(1)->steepness, 0.0f, 1.0f);
			ImGui::SliderFloat("Wave Length2", &water->getWave(1)->waveLength, 0.0f, 100);
		}

		if (ImGui::CollapsingHeader("wave 3"))
		{
			ImGui::SliderFloat2("Direction3", waveThreeDir, -1.0f, 1.0f);
			ImGui::SliderFloat("Steepness3", &water->getWave(2)->steepness, 0.0f, 1.0f);
			ImGui::SliderFloat("Wave Length3", &water->getWave(2)->waveLength, 0.0f, 100);
		}

		if (ImGui::CollapsingHeader("wave 4"))
		{
			ImGui::SliderFloat2("Direction4", waveFourDir, -1.0f, 1.0f);
			ImGui::SliderFloat("Steepness4", &water->getWave(3)->steepness, 0.0f, 1.0f);
			ImGui::SliderFloat("Wave Length4", &water->getWave(3)->waveLength, 0.0f, 100);
		}

	}

	// Lighting GUI
	const char* LIGHT_ITEMS[] = { "Directional", "Point", "Spot" };
	if(ImGui::CollapsingHeader("lights"))
	{
		ImGui::ColorEdit4("ambient", lights[0]->getAmbientColourFloatArray());
		

		for (int i = 0; i < MAX_LIGHTS; i++)
		{
			// define light headers
			std::string mainHeaderName = "light ";
			std::string shadowMapName = "display shadow map ";
			std::string listboxName = "light type ";
			std::string positionName = "position ";
			std::string directionName = "direction ";
			std::string colourName = "colour ";
			std::string specularColourName = "specular colour ";
			std::string specularPowerName = "specular power ";
			std::string linearFactorName = "linear factor ";
			std::string quadraticFactorName = "quadratic factor ";
			std::string outerConeName = "outer cone (degrees) ";
			std::string innerConeName = "inner cone (degrees) ";
			std::string indexAsString = std::to_string(i);

			// append current index to headers
			mainHeaderName += indexAsString;
			shadowMapName += indexAsString;
			listboxName += indexAsString;
			positionName += indexAsString;
			directionName += indexAsString;
			colourName += indexAsString;
			specularColourName += indexAsString;
			specularPowerName += indexAsString;
			linearFactorName += indexAsString;
			quadraticFactorName += indexAsString;
			outerConeName += indexAsString;
			innerConeName += indexAsString;

			if (ImGui::CollapsingHeader(mainHeaderName.c_str()))
			{
				
				ImGui::Checkbox(shadowMapName.c_str(), &displayShadowMaps[i]);
				// disables other visible shadow map ortho mesh apart from this one
				if (displayShadowMaps[i])
				{
					for (int j = 0; j < MAX_LIGHTS; j++)
					{
						if (i != j)
						{
							displayShadowMaps[j] = false;
						}

					}
				}

				// drop down box to switch light types
				int currentSelection = (int)lights[i]->getLightType();
				ImGui::ListBox(listboxName.c_str() , &currentSelection, LIGHT_ITEMS, IM_ARRAYSIZE(LIGHT_ITEMS), 3);

				switch (currentSelection)
				{
				case 0:
					lights[i]->setLightType(LightSource::LType::DIRECTIONAL);
					ImGui::SliderFloat3(positionName.c_str(), lights[i]->getPositionFloatArray(), -150.0f, 150.0f);
					ImGui::SliderFloat3(directionName.c_str(), lights[i]->getDirectionFloatArray(), -0.99f, 0.99f);
					ImGui::ColorEdit4(colourName.c_str(), lights[i]->getDiffuseColourFloatArray());


					break;
				case 1:
					lights[i]->setLightType(LightSource::LType::POINT);
					ImGui::SliderFloat3(positionName.c_str(), lights[i]->getPositionFloatArray(), 0, 100);
					ImGui::ColorEdit4(colourName.c_str(), lights[i]->getDiffuseColourFloatArray());
					ImGui::ColorEdit4(specularColourName.c_str(), lights[i]->getSpecularColourFloatArray());
					ImGui::SliderFloat(specularPowerName.c_str(), lights[i]->getSpecularPower(), 0.00f, 100.0f);
					ImGui::SliderFloat(linearFactorName.c_str(), lights[i]->getLinearfactor(), 0.014f, 0.35);
					ImGui::SliderFloat(quadraticFactorName.c_str(), lights[i]->getQuadraticFactor(), 0.0007f, 0.44);
					break;
				case 2:
					lights[i]->setLightType(LightSource::LType::SPOTLIGHT);
					ImGui::SliderFloat3(positionName.c_str(), lights[i]->getPositionFloatArray(), 0, 100);
					ImGui::SliderFloat3(directionName.c_str(), lights[i]->getDirectionFloatArray(), -1.0f, 1.0f);
					ImGui::ColorEdit4(colourName.c_str(), lights[i]->getDiffuseColourFloatArray());
					ImGui::ColorEdit4(specularColourName.c_str(), lights[i]->getSpecularColourFloatArray());
					ImGui::SliderFloat(specularPowerName.c_str(), lights[i]->getSpecularPower(), 0.00f, 100.0f);
					ImGui::SliderFloat(outerConeName.c_str(), lights[i]->getOuterCone(), *lights[i]->getInnerCone(), 90.0f);
					ImGui::SliderFloat(innerConeName.c_str(), lights[i]->getInnerCone(), 0.1f, *lights[i]->getOuterCone());
					ImGui::SliderFloat(linearFactorName.c_str(), lights[i]->getLinearfactor(), 0.014, 0.35);
					ImGui::SliderFloat(quadraticFactorName.c_str(), lights[i]->getQuadraticFactor(), 0.0007, 0.44);
					break;
				}
				lights[i]->update();


			}
		}
		// ensures ambient is the same
		lights[0]->update();

		
	}

	// Bloom GUI
	if (ImGui::CollapsingHeader("Post Processing"))
	{
		ImGui::SliderFloat("bloom threshold", &bloomThreshold, 0.0f, 1.0f);
		ImGui::SliderFloat("bloom intensity", &bloomIntensity, 0.0f, 1.0f);
		ImGui::SliderFloat("bloom gamma", &bloomGamma, 0.0f, 3.0f);
		
	}
	
	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

