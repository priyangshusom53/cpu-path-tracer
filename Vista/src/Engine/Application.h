#pragma once

#include "Dependencies.h"
#include "Window.h"
#include "LayerStack.h"
#include "Engine/Renderer/Renderer.h"
#include <functional>
#include"Scene/GLTF_Import.h"
//#include "Engine/Scene/Camera.h"



namespace vista 
{
	class Application
	{
	public:
		Application();

		virtual ~Application();

		void onEvent(Event& e);

		void run();

		void pushLayer(Layer* layer);
		
		void pushOverlay(Layer* layer);

	private:
		Window* m_Window;
		bool isRunning;
		bool onWindowClose(WindowCloseEvent& event);
		bool onMouseMoved(MouseMovedEvent& event);

		LayerStack layerStack;
		//Renderer* renderer;
		GLTF_Importer* importer;
		//Camera* cam;
		
	};
	
}

