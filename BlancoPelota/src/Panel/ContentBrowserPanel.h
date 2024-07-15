#pragma once
#include <Blanco.h>
#include <filesystem>

namespace Blanco
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();
		void OnImguiRender();
	public:
		std::filesystem::path m_CurrentPath;
		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
	};

}