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

	// Initalise scene variables.


lights[0] = new LightSource();
lights[0]->setLightType(LightSource::LType::DIRECTIONAL);
lights[0]->setAmbientColour(0.2, 0.2, 0.2, 1.0f);
lights[0]->setPosition(0.2, 0.2, 0.2);
lights[0]->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
lights[0]->setDirection(0.45f, 0.5f, 0.50f);
lights[1] = new LightSource();
lights[1]->setLightType(LightSource::LType::DIRECTIONAL);
lights[1]->setPosition(0.2, 0.2, 0.2);
lights[1]->setAmbientColour(0.2, 0.2, 0.2, 1.0f);
lights[1]->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
lights[1]->setDirection(0.45f, -0.5f, 0.75f);
lights[2] = new LightSource();
lights[2]->setLightType(LightSource::LType::DIRECTIONAL);
lights[2]->setPosition(0.2, 0.2, 0.2);
lights[2]->setAmbientColour(0.2, 0.2, 0.2, 1.0f);
lights[2]->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
lights[2]->setDirection(0.45f, -0.5f, 0.75f);
lights[3] = new LightSource();
lights[3]->setLightType(LightSource::LType::DIRECTIONAL);
lights[3]->setPosition(0.2, 0.2, 0.2);
lights[3]->setAmbientColour(0.2, 0.2, 0.2, 1.0f);
lights[3]->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
lights[3]->setDirection(0.45f, -0.5f, 0.75f);

textureMgr->loadTexture(L"water", L"res/water.png");
textureMgr->loadTexture(L"height", L"res/height.png");
water = new Water(renderer->getDevice(), renderer->getDeviceContext(), textureMgr->getTexture(L"water"), textureMgr->getTexture(L"height"));
water->setMesh(new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext()));

waterShader = new WaterShader(renderer->getDevice(), hwnd);
basicShader = new BasicShader(renderer->getDevice(), hwnd);

lightdir[0] = lights[0]->getDirection().x;
lightdir[1] = lights[0]->getDirection().y;
lightdir[2] = lights[0]->getDirection().z;
}


App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.

}


bool App1::frame()
{
	
	bool result;
	lights[0]->setDirection(lightdir[0], lightdir[1], lightdir[2]);
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

	water->render(worldMatrix, viewMatrix, projectionMatrix, basicShader, lights, deltaTime);

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

	if (ImGui::CollapsingHeader("water"))
	{
		ImGui::SliderFloat("Steepness", water->getSteepness(), 0, 1.0f);
		ImGui::SliderFloat("Wave Length ", water->getWaveLength(), 0.0f, 30.0f);
		ImGui::InputFloat("Gravity ", water->getGravity(), 0.0f, 10.8);


	}

	if(ImGui::CollapsingHeader("lights"))
	{
		ImGui::SliderFloat3("lightdir", lightdir, -1.0f, 1.0f);
	}
	
	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

