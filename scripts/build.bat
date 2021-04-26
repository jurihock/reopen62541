@echo off

pushd "%~dp0"
cd ..
set CMAKE_SOURCE_DIR=%CD%
popd

set CMAKE_BINARY_DIR=%CMAKE_SOURCE_DIR%\build

if not exist "%CMAKE_BINARY_DIR%" (
  echo "not found..."
)

pushd "%CMAKE_BINARY_DIR%"

cmake ..
cmake --build . --config Release
cmake --install . --prefix Install

popd
