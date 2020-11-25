
if [[ $# -lt 1 ]]; then
  echo " Too few parameters "
elif [[ $# -gt 1 ]]; then
  echo " Too many parameters "
else
  echo "parameters:"$1
  echo "start to conclude..."
  echo "........."
fi


if [[ $1 == "centos" ]]; then
  make -f build.mk centos PROJ_NAME=server_service
  make -f build.mk centos PROJ_NAME=server
  make -f build.mk centos PROJ_NAME=client
elif [[ $1 == "win" ]]; then
  make -f build.mk win PROJ_NAME=server_service
  make -f build.mk win PROJ_NAME=server
  make -f build.mk win PROJ_NAME=client
else
  echo ">>error:parameters wrong!"
fi
