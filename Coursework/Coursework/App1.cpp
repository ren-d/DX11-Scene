// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

App1::App1()
{

}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	// Initalise scene lighting.
	lights[0] = new LightSource();
	lights[0]->setLightType(LightSource::LType::DIRECTIONAL);
	lights[0]->setAmbientColour(0.2, 0.2, 0.2, 1.0f);
	lights[0]->setPosition(0.2, 0.2, 0.2);
	lights[0]->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
	lights[0]->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	lights[0]->setDirection(0.45f, 0.5f, 0.50f);
	lights[0]->setSpecularPower(100.0f);
	lights[0]->setConstantFactor(1.5f);
	lights[0]->setLinearFactor(0.125f);
	lights[0]->setQuadraticFactor(0.2f);


	lights[1] = new LightSource();
	lights[1]->setLightType(LightSource::LType::POINT);
	lights[1]->setPosition(1.0, 1.0, 30);
	lights[1]->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
	lights[1]->setAmbientColour(0.2, 0.2, 0.2, 1.0f);
	lights[1]->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	lights[1]->setDirection(0.45f, -0.5f, 0.75f);
	lights[1]->setSpecularPower(100.0f);
	lights[1]->setConstantFactor(1.5f);
	lights[1]->setLinearFactor(0.125f);
	lights[1]->setQuadraticFactor(0.2f);

	lights[2] = new LightSource();
	lights[2]->setLightType(LightSource::LType::POINT);
	lights[2]->setPosition(0.2, 0.2, 0.2);
	lights[2]->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
	lights[2]->setAmbientColour(0.2, 0.2, 0.2, 1.0f);
	lights[2]->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	lights[2]->setSpecularPower(100.0f);
	lights[2]->setDirection(0.45f, -0.5f, 0.75f);
	lights[2]->setConstantFactor(1.5f);
	lights[2]->setLinearFactor(0.125f);
	lights[2]->setQuadraticFactor(0.2f);

	lights[3] = new LightSource();
	lights[3]->setLightType(LightSource::LType::DIRECTIONAL);
	lights[3]->setPosition(0.2, 0.2, 0.2);
	lights[3]->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
	lights[3]->setAmbientColour(0.2, 0.2, 0.2, 1.0f);
	lights[3]->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	lights[3]->setDirection(0.45f, -0.5f, 0.75f);
	lights[3]->setSpecularPower(100.0f);
	lights[3]->setConstantFactor(1.5f);
	lights[3]->setLinearFactor(0.125f);
	lights[3]->setQuadraticFactor(0.2f);

	// load textures
	textureMgr->loadTexture(L"water", L"res/water.png");
	textureMgr->loadTexture(L"height", L"res/height.png");

	// initialise scene objects
	water = new Water(renderer->getDevice(), renderer->getDeviceContext(), textureMgr->getTexture(L"water"), textureMgr->getTexture(L"height"));
	water->setMesh(new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext()));
	
	// initialise shaders
	waterShader = new WaterShader(renderer->getDevice(), hwnd);

	// Setup GUI Variables
	lightdir[0] = lights[0]->getDirection().x;
	lightdir[1] = lights[0]->getDirection().y;
	lightdir[2] = lights[0]->getDirection().z;

	lightOneColour[0] = lights[0]->getDiffuseColour().x;
	lightOneColour[1] = lights[0]->getDiffuseColour().y;
	lightOneColour[2] = lights[0]->getDiffuseColour().z;
	lightOneColour[3] = lights[0]->getDiffuseColour().w;
	
	ambientColour[0] = lights[0]->getAmbientColour().x;
	ambientColour[1] = lights[0]->getAmbientColour().y;
	ambientColour[2] = lights[0]->getAmbientColour().z;
	ambientColour[3] = lights[0]->getAmbientColour().w;

	waveOneDir[0] = water->getWave(0)->direction.x;
	waveOneDir[1] = water->getWave(0)->direction.y;
	waveTwoDir[0] = water->getWave(1)->direction.x;
	waveTwoDir[1] = water->getWave(1)->direction.y;
	waveThreeDir[0] = water->getWave(2)->direction.x;
	waveThreeDir[1] = water->getWave(2)->direction.y;
	waveFourDir[0] = water->getWave(3)->direction.x;
	waveFourDir[1] = water->getWave(3)->direction.y;


}


App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

}


bool App1::frame()
{
	
	bool result;

	//update variables based on the GUI input
	lights[0]->setDirection(lightdir[0], lightdir[1], lightdir[2]);
	lights[0]->setDiffuseColour(lightOneColour[0], lightOneColour[1], lightOneColour[2], lightOneColour[3]);
	lights[0]->setAmbientColour(ambientColour[0], ambientColour[1], ambientColour[2], ambientColour[3]);
	water->setWaveDir(0, XMFLOAT2(waveOneDir[0], waveOneDir[1]));
	water->setWaveDir(1, XMFLOAT2(waveTwoDir[0], waveTwoDir[1]));
	water->setWaveDir(2, XMFLOAT2(waveThreeDir[0], waveThreeDir[1]));
	water->setWaveDir(3, XMFLOAT2(waveFourDir[0], waveFourDir[1]));


	result = BaseApplication::frame();

	deltaTime += timer->getTime();

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
	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();


	basepass();
	// Render GUI
	gui();

	// Present the rendered scene to the screen.
	renderer->endScene();

	return true;
}

void App1::basepass()
{

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();

	// Render Objects
	water->render(worldMatrix, viewMatrix, projectionMatrix, waterShader, lights, deltaTime);

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

	// Water GUI
	if (ImGui::CollapsingHeader("water"))
	{
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
	if(ImGui::CollapsingHeader("lights"))
	{
		ImGui::ColorEdit4("ambient", ambientColour);
		if (ImGui::CollapsingHeader("light 1"))
		{
			
			const char* LIST_ITEMS[] = { "Directional", "Point", "Spot" };
			static int currentSelection = (int)lights[0]->getLightType();
			ImGui::ListBox("listbox", &currentSelection, LIST_ITEMS, IM_ARRAYSIZE(LIST_ITEMS), 3);
			switch (currentSelection)
			{
			case 0:
				ImGui::SliderFloat3("direction 1", lightdir, -1.0f, 1.0f);
				ImGui::ColorEdit4("colour", lightOneColour);
				break;
			case 1:
				ImGui::SliderFloat3("position 1", lightdir, -1.0f, 1.0f);
				
				break;
			case 2:
				ImGui::SliderFloat3("direction 1", lightdir, -1.0f, 1.0f);
				ImGui::SliderFloat3("position 1", lightdir, -1.0f, 1.0f);
				break;
			}
			
			
		}
		
	}
	
	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

