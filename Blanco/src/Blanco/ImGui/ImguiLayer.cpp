#include "BLpch.h"
#include "ImguiLayer.h"
#include "Blanco/Core/Application.h"
#include "Glad/gl.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include "backends/imgui_impl_opengl3.cpp"
#include "backends/imgui_impl_glfw.cpp"
#include "ImGuizmo.h"

namespace Blanco
{
	ImguiLayer::ImguiLayer():Layer{"ImguiLayer"}
	{
	}
	ImguiLayer::~ImguiLayer()
	{
	}

	void ImguiLayer::OnAttach()
	{
        BL_PROFILE_FUNCTION();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;

		io.Fonts->AddFontFromFileTTF("assets/fonts/OpenSans/static/OpenSans-Bold.ttf", 25.0f);
        io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/OpenSans/static/OpenSans-Regular.ttf",25.0f);

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        // When view ports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        SetDarkThemeColors();

        // Setup Platform/Renderer back ends
        ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()),true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}
	void ImguiLayer::OnDetach()
	{
        BL_PROFILE_FUNCTION();

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        ImGuizmo::BeginFrame();
	}
    void ImguiLayer::OnEvent(Event& e)
    {
        if (m_BlockEvents) {
            ImGuiIO& io = ImGui::GetIO();
            e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
            e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
        }
    }
    void ImguiLayer::OnImguiRender()
    {
    }
    void ImguiLayer::Begin()
    {
        BL_PROFILE_FUNCTION();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

    }
    void ImguiLayer::End()
    {
        BL_PROFILE_FUNCTION();

        ImGui::Render();
		/*   int display_w, display_h;
		   glfwGetFramebufferSize(static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()), &display_w, &display_h);
		   glViewport(0, 0, display_w, display_h);*/
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }
    void ImguiLayer::SetDarkThemeColors()
    {
        auto& colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f,0.105f,0.1f,1.0f };

        //Headers
		colors[ImGuiCol_Header] = ImVec4{ 0.2f,0.205f,0.21f,1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f,0.1505f,0.151f,1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f,0.305f,0.31f,1.0f };

        //Buttons
		colors[ImGuiCol_Button] = ImVec4{ 0.2f,0.205f,0.21f,1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f,0.1505f,0.151f,1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f,0.305f,0.31f,1.0f };

        //Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f,0.205f,0.21f,1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f,0.1505f,0.151f,1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f,0.305f,0.31f,1.0f };

        //Tabs
		colors[ImGuiCol_Tab] = ImVec4{ 0.15f,0.1505f,0.151f,1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f,0.2805f,0.281f,1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f,0.3805f,0.381f,1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f,0.1505f,0.151f,1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f,0.205f,0.21f,1.0f };

        //Title BG
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f,0.1505f,0.151f,1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f,0.1505f,0.151f,1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.95f,0.1505f,0.951f,1.0f };
    }
}
