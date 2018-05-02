@echo off
pushd "%~dp0"
set GENERATOR=Visual Studio 12
if not defined CONFIG (
    set CONFIG=Release
)

rem Load MSVC12
if not defined VSINSTALLDIR (
    call "%VS120COMNTOOLS%\vsvars32.bat"
)

rem Generate and build (x86)
mkdir build\x86
pushd build\x86
cmake -G"%GENERATOR%" "%~dp0" || goto ERROR
msbuild pinvoke.sln /p:Configuration=%CONFIG% /m || goto ERROR
popd

rem Generate and build (x64)
mkdir build\x64
pushd build\x64
cmake -G"%GENERATOR% Win64" "%~dp0" || goto ERROR
msbuild pinvoke.sln /p:Configuration=%CONFIG% /m || goto ERROR
popd

rem Build .NET assemblies
msbuild Uno.Native.sln /p:Configuration=%CONFIG% || goto ERROR

rem Done
popd && exit /b 0

:ERROR
echo BUILD FAILED.
popd && pause && exit /b 1
