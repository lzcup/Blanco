#include "ContentBrowserPanel.h"
#include <imgui.h>

namespace Blanco
{
	static std::filesystem::path AssetPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel():m_CurrentPath(AssetPath)
	{
		m_DirectoryIcon = Texture2D::Create("Resource/Icons/folder_open_icon.png");
		m_FileIcon = Texture2D::Create("Resource/Icons/file_graph_icon.png");
	}

	void ContentBrowserPanel::OnImguiRender()
	{
		ImGui::Begin("Content Browser");
		if (m_CurrentPath != AssetPath)
		{
			if (ImGui::Button("<-"))
				m_CurrentPath = m_CurrentPath.parent_path();
		}

		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (auto& dirEntry : std::filesystem::directory_iterator(m_CurrentPath))
		{
			std::string filename = dirEntry.path().filename().string();

			ImGui::PushID(filename.c_str());
			Ref<Texture2D> icon = dirEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

			if (ImGui::BeginDragDropSource())
			{
				std::filesystem::path relativePath(dirEntry.path());
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (dirEntry.is_directory())
				{
					m_CurrentPath /= dirEntry.path().filename();
				}
			}
			ImGui::TextWrapped(filename.c_str());

			ImGui::NextColumn();

			ImGui::PopID();
		}
		ImGui::Columns(1);

		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
		ImGui::SliderFloat("Padding", &padding, 0, 32);

		ImGui::End();
	}
}