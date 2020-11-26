TOP_ROOT=$(shell cd ../ && pwd)
PROFILE=${TOP_ROOT}/${PROJ_NAME}/build/${PROJ_NAME}/${PROJ_NAME}.pro
BUILDFILE=${TOP_ROOT}/${PROJ_NAME}/build/${PROJ_NAME}/build/
RELEASEFILE_NAME=RELEASE_${PROJ_NAME}
RELEASEFILE=${TOP_ROOT}/${PROJ_NAME}/build/${PROJ_NAME}/${RELEASEFILE_NAME}/
EXEFILE=${PROJ_NAME}


centos:
	rm -rf ${BUILDFILE}
	rm -rf ${RELEASEFILE}
	cd $(TOP_ROOT)/common_lib/third_party_lib/restclient-cpp && chmod +x autogen.sh && ./autogen.sh && ./configure --prefix=`pwd`/lib/ && make && sudo make install
	qmake -o ${BUILDFILE}Makefile ${PROFILE} "CONFIG+=release"
	cd ${BUILDFILE} &&  make
	rm ${RELEASEFILE} -rf && mkdir ${RELEASEFILE}
	cp ${BUILDFILE}${EXEFILE} ${RELEASEFILE}
	cp ${TOP_ROOT}/common_lib/sskj_lib/com_client/lib/so/libcom.so* ${RELEASEFILE}
	cp ${TOP_ROOT}/common_lib/sskj_lib/com_server/lib/so/libcom_server.so* ${RELEASEFILE}
	cp ${TOP_ROOT}/common_lib/third_party_lib/restclient-cpp/lib/lib/librestclient-cpp.* ${RELEASEFILE}
	-linuxdeployqt-6-x86_64.AppImage ${RELEASEFILE}${EXEFILE} -appimage
	rm  ${BUILDFILE} -rf
	cp ${TOP_ROOT}/resources ${RELEASEFILE} -rf
	tar -zcvf  ${PROJ_NAME}.tar.gz  -C ${RELEASEFILE}../  ${RELEASEFILE_NAME}
	rm  ${RELEASEFILE} -rf


win:
	rm -rf ${BUILDFILE}
	cd $(TOP_ROOT)/common_lib/third_party_lib/restclient-cpp/ && chmod +x autogen.sh && ./autogen.sh && ./configure --prefix=`pwd`/lib/ && make && make install
	qmake -o ${BUILDFILE}Makefile ${PROFILE} "CONFIG+=release"
	cd ${BUILDFILE} && make
	rm ${RELEASEFILE} -rf && mkdir ${RELEASEFILE}
	cp ${BUILDFILE}release/${EXEFILE}.exe ${RELEASEFILE}
	cp ${TOP_ROOT}/common_lib/sskj_lib/com_client/lib/dll/* ${RELEASEFILE}
	cp ${TOP_ROOT}/common_lib/sskj_lib/com_server/lib/dll/* ${RELEASEFILE}
	cp ${TOP_ROOT}/common_lib/third_party_lib/restclient-cpp/lib/lib/* ${RELEASEFILE}
	windeployqt ${RELEASEFILE}${EXEFILE}.exe
	rm  ${BUILDFILE} -rf
	cp ${TOP_ROOT}/resources ${RELEASEFILE} -rf
	tar   ${PROJ_NAME}.zip -C ${RELEASEFILE}../  ${RELEASEFILE_NAME}
	rm  ${RELEASEFILE} -rf


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
