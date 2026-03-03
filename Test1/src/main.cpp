
#include "Application.h"
#include "ApplicationLayer.h"

int main()
{
    Application::Get()->Init();
    Application::Get()->PushOverlayLayer<ApplicationLayer>();
    Application::Get()->Run();
    Application::Get()->CleanUp();
}
