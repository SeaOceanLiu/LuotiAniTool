# 构建和运行说明

## 项目结构

本项目包含一个基于SDL3的可变大小窗体示例，使用项目中自带的SDL3头文件。

## 快速构建脚本

为了简化构建过程，项目提供了几个预定义的构建脚本：

1. [build_scripts/build.bat](build_scripts/build.bat) - 自动检测编译环境并构建项目
2. [build_scripts/build.ps1](build_scripts/build.ps1) - PowerShell版本的构建脚本，自动检测编译环境
3. [build_scripts/build_with_generator.bat](build_scripts/build_with_generator.bat) - 指定生成器的构建脚本
4. [build_scripts/build_with_generator.ps1](build_scripts/build_with_generator.ps1) - PowerShell版本的指定生成器构建脚本
5. [build_scripts/rebuild.bat](build_scripts/rebuild.bat) - 清理并重新构建整个项目

只需双击相应的脚本文件即可开始构建过程。

## 使用Visual Studio构建（推荐）

本项目已配置为使用Visual Studio进行构建，SDL3库文件位于[LibsAndDlls](LibsAndDlls/)目录中。

### 方法1：使用智能构建脚本（推荐）

```bash
# 自动检测Visual Studio环境
build_scripts\build.bat

# 或使用PowerShell版本
build_scripts\build.ps1
```

### 方法2：指定生成器构建

```bash
# 使用Visual Studio 2022生成器
build_scripts\build_with_generator.bat vs2022

# 使用Visual Studio 2019生成器
build_scripts\build_with_generator.bat vs2019

# 使用Visual Studio 2017生成器
build_scripts\build_with_generator.bat vs2017
```

### 方法3：手动使用CMake

```bash
# 创建构建目录
mkdir build
cd build

# 使用Visual Studio生成器
cmake .. -G "Visual Studio 17 2022"
# 或对于Visual Studio 2019
# cmake .. -G "Visual Studio 16 2019"
# 或对于Visual Studio 2017
# cmake .. -G "Visual Studio 15 2017"

# 构建项目
cmake --build . --config Debug
# 或构建Release版本
# cmake --build . --config Release
```

### 方法4：在Visual Studio中直接打开

1. 打开Visual Studio
2. 选择"打开本地文件夹"，选择项目根目录
3. Visual Studio会自动使用CMake配置项目
4. 点击"生成" → "全部生成"

## 使用命令行构建

### Windows (使用MinGW)

1. 确保已安装MinGW和CMake
2. 打开PowerShell或命令提示符
3. 进入项目根目录
4. 创建构建目录并进入：
   ```bash
   mkdir build
   cd build
   ```
5. 配置项目：
   ```bash
   cmake .. -G "MinGW Makefiles"
   ```
6. 编译项目：
   ```bash
   cmake --build .
   ```

### 使用指定生成器脚本构建MinGW版本

```bash
build_scripts\build_with_generator.bat mingw
```

### Linux/macOS

1. 确保已安装GCC/Clang和CMake
2. 打开终端
3. 进入项目根目录
4. 创建构建目录并进入：
   ```bash
   mkdir build
   cd build
   ```
5. 配置项目：
   ```bash
   cmake ..
   ```
6. 编译项目：
   ```bash
   make
   ```

## 运行程序

编译完成后，可执行文件将位于build/Debug或build/Release目录中：

### Windows

```bash
# Debug版本
Debug\LuotiAniTool.exe
# Release版本
Release\LuotiAniTool.exe
```

### Linux/macOS

```bash
./LuotiAniTool
```

## 注意事项

1. 本项目使用项目中自带的SDL3头文件
2. SDL3库文件位于[LibsAndDlls](LibsAndDlls/)目录中，包含：

   - SDL3.dll - SDL3运行时库
   - SDL3.lib - SDL3导入库
   - 其他SDL3扩展库（image, ttf等）
3. 运行程序时，需要确保SDL3.dll在以下位置之一：

   - 可执行文件同一目录（已通过CMake自动配置）
   - 系统PATH环境变量中的目录
   - Windows系统目录（如C:\Windows\System32）

## Visual Studio配置说明

1. 包含目录配置：

   - 项目属性 → C/C++ → 常规 → 附加包含目录
   - 添加：$(ProjectDir)..\src
2. 库目录配置：

   - 项目属性 → 链接器 → 常规 → 附加库目录
   - 添加：$(ProjectDir)..\LibsAndDlls
3. 依赖项配置：

   - 项目属性 → 链接器 → 输入 → 附加依赖项
   - 添加：SDL3.lib
4. 运行时库配置：

   - CMake已自动配置将SDL3.dll复制到输出目录
   - 无需手动操作

## 功能说明

程序实现了一个可变大小的SDL3窗口，具有以下功能：

- 创建可调整大小的SDL3窗口
- 实时响应窗口大小变化事件
- 在窗口中心绘制一个随窗口大小变化的矩形
- 支持ESC键退出程序
- 控制台输出窗口尺寸变化信息
