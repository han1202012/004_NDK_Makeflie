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
