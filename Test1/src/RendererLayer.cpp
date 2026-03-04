#include "RendererLayer.h"
#include "Application.h"
#include "GameLogicLayer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>


#define ANIMATION_FPS 12

static glm::vec3 color{ 1.f };

void RendererLayer::OnUpdate(float deltaTime)
{
	Trinacria::DSL::Renderer::ClearColorBuffer();

	GLFWwindow* window = Application::Get()->GetWindow();

	const float speed = 3.f;


	// Grid

	const float size = 1.f / 2;
	int i = 0;

	for (int x = 0; x < 11; x++)
	{
		for (int y = 0; y < 13; y++)
		{
			Trinacria::DSL::Texture* tex = i % 2 == 0 ? &_grey : &_purple;

			Trinacria::DSL::Renderer::CreateQuad(glm::vec2(x * size, y * size), tex);
			i++;
		}
	}


	// Grass

	Trinacria::DSL::Renderer::CreateQuad(glm::vec2(-1.5f), Trinacria::DSL::Texture::NO_TEXTURE, 1.f, glm::vec3(0.1f, 0.6f, 0.1f));

	// Tree

	Trinacria::DSL::Renderer::CreateQuad(glm::vec2(-1.f), &_tree);

	// Danger Zone

	Trinacria::DSL::Renderer::CreateQuad(glm::vec2(3.f), Trinacria::DSL::Texture::NO_TEXTURE, 6.f, glm::vec3(0.8f, 0.2f, 0.3f));

	// Player

	_playerAnimation.Play(deltaTime, _playerPos, color);

	Trinacria::DSL::Renderer::EndScene();

	float zoom = _cameraData.Zoom;

	glm::mat4 viewProjection = glm::scale(glm::mat4(1.f), glm::vec3(zoom, zoom, 1.f));
	
	glm::vec2 widowDimension = Application::Get()->GetWindowDimensions();

	viewProjection = glm::scale(viewProjection, glm::vec3(widowDimension.y / widowDimension.x, 1.f, 1.f));
	viewProjection = glm::translate(viewProjection, glm::vec3(-_cameraData.CameraPos.x, -_cameraData.CameraPos.y, 0.f));

	Trinacria::DSL::Renderer::ShaderProgram.Bind();
	Trinacria::DSL::Renderer::ShaderProgram.SetUniformMat4("u_View", viewProjection);

	Trinacria::DSL::Renderer::Draw();
	Trinacria::DSL::Renderer::FlushBuffers();
}

void RendererLayer::OnAttach()
{
	_grey.LoadTexture("assets/textures/grey.jpg");
	_purple.LoadTexture("assets/textures/purple.jpg");

	_playerAnimationSpriteSheet.LoadTexture("assets/textures/player.png", GL_NEAREST);

	_playerAnimationSpriteSheet.CellWidth = (float) _playerAnimationSpriteSheet.GetWidth() / 3.f;
	_playerAnimationSpriteSheet.CellHeight= (float) _playerAnimationSpriteSheet.GetHeight() / 3.f;

	_tinyVillage.LoadTexture("assets/textures/tilemap.png", GL_NEAREST);
	
	_tinyVillage.CellWidth = (float) _tinyVillage.GetWidth() / 12.f;
	_tinyVillage.CellHeight = (float) _tinyVillage.GetHeight() / 11.f;

	_playerAnimation.Init(_animation, 3, 12);

	GameLogicLayer* layer = Application::Get()->GetLayer<GameLogicLayer>();

	if (layer)
	{
		layer->DangerZoneEnteredDispatcher.RegistCallback(RendererLayer::OnEnteredDangerZone);

		layer->DangerZoneExitedDispatcher.RegistCallback([](const DangerZoneExitedEvent& event) {
			color = event.GetColorToRestore();
			});
	}
	
	Trinacria::DSL::Renderer::Init();
	Trinacria::DSL::Renderer::ShaderProgram.LoadShader("assets/shaders/Quad.vert", "assets/shaders/Quad.frag");
}

void RendererLayer::OnDetach()
{
	Trinacria::DSL::Renderer::CleanUp();
}

void RendererLayer::SetCameraData(const CameraData& cameraData)
{
	_cameraData = cameraData;
}

void RendererLayer::SetPlayerPos(glm::vec2 playerPos)
{
	_playerPos = playerPos;
}

void RendererLayer::OnEnteredDangerZone(const DangerZoneEnteredEvent& event)
{
	color = glm::vec3(event.GetDamageToApply(), 0.2f, 0.2f);
}

// Note: for matrix multiplication see this:
//	https://stackoverflow.com/questions/34303483/rotating-scaling-and-translating-2d-points-in-glm
