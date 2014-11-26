#!/bin/bash
mkdir build/ios
lipo -create build/Debug-iphoneos/libEasy2D-iOS.a build/Debug-iphonesimulator/libEasy2D-iOS.a -output build/ios/libEasy2D-iOS-D.a
lipo -create build/Release-iphoneos/libEasy2D-iOS.a build/Release-iphonesimulator/libEasy2D-iOS.a -output build/ios/libEasy2D-iOS-R.a
