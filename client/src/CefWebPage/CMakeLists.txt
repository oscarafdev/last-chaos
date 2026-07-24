cmake_minimum_required(VERSION 3.21)

set(CMAKE_CONFIGURATION_TYPES Debug Release)
project(LastChaosCefWebPage LANGUAGES C CXX)

set(CEF_ROOT "" CACHE PATH "Path to the extracted CEF binary distribution")
if(NOT EXISTS "${CEF_ROOT}/cmake/FindCEF.cmake")
  message(FATAL_ERROR "CEF_ROOT does not point to a CEF binary distribution")
endif()

list(APPEND CMAKE_MODULE_PATH "${CEF_ROOT}/cmake")
find_package(CEF REQUIRED)

add_subdirectory("${CEF_LIBCEF_DLL_WRAPPER_PATH}" libcef_dll_wrapper)
ADD_LOGICAL_TARGET(libcef_lib "${CEF_LIB_DEBUG}" "${CEF_LIB_RELEASE}")

add_library(CWebPage SHARED
  src/CefApplication.h
  src/CefBrowserClient.cpp
  src/CefBrowserClient.h
  src/CefRuntime.cpp
  src/CefRuntime.h
  src/CWebPageExports.cpp
  src/CWebPageExports.def
)
SET_LIBRARY_TARGET_PROPERTIES(CWebPage)
target_link_libraries(CWebPage PRIVATE libcef_lib libcef_dll_wrapper ${CEF_STANDARD_LIBS})
set_target_properties(CWebPage PROPERTIES
  OUTPUT_NAME "CWebPage"
  LINK_FLAGS "/DEF:\"${CMAKE_CURRENT_SOURCE_DIR}/src/CWebPageExports.def\""
)

add_executable(CefSubprocess WIN32
  src/CefApplication.h
  src/CefSubprocess.cpp
)
SET_EXECUTABLE_TARGET_PROPERTIES(CefSubprocess)
target_link_libraries(CefSubprocess PRIVATE libcef_lib libcef_dll_wrapper ${CEF_STANDARD_LIBS})

PRINT_CEF_CONFIG()
