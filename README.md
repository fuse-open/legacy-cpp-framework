Xli is a modular and cross-platform app development framework for C++. It is being used to implement the Uno C++ runtime and core library among some other things.

Xli is officially supported on **Android**, **iOS**, **OS X** and **Windows**. The build system also implements support for **Linux** and **Raspberry PI**, but these are not regularly tested. Additionally we're using [SDL2] as fallback, so any other platforms supported by [SDL2] could be added with some tweaks here and there.

Header documentation can be generated using `doxygen`.

Please see the <a href="modules.html">Modules</a> page for an overview of the functionality.

Questions and other feedback can be posted in the beta zone forum.

### Table of Contents

1. [Build instructions](#build-instructions)
    1. [Android](#android)
    2. [iOS](#ios)
    3. [Linux](#linux)
    4. [OS X](#os-x)
    5. [Raspberry PI](#raspberry-pi)
    6. [Windows](#windows)


# <a name="build-instructions"></a>Build instructions

Xli uses [CMake] as its underlying build system for all platforms. If you are familiar with [CMake] you can use the provided `CMakeLists.txt` and do your thing, or follow the instructions presented here for the supported platforms. **Note:** Some platforms (i.e. Linux) may require additional libraries to be installed on the system.

### Clean build

To clean up any old builds from the source tree, delete the directories `build` and `lib`.

## <a name="android"></a>Android

### Prerequisites

- [CMake]
- Android NDK (`ndk-build` must be available in *PATH*)
- Unix Shell

### Instructions - OS X, Linux, MSYS

1. Open terminal and `cd` to Xli directory
2. Execute `./build-android.sh`
   * This should produce .so files located here:
     - `lib/android/armeabi-v7a`

Debug binaries can be produced by replacing command in step 2 with `./build-android.sh --debug`.

Specific architectures can be built by providing the `--archs=` option in step 2 (`armeabi-v7a`).

List of available options can be shown using `./build-android.sh --help`.

### Instructions - Windows

Windows users need a way to execute unix shell scripts. This can be done using [MSYS].

Uno users can use the *Uno Native Build environment* provided by Outracks, containing both [CMake], [MSYS] and the Android SDK + NDK. Open `start.bat`, type `bash -li` to get an [MSYS] shell, then follow the instructions given above.


## <a name="ios"></a>iOS

### Prerequisites

- [CMake]
- Xcode with most recent iOS SDK
- Xcode command line tools
- OS X (>= 10.7)

### Instructions

1. Open terminal and `cd` to Xli directory
2. Execute `./build-iOS.sh`
   * This should produce static libraries located here:
     - `lib/iOS/Debug-iphoneos/`
     - `lib/iOS/Debug-iphonesimulator/`
     - `lib/iOS/Release-iphoneos/`
     - `lib/iOS/Release-iphonesimulator/`
   * Generated Xcode projects located here:
     - `build/iOS/OS/Xli.xcodeproj`
     - `build/iOS/SIMULATOR/Xli.xcodeproj`

Specific architectures can be built by providing the `--archs=` option in step 2 (`OS`, `SIMULATOR`).


## <a name="linux"></a>Linux

### Prerequisites

- [CMake]
- GNU make, C++ compiler, etc
- libcurl (with OpenSSL support)
- libfreetype
- libglew
- libjpeg
- libpng
- libsdl2

**APT** users can execute `sudo apt-get install -y libpng-dev libjpeg-dev libfreetype6-dev libsdl2-dev libglew-dev libcurl4-openssl-dev cmake g++`

### Instructions

1. Open terminal and `cd` to Xli directory
2. Execute `./build.sh`
   * This should produce .so files located here:
     - `lib/linux/x86_32/` *OR* `lib/linux/x86_64/`
3. Optional step: `sudo ./build.sh install`

Debug binaries can be produced by replacing command in step 2 with `./build.sh --debug`.

List of available options can be shown using `./build.sh --help`.


## <a name="os-x"></a>OS X

### Prerequisites

- [CMake]
- Xcode
- Xcode command line tools
- OS X (>= 10.7)

### Instructions

1. Open terminal and `cd` to Xli directory
2. Execute `./build.sh`
   * This should produce universal binaries located here:
     - `lib/OSX/x86/`
3. Optional step: `sudo ./build.sh install`

Debug binaries can be produced by replacing command in step 2 with `./build.sh --debug`.

List of available options can be shown using `./build.sh --help`.

**Note:** When building applications using Xcode, it would be useful to also build Xli using Xcode for better integration with debugger and such. This can be achieved using these alternative instructions:

1. Open terminal and `cd` to Xli directory
2. Execute `./build.sh --platform=xcode`
   * This should produce universal binaries located here:
     - `lib/OSX/x86/Debug/`
     - `lib/OSX/x86/Release/`
   * Generated Xcode project located here:
     - `build/Xcode/Xli.xcodeproj`


## <a name="raspberry-pi"></a>Raspberry PI

### Prerequisites

- [CMake]
- GNU make, C++ compiler, etc
- libcurl (with OpenSSL support)
- libfreetype
- libjpeg
- libpng
- libsdl2

**APT** users can execute `sudo apt-get install -y libpng12-dev libjpeg-dev libfreetype6-dev libcurl4-openssl-dev cmake g++`

[SDL2] can be built and installed from source. To install from Mercurial, do this:

    hg clone http://hg.libsdl.org/SDL
    cd SDL
    ./configure
    make -j 2
    sudo make install

### Instructions

1. Open terminal and `cd` to Xli directory
2. Execute `./build.sh`
   * This should produce .so files located here:
     - `lib/linux/arm/`
3. Optional step: `sudo ./build.sh install`

Debug binaries can be produced by replacing command in step 2 with `./build.sh --debug`.

List of available options can be shown using `./build.sh --help`.


## <a name="windows"></a>Windows

### Prerequisites

- [CMake] \(must be in *PATH*)
- Visual Studio 2013 (or any version down to 2010)
- Windows (>= Vista)

### Instructions - VS2013

1. Execute `build-vs2013.bat`
   * This should produce static libraries located here:
     - `lib\vs2013\x86\Debug\`
     - `lib\vs2013\x86\Release\`
     - `lib\vs2013\x64\Debug\`
     - `lib\vs2013\x64\Release\`
   * Generated Visual Studio 2013 solutions located here:
     - `build\vs2013\x64\Xli.sln`
     - `build\vs2013\x86\Xli.sln`

### Instructions - Other versions

1. Open CMake GUI
2. Browse to Xli directory
3. Configure using one of the Visual Studio generators
4. Open the generated solution in Visual Studio
5. Build

### VS debugger integration

Use a text editor and copy the contents of `vsconfig\autoexp.dat` and paste at the end of your `<VisualStudioFolder>\Common7\Packages\Debugger\autoexp.dat`. Now Visual Studio should be able to more easily inspect instances of Xli types.



[CMake]: http://www.cmake.org/
[MSYS]: http://www.mingw.org/wiki/MSYS
[SDL2]: http://libsdl.org/
