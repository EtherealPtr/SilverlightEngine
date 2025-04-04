@echo off
setlocal

echo ---Setting up Silverlight---

echo Checking for Vulkan SDK
if not defined VULKAN_SDK (
    echo ERROR: Vulkan SDK not found. Please install it from https://vulkan.lunarg.com/sdk/home
    pause
    exit /b 1
)

echo Vulkan SDK found!

if not exist build (
    mkdir build
)

cd build

echo Generating Visual Studio Solution, please wait
cmake .. -G "Visual Studio 17 2022" -A x64

echo Starting Silverlight!
start SilverlightEngine.sln
pause
