@echo off

pushd "%~dp0"
cd ..
set CMAKE_SOURCE_DIR=%CD%
popd

set CMAKE_BINARY_DIR=%CMAKE_SOURCE_DIR%\build

if not exist "%CMAKE_BINARY_DIR%" (
  mkdir "%CMAKE_BINARY_DIR%"
)

uv venv --allow-existing --directory "%CMAKE_SOURCE_DIR%"
call "%CMAKE_SOURCE_DIR%\.venv\Scripts\activate"

cmake -S "%CMAKE_SOURCE_DIR%" -B "%CMAKE_BINARY_DIR%" --fresh
cmake --build "%CMAKE_BINARY_DIR%" --config Release
cmake --install "%CMAKE_BINARY_DIR%" --prefix install
