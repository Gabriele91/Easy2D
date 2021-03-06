#init configure file
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
######################################################################
LOCAL_MODULE := Easy2DAndroid
#LOCAL_ARM_MODE := arm
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
#easy2d paths
EASY2D_PATH_SRC= $(EASY2D_PATH)/src
EASY2D_PATH_SRC_GUI= $(EASY2D_PATH)/src/gui
EASY2D_PATH_SRC_UI= $(EASY2D_PATH)/src/ui
EASY2D_PATH_SRC_AUDIO= $(EASY2D_PATH)/src/audio
EASY2D_PATH_SRC_IMAGE= $(EASY2D_PATH)/src/image
EASY2D_PATH_SRC_THREAD= $(EASY2D_PATH)/src/thread
EASY2D_PATH_SRC_SCRIPT= $(EASY2D_PATH)/src/script
EASY2D_PATH_SRC_SOCKET= $(EASY2D_PATH)/src/socket
EASY2D_PATH_INCLUDE= $(EASY2D_PATH)/include
EASY2D_PATH_INCLUDE_GUI= $(EASY2D_PATH)/include/gui
EASY2D_PATH_INCLUDE_UI= $(EASY2D_PATH)/include/ui
EASY2D_PATH_INCLUDE_IMAGE= $(EASY2D_PATH)/include/image
EASY2D_PATH_INCLUDE_AUDIO= $(EASY2D_PATH)/include/audio
EASY2D_PATH_INCLUDE_THREAD= $(EASY2D_PATH)/include/thread
EASY2D_PATH_INCLUDE_SCRIPT= $(EASY2D_PATH)/include/script
EASY2D_PATH_INCLUDE_SOCKET= $(EASY2D_PATH)/include/socket
#easy2d/android paths
EASY2D_PATH_SRC_ANDROID= $(EASY2D_PATH)/src/android
EASY2D_PATH_INCLUDE_ANDROID= $(EASY2D_PATH)/include/android
#cpp files
EASY2D_C   =$(wildcard $(LOCAL_PATH)/$(EASY2D_PATH_SRC_ANDROID)/*.c)
EASY2D_CPP =$(wildcard $(LOCAL_PATH)/$(EASY2D_PATH_SRC)/*.cpp)
EASY2D_CPP+=$(wildcard $(LOCAL_PATH)/$(EASY2D_PATH_SRC_GUI)/*.cpp)
EASY2D_CPP+=$(wildcard $(LOCAL_PATH)/$(EASY2D_PATH_SRC_UI)/*.cpp)
EASY2D_CPP+=$(wildcard $(LOCAL_PATH)/$(EASY2D_PATH_SRC_AUDIO)/*.cpp)
EASY2D_CPP+=$(wildcard $(LOCAL_PATH)/$(EASY2D_PATH_SRC_IMAGE)/*.cpp)
EASY2D_CPP+=$(wildcard $(LOCAL_PATH)/$(EASY2D_PATH_SRC_THREAD)/*.cpp)
EASY2D_CPP+=$(wildcard $(LOCAL_PATH)/$(EASY2D_PATH_SRC_SCRIPT)/*.cpp)
EASY2D_CPP+=$(wildcard $(LOCAL_PATH)/$(EASY2D_PATH_SRC_SOCKET)/*.cpp)
EASY2D_CPP+=$(wildcard $(LOCAL_PATH)/$(EASY2D_PATH_SRC_ANDROID)/*.cpp)
#h files
EASY2D_H =$(wildcard $(LOCAL_PATH)/$(EASY2D_PATH)/*.h)
EASY2D_H+=$(wildcard $(LOCAL_PATH)/$(EASY2D_PATH_INCLUDE)/*.h)
EASY2D_H+=$(wildcard $(LOCAL_PATH)/$(EASY2D_PATH_INCLUDE_GUI)/*.h)
EASY2D_H+=$(wildcard $(LOCAL_PATH)/$(EASY2D_PATH_INCLUDE_UI)/*.h)
EASY2D_H+=$(wildcard $(LOCAL_PATH)/$(EASY2D_PATH_INCLUDE_IMAGE)/*.h)
EASY2D_H+=$(wildcard $(LOCAL_PATH)/$(EASY2D_PATH_INCLUDE_AUDIO)/*.h)
EASY2D_H+=$(wildcard $(LOCAL_PATH)/$(EASY2D_PATH_INCLUDE_THREAD)/*.h)
EASY2D_H+=$(wildcard $(LOCAL_PATH)/$(EASY2D_PATH_INCLUDE_SCRIPT)/*.h)
EASY2D_H+=$(wildcard $(LOCAL_PATH)/$(EASY2D_PATH_INCLUDE_SOCKET)/*.h)
EASY2D_H+=$(wildcard $(LOCAL_PATH)/$(EASY2D_PATH_INCLUDE_ANDROID)/*.h)
#remove no x86/arm files
ifeq ($(APP_OPTIM),debug)
		#delete nano
		toRemove:= Math3D_neon.cpp
		EASY2D_CPP:= $(subst $(toRemove),$(empty),$(EASY2D_CPP))
		#delete vfp
		toRemove:= Math3D_vfp.cpp
		EASY2D_CPP:= $(subst $(toRemove),$(empty),$(EASY2D_CPP))
		#delete SSE2
		toRemove:= Math3D_SSE2.cpp
		EASY2D_CPP:= $(subst $(toRemove),$(empty),$(EASY2D_CPP))
else
	ifeq ($(TARGET_ARCH_ABI),x86)
		#delete nano
		toRemove:= Math3D_neon.cpp
		EASY2D_CPP:= $(subst $(toRemove),$(empty),$(EASY2D_CPP))
		#delete vfp
		toRemove:= Math3D_vfp.cpp
		EASY2D_CPP:= $(subst $(toRemove),$(empty),$(EASY2D_CPP))
	else
		#type of arch neon
		ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
			#delete vfp
			toRemove:= Math3D_vfp.cpp
			EASY2D_CPP:= $(subst $(toRemove),$(empty),$(EASY2D_CPP))
		endif	
		#type of arch vfp
		ifeq ($(TARGET_ARCH_ABI),armeabi)
			#delete nano
			toRemove:= Math3D_neon.cpp
			EASY2D_CPP:= $(subst $(toRemove),$(empty),$(EASY2D_CPP))
		endif
		#delete SSE2
		toRemove:= Math3D_SSE2.cpp
		EASY2D_CPP:= $(subst $(toRemove),$(empty),$(EASY2D_CPP))
	endif
endif
#delete WINDOW MUTEX
toRemove:= thread/MutexWin32.cpp
EASY2D_CPP:= $(subst $(toRemove),$(empty),$(EASY2D_CPP))
#delete WINDOW THREAD
toRemove:= thread/ThreadWin32.cpp
EASY2D_CPP:= $(subst $(toRemove),$(empty),$(EASY2D_CPP))
#delete memory
toRemove:= Memory.cpp
EASY2D_CPP:= $(subst $(toRemove),$(empty),$(EASY2D_CPP))
#delete OpenGL Render (Only OpenGLES2 render)
toRemove:= RenderContextOGL2.cpp
EASY2D_CPP:= $(subst $(toRemove),$(empty),$(EASY2D_CPP))
######################################################################
#configure android build:
#libs #:$(LOCAL_PATH)/%=%)
#OpenGL 1.1 add  -lGLESv1_CM
#link libes
LOCAL_LDLIBS:= -llog -lz -landroid -lGLESv2 -landroid -lEGL  -s -lgnustl_static -lsupc++ 
LOCAL_LDLIBS+=$(DIP_STATIC_LIBS)
#include
LOCAL_C_INCLUDES:=$(LOCAL_PATH)/$(DIP_PATH_INCLUDE)\
                  $(LOCAL_PATH)/$(EASY2D_PATH)\
                  $(LOCAL_PATH)/$(EASY2D_PATH_INCLUDE)\
                  $(LOCAL_PATH)/$(EASY2D_PATH_INCLUDE_GUI)\
                  $(LOCAL_PATH)/$(EASY2D_PATH_INCLUDE_UI)\
                  $(LOCAL_PATH)/$(EASY2D_PATH_INCLUDE_AUDIO)\
                  $(LOCAL_PATH)/$(EASY2D_PATH_INCLUDE_IMAGE)\
                  $(LOCAL_PATH)/$(EASY2D_PATH_INCLUDE_THREAD)\
                  $(LOCAL_PATH)/$(EASY2D_PATH_INCLUDE_SCRIPT)\
                  $(LOCAL_PATH)/$(EASY2D_PATH_INCLUDE_SOCKET)\
                  $(LOCAL_PATH)/$(EASY2D_PATH_INCLUDE_ANDROID)		  
#files
LOCAL_SRC_FILES:=$(GLUE_C:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES+=$(GLUE_H:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES+=$(EASY2D_C:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES+=$(EASY2D_CPP:$(LOCAL_PATH)/%=%)


######################################################################
#glue path
#GLUE_PATH_SRC= native_app_glue
#GLUE_PATH_INCLUDE= native_app_glue
#cpp files
#GLUE_C =$(wildcard $(LOCAL_PATH)/$(GLUE_PATH_SRC)/*.c)
#h files
#GLUE_H =$(wildcard $(LOCAL_PATH)/$(GLUE_PATH_INCLUDE)/*.h)
#glue
#LOCAL_C_INCLUDES+=$(LOCAL_PATH)/$(GLUE_PATH_INCLUDE)
#LOCAL_SRC_FILES+=$(EASY2D_H:$(LOCAL_PATH)/%=%)
######################################################################

#OpenGL extra propriety
LOCAL_CFLAGS += -D__STDC_LIMIT_MACROS -DDEF_SET_OPENGL_ES2 -DGL_GLEXT_PROTOTYPES
#easy2d extra propriety
LOCAL_CFLAGS += -DORDERED_TABLE -DDISABLE_FORCE_INLINE -DUSE_LUA -DENABLE_STREAM_BUFFER
#-std=c++11
LOCAL_CPPFLAGS += -std=c++14 -frtti -fexceptions -DDISABLE_FORCE_INLINE

ifeq ($(APP_OPTIM),debug)
	#debug
	LOCAL_CFLAGS += -g -ggdb 
	LOCAL_CFLAGS += -D_DEBUG
	LOCAL_CFLAGS += -DDISABLE_SIMD
else
	#release
	LOCAL_CFLAGS += -ffast-math -O3 -Wno-psabi 
	LOCAL_CFLAGS += -D_DEBUG
	LOCAL_CFLAGS += -DENABLE_SIMD
	#type of arch neon
	ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
		LOCAL_CFLAGS   += -mfpu=neon -mfloat-abi=softfp
	endif	
	#type of arch vfp
	ifeq ($(TARGET_ARCH_ABI),armeabi)
		LOCAL_CFLAGS   += -mfpu=vfp -mfloat-abi=softfp -mcpu=cortex-m3
	endif
endif
######################################################################
include $(BUILD_STATIC_LIBRARY)
######################################################################