#include "ExampleScene.h"
#include "ExampleDirectionalLight.h"
#include "ExamplePointLight.h"
#include <array>
#include <Silverlight.h>

namespace ExampleApp
{
	class ExampleClientApp : public Silverlight::Application
	{
	public:
		ExampleClientApp() :
			m_DirectionalLight{ glm::vec3(-0.3f, -1.0f, -0.5f), glm::vec4(1.0f, 0.95f, 0.9f, 0.5f) },
			m_Ground{ glm::vec3(0.0f), glm::vec3(1.0f), "Textures/ground.jpg" },
			m_CustomModel{ "Models/monkey.glb", glm::vec3(0.0f, 1.0f, 0.0f) },
			m_PointLights{ { { glm::vec3(-2.0f, 0.5f, -1.2f), glm::vec4(1.0f, 0.2f, 0.2f, 3.0f) },		// Red
							 { glm::vec3(2.0f, 1.0f, 1.2f), glm::vec4(0.2f, 1.0f, 0.2f, 3.0f) },		// Green
							 { glm::vec3(-2.0f, 0.5f, 1.2f), glm::vec4(0.2f, 0.2f, 1.0f, 3.0f) } } }	// Blue
		{
			m_CustomModel.SetTintColor(glm::vec4(1.0f, 0.8f, 0.3f, 1.0f));
		}

		ExampleClientApp(const ExampleClientApp&) = delete;
		ExampleClientApp& operator=(const ExampleClientApp&) = delete;
		ExampleClientApp(ExampleClientApp&&) = delete;
		ExampleClientApp& operator=(ExampleClientApp&&) = delete;

	private:
		ExampleDirectionalLight m_DirectionalLight;
		ExampleQuad m_Ground;
		ExampleCustomModel m_CustomModel;
		std::array<ExamplePointLight, 3> m_PointLights;
	};
} 

std::unique_ptr<Silverlight::Application> CreateApplication()
{
	return std::make_unique<ExampleApp::ExampleClientApp>();
}