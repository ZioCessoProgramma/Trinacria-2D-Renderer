#include "ApplicationLayer.h"
#include "Application.h"
#include "RendererLayer.h"
#include "GameLogicLayer.h"
#include <Trinacria/InputEvents.h>

void ApplicationLayer::OnAttach()
{
	// Will be destroyed by application decostructor
	Trinacria::InputPollerLayer* layer = new Trinacria::InputPollerLayer;
	layer->SetWindow(Application::Get()->GetWindow());

	Application::Get()->PushOverlayLayer(layer);

	Application::Get()->PushLayer<GameLogicLayer>();
	Application::Get()->PushLayer<RendererLayer>();
}
