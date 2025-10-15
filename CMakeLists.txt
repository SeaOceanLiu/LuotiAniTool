cmake_minimum_required(VERSION 3.20)
project(LuotiAniTool)

set(CMAKE_CXX_STANDARD 17)

# 添加DebugTrace库
add_library(DebugTrace
    subModules/DebugTrace/DebugTrace.cpp
)

# 设置DebugTrace库的包含目录
target_include_directories(DebugTrace PUBLIC
    subModules/DebugTrace
)

# 链接Windows系统库
if(WIN32)
    target_link_libraries(DebugTrace PRIVATE
        user32
        kernel32
    )
endif()

# 添加UIControls库
add_subdirectory(UIControls)

# 添加可执行文件
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/LuotiAniTool.rc
)

# 包含SDL3头文件目录和其他必要的包含路径
target_include_directories(${PROJECT_NAME} PRIVATE
    src
    src/SDL3
    subModules/DebugTrace
    UIControls/include
)

# 链接DebugTrace库和UIControls库
target_link_libraries(${PROJECT_NAME} PRIVATE DebugTrace UIControls)

# 为Release模式添加NDEBUG定义
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:NDEBUG>)
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:MinSizeRel>:NDEBUG>)
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:RelWithDebInfo>:NDEBUG>)

# 配置SDL3库
set(SDL3_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/LibsAndDlls)

# 链接SDL3库
if(WIN32)
    # Windows平台
    target_link_directories(${PROJECT_NAME} PRIVATE ${SDL3_LIB_DIR})
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL3)

    # 复制SDL3.dll到输出目录
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SDL3_LIB_DIR}/SDL3.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)

    # 复制DebugInfoX64.dll到输出目录
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/subModules/DebugTrace/DebugInfoX64.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)
else()
    # 尝试查找系统中的SDL3库
    find_package(SDL3 QUIET)
    if(SDL3_FOUND)
        target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
    else()
        message(WARNING "未在系统中找到SDL3库。请确保SDL3库在运行时可访问。")
    endif()
endif()