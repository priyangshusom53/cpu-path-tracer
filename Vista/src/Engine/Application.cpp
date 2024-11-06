#include"Application.h"


namespace vista 
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application() 
	{
		Log::initLog();


		isRunning = true;
		m_Window = new Window();
		m_Window->initWindow(1280, 720, "app1");
		InitGlad::initGlad();

		m_Window->setEventCallback(BIND_EVENT_FN(onEvent));

		//renderer = new Renderer();
		//renderer->initRenderer();

		//pushLayer(renderer);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
		CORE_LOG_INFO("----------MATERIALS OF SPONZA-----------");
		std::string path = "D:\\opengl\\LearnOpenGL\\resources\\objects\\cyborg\\cyborg.obj";
		importer = new GLTF_Importer();
		importer->importScene(path);
		CORE_LOG_INFO("----------MATERIALS OF SPONZA-----------");

		//cam = new Camera();
	
	}

	void Application::pushLayer(Layer* layer)
	{
		layerStack.pushLayer(layer);
	}

	void Application::pushOverlay(Layer* layer)
	{
		layerStack.pushOverlay(layer);
	}

	void Application::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowClose));
		dispatcher.dispatch<MouseMovedEvent>(BIND_EVENT_FN(onMouseMoved));

		for (auto it = layerStack.end(); it != layerStack.begin(); )
		{
			(*--it)->onEvent(e);
			if (e.handled)
				break;
		}

	}

	void Application::run() 
	{
		while (isRunning)
		{

			//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_CULL_FACE);
			glFrontFace(GL_CCW);
			glCullFace(GL_BACK);
			//glDisable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);
			importer->draw(m_Window->getGLFWwindowPointer());
			//renderer->render(m_Window->getGLFWwindowPointer());
			
			for (Layer* layer : layerStack)
				layer->onUpdate();
			
			//
			m_Window->update();
			
		}

	}

	bool Application::onWindowClose(WindowCloseEvent& event)
	{
		isRunning = false;
		
		return true;
	}

	bool Application::onMouseMoved(MouseMovedEvent& event)
	{
		/*using Scene = GLTF_Importer::Scene;
		Scene* scene = importer->m_Scene;*/

		if (importer->m_Scene->cam->firstMouse)
		{
			importer->m_Scene->cam->lastX = event.getXPos();
			importer->m_Scene->cam->lastY = event.getYPos();
			importer->m_Scene->cam->firstMouse = false;
		}
		float xoffset = event.getXPos() - importer->m_Scene->cam->lastX;
		float yoffset = importer->m_Scene->cam->lastY - event.getYPos();
		importer->m_Scene->cam->lastX = event.getXPos();
		importer->m_Scene->cam->lastY = event.getYPos();
		float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;
		importer->m_Scene->cam->yaw += xoffset;
		importer->m_Scene->cam->pitch += yoffset;
		if (importer->m_Scene->cam->pitch > 89.0f)
			importer->m_Scene->cam->pitch = 89.0f;
		if (importer->m_Scene->cam->pitch < -89.0f)
			importer->m_Scene->cam->pitch = -89.0f;
		glm::vec3 direction;
		direction.x = cos(glm::radians(importer->m_Scene->cam->yaw)) * cos(glm::radians(importer->m_Scene->cam->pitch));
		direction.y = sin(glm::radians(importer->m_Scene->cam->pitch));
		direction.z = sin(glm::radians(importer->m_Scene->cam->yaw)) * cos(glm::radians(importer->m_Scene->cam->pitch));
		importer->m_Scene->cam->cameraFront = glm::normalize(direction);

		return true;
	}



	Application::~Application()
	{
		m_Window->destroyWindow();
		delete m_Window;
	}
}