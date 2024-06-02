#pragma once

namespace Blanco
{
	enum class RenderAPI {
		NONE = 0,
		OPENGL = 1
	};

	class Renderer {
	public:
		inline static void SetAPI(const RenderAPI& renderAPI) { s_RenderAPI = renderAPI; };
		inline static RenderAPI& GetAPI() { return s_RenderAPI; }
	private:
		static RenderAPI s_RenderAPI;
	};
}