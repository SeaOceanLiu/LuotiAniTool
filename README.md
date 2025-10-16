# LuotiAniTool
LuotiAni is an animation library runs on SDL3, similar to Lottie. It also uses JSON to define animation layers and keyframes, then automatically generates continuous animation frames based on rules during the initialization phase by loading the JSON file. LuotiAniTool is used to generate, play and test whether LuotiAni JSON meets expectations

## 快速构建

项目提供了多种构建脚本，方便快速编译：

- [build_scripts/rebuild.bat](build_scripts/rebuild.bat) - 使用CMake重新生成并编译整个项目（推荐）
- [build_scripts/build.bat](build_scripts/build.bat) - 自动检测编译环境并构建项目
- [build_scripts/build.ps1](build_scripts/build.ps1) - PowerShell版本的构建脚本，自动检测编译环境
- [build_scripts/build_with_generator.bat](build_scripts/build_with_generator.bat) - 指定生成器的构建脚本
- [build_scripts/build_with_generator.ps1](build_scripts/build_with_generator.ps1) - PowerShell版本的指定生成器构建脚本
- [build_scripts/clean.bat](build_scripts/clean.bat) - 清理所有编译中间结果和最终结果
- [build_scripts/clean.ps1](build_scripts/clean.ps1) - PowerShell版本的清理脚本

## DebugTrace 子模块

项目依赖 DebugTrace 库用于调试信息输出。可以使用以下脚本之一来下载或更新 DebugTrace 子模块：

- [subModules/download_DebugTrace.bat](subModules/download_DebugTrace.bat) - Windows批处理版本
- [subModules/download_DebugTrace.ps1](subModules/download_DebugTrace.ps1) - PowerShell版本

## SDL3 可变大小窗体示例

这个项目演示了如何创建一个基于SDL3的可变大小窗体应用程序。

### 功能特性

- 创建可调整大小的SDL3窗口
- 实时响应窗口大小变化
- 在窗口中心绘制一个随窗口大小变化的矩形
- 支持ESC键退出程序

### 文件说明

- [src/main.cpp](src/main.cpp) - 主程序文件，包含窗口创建和事件处理逻辑
- [src/SDL3/](src/SDL3/) - SDL3头文件目录
- [CMakeLists.txt](CMakeLists.txt) - CMake构建配置文件
- [BUILD_INSTRUCTIONS.md](BUILD_INSTRUCTIONS.md) - 构建和运行说明

### 编译和运行

请参考 [BUILD_INSTRUCTIONS.md](BUILD_INSTRUCTIONS.md) 文件获取详细的构建和运行说明。

### 使用说明

- 拖拽窗口边缘可以调整窗口大小
- 程序会自动适应新的窗口尺寸并在控制台显示
- 按ESC键或点击窗口关闭按钮退出程序

### 故障排除

1. **找不到SDL库**：
   - 确保SDL3运行时库已正确安装
   - 检查SDL3动态链接库(.dll/.so)是否在正确位置

2. **运行时错误**：
   - 确保SDL3库的动态链接库在系统路径中或与可执行文件在同一目录

### 构建选项

项目支持多种构建方式：

1. **自动检测环境**：
   ```
   build_scripts\build.bat
   build_scripts\build.ps1
   ```

2. **指定生成器**：
   ```
   build_scripts\build_with_generator.bat vs2022 Debug
   build_scripts\build_with_generator.bat mingw Release
   ```

3. **清理构建产物**：
   ```
   build_scripts\clean.bat
   build_scripts\clean.ps1
   ```

4. **手动使用CMake**：
   ```
   mkdir build
   cd build
   # 使用Visual Studio生成器
   cmake .. -G "Visual Studio 17 2022"
   # 或使用MinGW生成器
   cmake .. -G "MinGW Makefiles"
   ```