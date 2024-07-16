include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "Blanco"
configurations 
{
	"Debug","Release","Dist"
}
solution_items
{
	".editorconfig"
}
flags
{
	"MultiProcessorCompile"
}
architecture "x64"
cppdialect "C++20"
startproject "BlancoPelota"

outputdir="%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
  include "vendor/premake"
  include "Blanco/vendor/glfw"
  include "Blanco/vendor/glad"
  include "Blanco/vendor/imgui"
  include "Blanco/vendor/yaml"
  include "Blanco/vendor/Box2D"
group ""

include "Blanco"
include "BlancoPelota"
include "Sandbox"

