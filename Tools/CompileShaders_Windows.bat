@echo off
setlocal enabledelayedexpansion

set GLSLC=glslc
set ROOT_DIR=%~dp0\..
set SHADER_DIR=%ROOT_DIR%\SilverlightEngine\Assets\Shaders
set OUTPUT_DIR=%ROOT_DIR%\build\CompiledShaders
set RUNTIME_DIR=%ROOT_DIR%\build\Binaries\Shaders

echo.
echo ----Compiling shaders----

for /R "%SHADER_DIR%" %%f in (*.vert *.frag) do (
    set "SHADER=%%f"
    set "FILE=%%~nxf"
    set "EXT=%%~xf"
    set "STAGE=!EXT:.=!"
    set "SPV_OUT=%OUTPUT_DIR%\!FILE!.spv"

    REM Create output directories
    if not exist "%OUTPUT_DIR%" mkdir "%OUTPUT_DIR%"
    if not exist "%RUNTIME_DIR%" mkdir "%RUNTIME_DIR%"

    REM Compile if needed
    if not exist "!SPV_OUT!" (
        echo [NEW] Compiling !FILE!
        %GLSLC% -fshader-stage=!STAGE! "!SHADER!" -o "!SPV_OUT!"
    ) else (
        for %%A in ("!SHADER!") do set "SRC_MOD=%%~tA"
        for %%B in ("!SPV_OUT!") do set "OUT_MOD=%%~tB"

        if "!SRC_MOD!" GTR "!OUT_MOD!" (
            echo [UPDATED] Compiling !FILE!
            %GLSLC% -fshader-stage=!STAGE! "!SHADER!" -o "!SPV_OUT!"
        )
    )

    REM Copy to runtime folder
    copy /Y "!SPV_OUT!" "%RUNTIME_DIR%" >nul
)

echo ----Compiling shaders complete----
endlocal
