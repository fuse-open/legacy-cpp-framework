@echo off
pushd "%~dp0"
set GENERATOR=Visual Studio 15 2017

rem Generate and build (x64)
mkdir build\x64
pushd build\x64
cmake -G"%GENERATOR% Win64" "%~dp0" || goto ERROR
cmake --build . --use-stderr -- /p:Configuration=Debug /m || goto ERROR
cmake --build . --use-stderr -- /p:Configuration=Release /m || goto ERROR
popd

rem Done
popd && exit /b 0

:ERROR
echo BUILD FAILED.
popd && pause && exit /b 1
