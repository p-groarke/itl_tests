include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.fea_cmake.txt)
cmake_minimum_required (VERSION 3.20)
project(itl_tests VERSION 1.0.0 LANGUAGES CXX)

include(GenerateExportHeader)

fea_cmake_download(89ce8c2fcfc0247da8c27849552aea95ebcbaf05)
fea_import_fea_cmake_download()
fea_import_clangformat()
fea_import_gitignore()
fea_set_output_dir(${CMAKE_BINARY_DIR}/bin)

set(FEA_TESTS Off CACHE INTERNAL "")
set(FEA_CPP17 On CACHE INTERNAL "")
fea_fetch_content(fea_libs
	https://github.com/p-groarke/fea_libs.git
	71cbd4bb6d1e7b56ee37bf01c77ae6cf1df82c60
)

fea_pull_conan()
find_package(GTest CONFIG REQUIRED QUIET)
find_package(TBB CONFIG REQUIRED QUIET)
find_package(glm CONFIG REQUIRED QUIET)

# Internal shared lib
set(SHARED_LIB_NAME "shrd_lib")
file(GLOB_RECURSE SHARED_INCLUDE_FILES "shared_include/*.hpp" "shared_include/*.h")
file(GLOB_RECURSE SHARED_SOURCE_FILES "shared_src/*.hpp" "shared_src/*.h" "shared_src/*.cpp" "shared_src/*.c")
add_library(${SHARED_LIB_NAME} SHARED ${SHARED_INCLUDE_FILES} ${SHARED_SOURCE_FILES})
generate_export_header(${SHARED_LIB_NAME} EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/exports/shrd_lib_export.h")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/shared_include PREFIX "Header Files" FILES ${SHARED_INCLUDE_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/shared_src PREFIX "Source Files" FILES ${SHARED_SOURCE_FILES})

fea_set_compile_options(${SHARED_LIB_NAME} PUBLIC)
fea_release_profiling(${SHARED_LIB_NAME})
target_compile_definitions(${SHARED_LIB_NAME} PRIVATE ${SHARED_LIB_NAME}_EXPORTS)
target_link_libraries(${SHARED_LIB_NAME} PRIVATE TBB::tbb TBB::tbbmalloc fea_libs glm::glm)

target_include_directories(${SHARED_LIB_NAME} PUBLIC
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/shared_include>
)
target_include_directories(${SHARED_LIB_NAME} PUBLIC
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/exports>
)


# Main Project
file(GLOB_RECURSE INCLUDE_FILES "include/*.hpp" "include/*.h")
file(GLOB_RECURSE SOURCE_FILES "src/*.hpp" "src/*.h" "src/*.cpp" "src/*.c")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include PREFIX "Header Files" FILES ${INCLUDE_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "Source Files" FILES ${SOURCE_FILES})

add_executable(${PROJECT_NAME} ${INCLUDE_FILES} ${SOURCE_FILES})

fea_set_compile_options(${PROJECT_NAME} PUBLIC)
fea_release_profiling(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} GTest::GTest TBB::tbb TBB::tbbmalloc fea_libs glm::glm ${SHARED_LIB_NAME})


# Interface
target_include_directories(${PROJECT_NAME} PUBLIC
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_include_directories(${PROJECT_NAME} PRIVATE src) # For based paths.

# Make your project the "startup" project in Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})