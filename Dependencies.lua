-- Blanco Dependencies

VULKAN_SDK = os.getenv("VULKAN_SDK")

--include directories
IncludeDir = {}
IncludeDir["spdlog"] = "%{wks.location}/Blanco/vendor/spdlog/include"
IncludeDir["GLFW"] = "%{wks.location}/Blanco/vendor/glfw/include"
IncludeDir["Glad"] = "%{wks.location}/Blanco/vendor/glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Blanco/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/Blanco/vendor/glm/glm"
IncludeDir["stb_image"] = "%{wks.location}/Blanco/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/Blanco/vendor/entt/include"
IncludeDir["Yaml"] = "%{wks.location}/Blanco/vendor/yaml/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/Blanco/vendor/ImGuizmo"
IncludeDir["shaderc"] = "%{wks.location}/Blanco/vendor/shaderc/include"
IncludeDir["SPIRV_Cross"] = "%{wks.location}/Blanco/vendor/SPIRV-Cross"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"

LibraryDir = {}

LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
LibraryDir["VulkanSDK_Debug"] = "%{wks.location}/Blanco/vendor/VulkanSDK/Lib"

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"