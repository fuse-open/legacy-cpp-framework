default:
	cmake/cmakemake

android:
	cmake/cmakemake --platform=Android

ios: bin/ios-deploy
	cmake/cmakemake --platform=iOS

xcode:
	cmake/cmakemake --platform=Xcode

check:
	cmake/cmakemake tests
	tests/base-test ${ARGS}

.PHONY: pinvoke
pinvoke:
	sh pinvoke/build.sh

clean:
	rm -rf build include lib

install:
	make install --dir=build/Release

bin/ios-deploy: | bin
	xcodebuild -project 3rdparty/ios-deploy/ios-deploy.xcodeproj
	cp -f 3rdparty/ios-deploy/build/Release/ios-deploy bin

bin:
	mkdir bin
