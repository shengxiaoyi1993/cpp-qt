TOP_ROOT=$(shell cd ../ && pwd)
PROFILE=${PROJ_NAME}/build/${PROJ_NAME}/${PROJ_NAME}.pro
BUILDFILE=${PROJ_NAME}/build/${PROJ_NAME}/build/
RELEASEFILE=${PROJ_NAME}/build/${PROJ_NAME}/RELEASE_DEMO/
EXEFILE=${PROJ_NAME}


all:
	echo ${TOP_ROOT}
	echo ${PROFILE}
	echo ${BUILDFILE}
	echo ${RELEASEFILE}
	echo ${EXEFILE}
