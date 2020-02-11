@[TOC]


**CSDN 博客地址 :** [【Android NDK 开发】NDK 交叉编译 ( Ubuntu 中交叉编译动态库 | Android Studio 中配置使用第三方动态库 )](https://hanshuliang.blog.csdn.net/article/details/104255990)



<br>
<br>

#### I . 动态库 与  静态库

---

<br>

**1 . 函数库分类 :** <font color=blue>① 动态库 , <font color=red>② 静态库 ; 

<br>

**2 . 静态库 :** 

<font color=blue>编译链接时 , 将整个库文件打包到可执行文件中 , 造成可执行文件较大 , 但运行时不需要库文件 ; 

<font color=purple>Android 与 Linux 静态库 后缀为 <font color=red>".a" ; 

<br>


**3 . 动态库 :** 

<font color=blue>编译链接时 , 不将库打包入可执行文件中 , 在程序运行时调用到该库时才链接加载该动态库 ; 

<font color=purple>Android 与 Linux 静态库 后缀为 <font color=red>".so" ; 

<br>


**4 . 静态库与动态库对比 :** 

**① 静态库时间效率高 :** <font color=green>执行时没有动态链接的操作 , 所有的代码都在可执行文件内部 , 时间消耗少 ; 

**② 动态库空间效率高 :** <font color=brown>动态库如果被多个程序调用 , 只要有一个动态库在内存中即可 ; 




<br>
<br>

#### II . 编译动态库

---

<br>


**1 . 要编译的源文件 :** <font color=blue>add.c 源文件 ; 

```cpp
#include <stdio.h>

int add(int a, int b){
	return a + b;
}
```

<br>

**2 . 共享动态库编译参数 :** <font color=blue>编译动态库需要添加 <font color=red>"-fPIC" <font color=blue>和 <font color=magenta>"-shared" <font color=blue>两个参数 ; 

<br>

**3 . 编译命令 :** 

<br>


**① 设置 编译器 临时环境变量 :** 

<font color=blue>export <font color=red>CC<font color=blue>=<font color=blue>/home/book/NDK/android-ndk-r17c/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc

<br>

**② 设置指定 头文件和函数库 的临时环境变量 :** 

<font color=blue>export <font color=red>HEAD_LIB<font color=blue>="<font color=green>\-\-sysroot=/home/book/NDK/android-ndk-r17c/platforms/android-21/arch-arm <font color=purple>-isystem /home/book/NDK/android-ndk-r17c/sysroot/usr/include <font color=red>-isystem /home/book/NDK/android-ndk-r17c/sysroot/usr/include/arm-linux-androideabi"

<br>


**③ 最终命令 :** 

<font color=blue>\$CC <font color=green>\$HEAD_LIB  <font color=red>-fPIC <font color=magenta>-shared <font color=purple>add.c <font color=orange>-o libadd.so 

<br>


**4 . 编译结果 :** <font color=blue>在该目录下生成了 libadd.so 动态库 ; 

```shell
book@book-virtual-machine:~/NDK$ export CC=/home/book/NDK/android-ndk-r17c/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc
book@book-virtual-machine:~/NDK$ export HEAD_LIB="--sysroot=/home/book/NDK/android-ndk-r17c/platforms/android-21/arch-arm -isystem /home/book/NDK/android-ndk-r17c/sysroot/usr/include -isystem /home/book/NDK/android-ndk-r17c/sysroot/usr/include/arm-linux-androideabi"
book@book-virtual-machine:~/NDK$ $CC $HEAD_LIB  -fPIC -shared add.c -o libadd.so 
book@book-virtual-machine:~/NDK$ ls
add.c             android-ndk-r17c-linux-x86_64.zip  libadd.so
android-ndk-r17c  a.out                              main.c
book@book-virtual-machine:~/NDK$ 
```


![在这里插入图片描述](https://img-blog.csdnimg.cn/20200210225656161.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hhbjEyMDIwMTI=,size_16,color_FFFFFF,t_70)

<br>

<font color=red>**目录中的 libadd.so 就是编译完成的动态库 , 该动态库可以放到 Android Studio 项目中使用 ;** 



<br>
<br>

#### III. Android Studio 使用第三方动态库

---

<br>


**1 . 拷贝动态库 :** 

<font color=blue>在 AS 项目的 main 目录下 , 创建 <font color=red>**jniLibs/armeabi-v7a** <font color=blue>目录 , 将 libadd.so 拷贝到该目录中 ; 

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200210235735511.png)

<br>


**2 . CMakeLists.txt 中配置动态库 :** 



**配置示例 :** 

```shell
# 设置变量
# CMAKE_CXX_FLAGS 表示会将 C++ 的参数传给编译器
# CMAKE_C_FLAGS 表示会将 C 参数传给编译器

# 参数设置 : 传递 CMAKE_CXX_FLAGS C+= 参数给编译器时 , 在 该参数后面指定库的路径
#   CMAKE_SOURCE_DIR 指的是当前的文件地址
#   -L 参数指定动态库的查找路径
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/../jniLibs/armeabi-v7a")
```

<br>

**3 . Module 级别的 build.gradle 中配置动态库 :** 

<font color=blue>在 android -> defaultConfig -> externalNativeBuild -> cmake 下添加 `abiFilters "armeabi-v7a"` 配置 ; 

```java
defaultConfig {
        ...
        externalNativeBuild {
            cmake {
                cppFlags ""
                abiFilters "armeabi-v7a"
            }
        }
    }
```

<br>


**4 . 在 C++ 中调用该库 :** 

<br>

**① 首先声明该动态库中的方法 :** 

**注意 :** <font color=blue>这是在 C++ 语言中调用 C 语言方法 , 要使用 `extern "C"{}` 兼容 C 和 C++ 调用 ; 

```cpp
//调用 libadd.so 动态库中的方法
extern "C" {    //注意在 C++ 中调用 C 语言方法 , 需要做兼容设置
    extern int add(int a, int b);
}
```

<br>


**② 调用动态库中的函数 :** <font color=red>之后就可以在该 C++ 文件中任意使用该方法了 ; 

<br>


**5 . 执行结果 :** 

```shell
01-25 19:40:27.444 5929-5929/kim.hsl.makeflie I/JNI_TAG: libadd.so : sum = 3
```



<br>
<br>

#### IV . Android Studio 关键代码 

---

<br>

**1 . CMakeLists.txt 配置文件 :** 
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200211000927903.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hhbjEyMDIwMTI=,size_16,color_FFFFFF,t_70)
```shell
cmake_minimum_required(VERSION 3.4.1)

add_library( # Sets the name of the library.
        native-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        native-lib.cpp)


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

target_link_libraries(
        native-lib

        # 表示要链接 libadd.so 动态库
        add

        ${log-lib})
```

