# OpenSSL Library for iOS and Android

##English

This a static library compile from openssl for iOS and Android.

##OpenSSL Version

[openssl-1.0.2c.tar.gz](https://www.openssl.org/source/openssl-1.0.2c.tar.gz)

##For iOS

Copy `ios_libs/libcrypto.a` and `ios_libs/libssl.a` to your project.

Copy `include/openssl` folder to your project.

Add `libcrypto.a` and `libssl.a` to `Frameworks` group and add them to `[Build Phases]  ====> [Link Binary With Libraries]`.

Add openssl include path to your `[Build Settings] ====> [User Header Search Paths]`

##For Android

Copy `android_libs/armeabi` folder to your android project `libs` folder.

Copy `include/openssl` folder to your android project.

Add openssl include path to `jni/Android.mk`. 

```
#Android.mk

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/Your Openssl Include Path/openssl \
	
```

##Reference

>[《How-To-Build-openssl-For-iOS》](http://www.cvursache.com/2013/08/13/How-To-Build-openssl-For-iOS/)
>
>[《Compiling the latest OpenSSL for Android》](http://stackoverflow.com/questions/11929773/compiling-the-latest-openssl-for-android)

##中文

这是一个适用于 iOS 平台和 Android 平台的 Openssl 静态链接库。基于 openssl-1.0.2c 版本编译生成。

##在 iOS 工程中使用

将 `ios_libs/libcrypto.a` 和 `ios_libs/libssl.a` 两个静态链接库文件拷贝到你的 iOS 工程下面合适的位置。

将 `include/openssl` 文件夹拷贝到你的 iOS 工程下面合适的位置。注意，所有的头文件均要放置到 `openssl` 文件夹下，不要随意更改文件夹名称。

将 `libcrypto.a` 和 `libssl.a` 两个静态链接库文件通过`[Build Phases]  ====> [Link Binary With Libraries]`引入你的 iOS 工程。

将包含所有头文件的 `openssl` 文件夹设置到头文件搜索路径中。即在 `[Build Settings] ====> [User Header Search Paths]` 中设置好。

##在 Android 工程中使用

将 `android_libs/armeabi` 文件夹拷贝到你的 Android 工程下面的 `libs` 文件夹中。

将 `include/openssl` 文件夹拷贝到你的 Android 工程下面合适的位置。注意，所有的头文件均要放置到 `openssl` 文件夹下，不要随意更改文件夹名称。

修改 `jni/Android.mk` 文件，将头文件路径加入到搜索路径中，例如：

```
#Android.mk

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/Your Openssl Include Path/openssl \
	
```

##参考资料

>[《How-To-Build-openssl-For-iOS》](http://www.cvursache.com/2013/08/13/How-To-Build-openssl-For-iOS/)
>
>[《Compiling the latest OpenSSL for Android》](http://stackoverflow.com/questions/11929773/compiling-the-latest-openssl-for-android)