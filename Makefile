default:
	cmake/cmakemake

android:
	cmake/cmakemake --platform=Android

ios:
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
