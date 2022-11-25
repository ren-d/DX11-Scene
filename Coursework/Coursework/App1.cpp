// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

App1::App1()
{

}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	// Initalise scene variables.
	
	
	sun = new Light();
	sun->setAmbientColour(0.2, 0.2, 0.2, 1.0f);
	sun->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	sun->setDirection(0.45f, -0.5f, 0.75f);

	textureMgr->loadTexture(L"brick", L"res/brick1.dds");
	textureMgr->loadTexture(L"height", L"res/height.png");
	textureMgr->loadTexture(L"cottage", L"res/models/cottage_textures/cottage_diffuse.png");
	textureMgr->loadTexture(L"cottageNormal", L"res/models/cottage_textures/cottage_normal.png");
	heightMapObj = new HeightMappedObject(renderer->getDevice(), renderer->getDeviceContext(), textureMgr->getTexture(L"brick"), textureMgr->getTexture(L"height"));
	heightMapObj->setMesh(new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext()));
	heightShader = new HeightMapShader(renderer->getDevice(), hwnd);

	obj = new SceneObject(renderer->getDevice(), renderer->getDeviceContext(), textureMgr->getTexture(L"brick"));
	obj->setMesh(new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext()));
	basicShader = new BasicShader(renderer->getDevice(), hwnd);
	modelShader = new ModelShader(renderer->getDevice(), hwnd);
	houseModel = new AModel(renderer->getDevice(), "res/models/cottage_fbx.fbx");
	house = new ModelObject(renderer->getDevice(), renderer->getDeviceContext(), textureMgr->getTexture(L"cottage"), textureMgr->getTexture(L"cottageNormal"));
	house->setModel(houseModel);

	lightdir[0] = sun->getDirection().x;
	lightdir[1] = sun->getDirection().y;
	lightdir[2] = sun->getDirection().z;
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
	sun->setDirection(lightdir[0], lightdir[1], lightdir[2]);
	result = BaseApplication::frame();
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

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();
	
	obj->translate(XMFLOAT3(0.0f, 10.0f, 0.0f));

	heightMapObj->render(worldMatrix, viewMatrix, projectionMatrix, heightShader, sun);
	house->render(worldMatrix, viewMatrix, projectionMatrix, modelShader, sun);
	// Render GUI
	gui();

	// Present the rendered scene to the screen.
	renderer->endScene();

	return true;
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
	ImGui::SliderFloat("Amplitude ", heightMapObj->getAmplitude(), -50.0f, 50.0f);
	ImGui::SliderFloat3("lightdir", lightdir, -1.0f, 1.0f);
	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

