if (MSVC)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(SDL2_LIB_PATH_SUFFIX lib/x64)
    else()
        set(SDL2_LIB_PATH_SUFFIX lib/x86)
    endif()

    set(SDL2_DIR "SDL2-devel-2.0.14-VC")
    set(SDL2_INCLUDE_DIRS "${SDL2_DIR}/include")
    set(SDL2_LIBRARIES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${SDL2_DIR}/${SDL2_LIB_PATH_SUFFIX}")
    set(SDL2_LIBRARY "${SDL2_LIBRARIES_DIR}/SDL2.lib")
    set(SDL2_MAIN_LIBRARY "${SDL2_LIBRARIES_DIR}/SDL2main.lib")
    set(SDL2_LIBRARIES ${SDL2_LIBRARY} ${SDL2_MAIN_LIBRARY})
else ()
    find_package(SDL2 REQUIRED)
endif ()

add_library(SDL2 INTERFACE)
target_include_directories(SDL2 INTERFACE ${SDL2_INCLUDE_DIRS})
target_link_libraries(SDL2 INTERFACE ${SDL2_LIBRARIES})
