
PROFILE=demo_compile.pro
BUILDFILE=build/
RELEASEFILE=RELEASE_DEMO/
EXEFILE=demo_compile


centos:
	rm -rf ${BUILDFILE}
	qmake -o ${BUILDFILE}Makefile ${PROFILE} "CONFIG+=release"
	cd ${BUILDFILE} && make
	rm ${RELEASEFILE} -rf && mkdir ${RELEASEFILE}
	cp ${BUILDFILE}${EXEFILE} ${RELEASEFILE}
	-linuxdeployqt-6-x86_64.AppImage ${RELEASEFILE}${EXEFILE} -appimage
	rm  ${BUILDFILE} -rf

win:
	rm -rf ${BUILDFILE}
	qmake -o ${BUILDFILE}Makefile ${PROFILE} "CONFIG+=release"
	cd ${BUILDFILE} && make
	rm ${RELEASEFILE} -rf && mkdir ${RELEASEFILE}
	cp ${BUILDFILE}${EXEFILE}.exe ${RELEASEFILE}
	windeployqt ${BUILDFILE}${EXEFILE}.exe
	rm  ${BUILDFILE} -rf

	#cp /mingw64/bin/libdouble-conversion.dll build/UVS_CDKey_Generator1.0/
	#cp /mingw64/bin/libicuin67.dll build/UVS_CDKey_Generator1.0/
	#cp /mingw64/bin/libicuuc67.dll build/UVS_CDKey_Generator1.0/
	#cp /mingw64/bin/libpcre2-16-0.dll build/UVS_CDKey_Generator1.0/
	#cp /mingw64/bin/libpng16-16.dll build/UVS_CDKey_Generator1.0/
	#cp /mingw64/bin/libharfbuzz-0.dll build/UVS_CDKey_Generator1.0/
	#cp /mingw64/bin/zlib1.dll build/UVS_CDKey_Generator1.0/
	#cp /mingw64/bin/libicudt67.dll build/UVS_CDKey_Generator1.0/
	#cp /mingw64/bin/libgraphite2.dll build/UVS_CDKey_Generator1.0
	#cp /mingw64/bin/libfreetype-6.dll build/UVS_CDKey_Generator1.0/
	#cp /mingw64/bin/libglib-2.0-0.dll build/UVS_CDKey_Generator1.0/
	#cp /mingw64/bin/libbrotlidec.dll build/UVS_CDKey_Generator1.0/
	#cp /mingw64/bin/libintl-8.dll build/UVS_CDKey_Generator1.0/
	#cp /mingw64/bin/libbz2-1.dll build/UVS_CDKey_Generator1.0/
	#cp /mingw64/bin/libpcre-1.dll build/UVS_CDKey_Generator1.0/
	#cp /mingw64/bin/libbrotlicommon.dll build/UVS_CDKey_Generator1.0/
	#cp /mingw64/bin/libiconv-2.dll build/UVS_CDKey_Generator1.0/
