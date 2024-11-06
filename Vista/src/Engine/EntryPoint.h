#pragma once




#ifdef VSTA_PLATFORM_WINDOWS


extern vista::Application* vista::createApplication();



int main(int argc, char** argv) 
{
	auto app = vista::createApplication();
	
	app->run();
	delete app;
}
#endif