#pragma once
#include "Core.h"

namespace Blanco
{
	class BL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	//to be defined in client
	Application* CreatApplication();
}