<br>
<br>

**2 . native-lib 本地 C++ 文件 :** 
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200211001109192.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hhbjEyMDIwMTI=,size_16,color_FFFFFF,t_70)
```cpp
#include <jni.h>
#include <string>

#include <android/log.h>

//调用 libadd.so 动态库中的方法
extern "C" {    //注意在 C++ 中调用 C 语言方法 , 需要做兼容设置
    extern int add(int a, int b);
}

extern "C" JNIEXPORT jstring JNICALL
Java_kim_hsl_makeflie_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    //调用动态库中的函数
    int sum = add(1, 2);
    //打印计算结果
    __android_log_print(ANDROID_LOG_INFO, "JNI_TAG", "libadd.so : sum = %d", sum);

    return env->NewStringUTF(hello.c_str());
}

```



**3 . Module 级别的 build.gradle 配置文件 :** 

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200211000821788.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hhbjEyMDIwMTI=,size_16,color_FFFFFF,t_70)

```java
apply plugin: 'com.android.application'

android {
    compileSdkVersion 29
    buildToolsVersion "29.0.0"
    defaultConfig {
        applicationId "kim.hsl.makeflie"
        minSdkVersion 15
        targetSdkVersion 29
        versionCode 1
        versionName "1.0"
        testInstrumentationRunner "androidx.test.runner.AndroidJUnitRunner"
        externalNativeBuild {
            cmake {
                cppFlags ""
                abiFilters "armeabi-v7a"
            }
        }
    }
    buildTypes {
        release {
            minifyEnabled false
            proguardFiles getDefaultProguardFile('proguard-android-optimize.txt'), 'proguard-rules.pro'
        }
    }
    externalNativeBuild {
        cmake {
            path "src/main/cpp/CMakeLists.txt"
            version "3.10.2"
        }
    }
}

dependencies {
    implementation fileTree(dir: 'libs', include: ['*.jar'])
    implementation 'androidx.appcompat:appcompat:1.1.0'
    implementation 'androidx.constraintlayout:constraintlayout:1.1.3'
    testImplementation 'junit:junit:4.12'
    androidTestImplementation 'androidx.test:runner:1.2.0'
    androidTestImplementation 'androidx.test.espresso:espresso-core:3.2.0'
}
```




<br>
<br>

#### V . 博客相关资源下载

---

<br>

**CSDN 博客地址 :** [【Android NDK 开发】NDK 交叉编译 ( Ubuntu 中交叉编译动态库 | Android Studio 中配置使用第三方动态库 )](https://hanshuliang.blog.csdn.net/article/details/104255990)

**博客资源下载地址 :** [https://download.csdn.net/download/han1202012/12148685](https://download.csdn.net/download/han1202012/12148685)

**示例代码 GitHub 地址 :** [https://github.com/han1202012/004_NDK_Makeflie](https://github.com/han1202012/004_NDK_Makeflie)
