# UIControls库的CMake配置文件

# 添加UIControls库
add_library(UIControls
    src/Actor.cpp
    src/Bench.cpp
    src/Button.cpp
    src/ConstDef.cpp
    src/ControlBase.cpp
    src/Dialog.cpp
    src/EventQueue.cpp
    src/Label.cpp
    src/Material.cpp
    src/Panel.cpp
    src/ResourceLoader.cpp
    src/Menu.cpp
)

# 设置UIControls库的包含目录
target_include_directories(UIControls PUBLIC
    include
    ../src
    ../src/SDL3
    ../src/SDL3_ttf
    ../subModules/DebugTrace
)

# 设置C++标准
target_compile_features(UIControls PUBLIC cxx_std_17)

# 如果需要链接其他库，可以在这里添加
# target_link_libraries(UIControls PRIVATE SomeOtherLibrary)
