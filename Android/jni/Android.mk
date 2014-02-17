#init configure file
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
######################################################################
LOCAL_MODULE := Easy2DAndroid
LOCAL_ARM_MODE := arm
######################################################################
#global paths
DIP_PATH= ../../dependencies-android
EASY2D_PATH= ../../
######################################################################
#dependencies paths
DIP_PATH_LIB= $(DIP_PATH)/lib
DIP_PATH_INCLUDE= $(DIP_PATH)/include
#libs files
DIP_STATIC_LIBS=  $(wildcard $(LOCAL_PATH)/$(DIP_PATH_LIB)/*.a)
######################################################################
#glue path
GLUE_PATH_SRC= native_app_glue
GLUE_PATH_INCLUDE= native_app_glue
#cpp files
GLUE_C =$(wildcard $(LOCAL_PATH)/$(GLUE_PATH_SRC)/*.c)
#h files
GLUE_H =$(wildcard $(LOCAL_PATH)/$(GLUE_PATH_INCLUDE)/*.h)
######################################################################
#easy2d paths
EASY2D_PATH_SRC= $(EASY2D_PATH)/src
EASY2D_PATH_SRC_IMAGE= $(EASY2D_PATH)/src/Image
EASY2D_PATH_SRC_THREAD= $(EASY2D_PATH)/src/thread
EASY2D_PATH_INCLUDE= $(EASY2D_PATH)/include
EASY2D_PATH_INCLUDE_THREAD= $(EASY2D_PATH)/include/thread
#easy2d/android paths
EASY2D_PATH_SRC_ANDROID= $(EASY2D_PATH)/src/android
EASY2D_PATH_INCLUDE_ANDROID= $(EASY2D_PATH)/include/android
#cpp files
EASY2D_C   =$(wildcard $(LOCAL_PATH)/$(EASY2D_PATH_SRC_ANDROID)/*.c)
EASY2D_CPP =$(wildcard $(LOCAL_PATH)/$(EASY2D_PATH_SRC)/*.cpp)
EASY2D_CPP+=$(wildcard $(LOCAL_PATH)/$(EASY2D_PATH_SRC_IMAGE)/*.cpp)
EASY2D_CPP+=$(wildcard $(LOCAL_PATH)/$(EASY2D_PATH_SRC_THREAD)/*.cpp)
EASY2D_CPP+=$(wildcard $(LOCAL_PATH)/$(EASY2D_PATH_SRC_ANDROID)/*.cpp)
#h files
EASY2D_H =$(wildcard $(LOCAL_PATH)/$(EASY2D_PATH)/*.h)
EASY2D_H+=$(wildcard $(LOCAL_PATH)/$(EASY2D_PATH_INCLUDE)/*.h)
EASY2D_H+=$(wildcard $(LOCAL_PATH)/$(EASY2D_PATH_INCLUDE_THREAD)/*.h)
EASY2D_H+=$(wildcard $(LOCAL_PATH)/$(EASY2D_PATH_INCLUDE_ANDROID)/*.h)
#delete nano
toRemove:= Math3D_neon.cpp
EASY2D_CPP:= $(subst $(toRemove),$(empty),$(EASY2D_CPP))
#delete vfp
toRemove:= Math3D_vfp.cpp
EASY2D_CPP:= $(subst $(toRemove),$(empty),$(EASY2D_CPP))
#delete SSE2
toRemove:= Math3D_SSE2.cpp
EASY2D_CPP:= $(subst $(toRemove),$(empty),$(EASY2D_CPP))
#delete WINDOW MUTEX
toRemove:= thread/MutexWin32.cpp
EASY2D_CPP:= $(subst $(toRemove),$(empty),$(EASY2D_CPP))
#delete WINDOW THREAD
toRemove:= thread/ThreadWin32.cpp
EASY2D_CPP:= $(subst $(toRemove),$(empty),$(EASY2D_CPP))
#delete memory
toRemove:= Memory.cpp
EASY2D_CPP:= $(subst $(toRemove),$(empty),$(EASY2D_CPP))
######################################################################
#configure android build:
#libs #:$(LOCAL_PATH)/%=%)
LOCAL_LDLIBS:= -llog -lz -landroid -lGLESv1_CM -lGLESv2 -landroid -lEGL  -s -lgnustl_static -lsupc++ 
LOCAL_LDLIBS+=$(DIP_STATIC_LIBS)
#include
LOCAL_C_INCLUDES:=$(LOCAL_PATH)/$(GLUE_PATH_INCLUDE)\
                  $(LOCAL_PATH)/$(DIP_PATH_INCLUDE)\
                  $(LOCAL_PATH)/$(EASY2D_PATH)\
                  $(LOCAL_PATH)/$(EASY2D_PATH_INCLUDE)\
                  $(LOCAL_PATH)/$(EASY2D_PATH_INCLUDE_THREAD)\
                  $(LOCAL_PATH)/$(EASY2D_PATH_INCLUDE_ANDROID)		  
#files
LOCAL_SRC_FILES:=$(GLUE_C:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES+=$(GLUE_H:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES+=$(EASY2D_C:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES+=$(EASY2D_CPP:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES+=$(EASY2D_H:$(LOCAL_PATH)/%=%)
#openGL extra propriety
LOCAL_CFLAGS += -D__STDC_LIMIT_MACROS -DDEF_SET_OPENGL_ES2 -DGL_GLEXT_PROTOTYPES
#easy2d extra propriety
LOCAL_CFLAGS += -DDISABLE_SIMD -DORDERED_TABLE -DDISABLE_FORCE_INLINE
#-std=c++11
LOCAL_CPPFLAGS   += -std=gnu++11 -frtti -fexceptions -DDISABLE_FORCE_INLINE
#debug
#LOCAL_CFLAGS += -g -ggdb 
#release
LOCAL_CFLAGS += -ffast-math -O3 -Wno-psabi 

######################################################################
include $(BUILD_STATIC_LIBRARY)
######################################################################