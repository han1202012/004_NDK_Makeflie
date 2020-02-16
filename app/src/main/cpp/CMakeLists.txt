
# 指定 CMake 最低版本
cmake_minimum_required(VERSION 3.4.1)

# 设置函数库编译
add_library( # 参数 1 : 设置生成的动态库名称
        native-lib

        # 参数 2 : 设置生成的函数库类型 : ① 静态库 STATIC ② 动态库 SHARED
        SHARED

        # 参数 3 : 配置要编译的源文件
        native-lib.cpp)


# 引入静态库
#       ① 参数 1 ( add ) : 设置引入的静态库名称
#       ② 参数 2 ( SHARED ) : 设置引入的函数库类型 : ① 静态库 STATIC ② 动态库 SHARED
#       ③ 参数 3 ( IMPORTED ) : 表示引入第三方静态库 , 导入静态库 , 相当于预编译静态库
#                                   后续还需要设置导入路径 , 配合该配置使用
add_library(
        # 设置引入的静态库名称
        add

        # 设置引入的函数库类型为静态库
        STATIC

        # 表示引入第三方静态库
        IMPORTED)

# 设置上述静态库的导入路径
#       设置目标属性参数 :
#           ① 参数 1 ( add ) : 要设置哪个函数库的属性
#           ② 参数 2 ( PROPERTIES ) : 设置目标属性
#           ③ 参数 3 ( IMPORTED_LOCATION ) : 设置导入路径
#           ④ 参数 4 : 配置静态库的文件路径
set_target_properties(
        # 设置目标
        add

        # 设置属性
        PROPERTIES

        # 导入路径
        IMPORTED_LOCATION

        # ${CMAKE_SOURCE_DIR} 是本 CMakeList.txt 构建脚本的路径 , 是 CMake 工具内置的变量
        #       Android CMake 也内置了一些变量 , 如 ANDROID_ABI
        ${CMAKE_SOURCE_DIR}/../jniLibs/armeabi-v7a/libadd.a)

# 打印日志信息
#       ${ANDROID_ABI} 的作用是获取当前的 CPU 指令集架构
#           当本次编译 armeabi-v7a CPU 架构时 , ${ANDROID_ABI} 值为 armeabi-v7a
#           当本次编译 x86 CPU 架构时 , ${ANDROID_ABI} 值为 x86
message("CMAKE_SOURCE_DIR : ${CMAKE_SOURCE_DIR}, ANDROID_ABI : ${ANDROID_ABI}")


# 到预设的目录查找 log 库 , 将找到的路径赋值给 log-lib
#   这个路径是 NDK 的 ndk-bundle\platforms\android-29\arch-arm\usr\lib\liblog.so
#   不同的 Android 版本号 和 CPU 架构 需要到对应的目录中查找 , 此处是 29 版本 32 位 ARM 架构的日志库
find_library(
        log-lib

        log)


# 设置变量
# CMAKE_CXX_FLAGS 表示会将 C++ 的参数传给编译器
# CMAKE_C_FLAGS 表示会将 C 参数传给编译器

# 参数设置 : 传递 CMAKE_CXX_FLAGS C+= 参数给编译器时 , 在 该参数后面指定库的路径
#   CMAKE_SOURCE_DIR 指的是当前的文件地址
#   -L 参数指定动态库的查找路径
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/../jniLibs/armeabi-v7a")

# 链接函数库
#       参数 1 : 本构建脚本要生成的动态库目标
#       参数 2 ~ ... : 后面是之前预编译的动态库或静态库 , 或引入的动态库
target_link_libraries(
        native-lib

        # 表示 编译 native-lib 模块, 要链接 add 模块
        add

        ${log-lib})