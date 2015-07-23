APP_STL := c++_static
APP_MODULES := Easy2DAndroid
APP_ABI := armeabi armeabi-v7a x86
APP_PLATFORM := android-10
#gcc version
#NDK_TOOLCHAIN_VERSION=4.9
#clang
NDK_TOOLCHAIN_VERSION := clang

ifeq ($(APP_DEBUG), true)
  APP_OPTIM := debug
  $(info ************ NDK DEBUG VERSION ************)
else
  APP_OPTIM := release
  $(info ************ NDK RELEASE VERSION **********)
endif
