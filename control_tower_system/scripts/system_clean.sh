


if [[ $# -lt 1 ]]; then
  echo " Too few parameters "
  exit 1
elif [[ $# -gt 1 ]]; then
  echo " Too many parameters "
  exit 1
else
  echo "parameters:"$1
  echo "start to conclude..."
  echo "........."
fi



if [[ $1 == "centos" ]]; then
  rm ../common_lib/sskj_lib/*.zip
  rm ../common_lib/third_party_lib/*.zip
  
  make -f thirdlib.mk rmall_sudo
  rm ../server_service/build/server_service/build-server_service-Desktop_Qt_5_9_7_GCC_64bit-Debug/ -rf
  rm ../server_service/build/server_service/server_service.pro.user

  rm ../server/build/server/server.pro.user
  rm ../server/build/server/build-server-Desktop_Qt_5_9_7_GCC_64bit-Debug/ -rf

  rm ../client/build/client/client.pro.user
  rm ../client/build/client/build-client-Desktop_Qt_5_9_7_GCC_64bit-Debug/ -rf




elif [[ $1 == "win" ]]; then
  rm ../common_lib/sskj_lib/*.zip
  rm ../common_lib/third_party_lib/*.zip

  make -f thirdlib.mk rmall
  rm ../server_service/build/server_service/build-server_service-Desktop_Qt_5_9_7_GCC_64bit-Debug/ -rf
  rm ../server_service/build/server_service/server_service.pro.user

  rm ../server/build/server/server.pro.user
  rm ../server/build/server/build-server-Desktop_Qt_5_9_7_GCC_64bit-Debug/ -rf

  rm ../client/build/client/client.pro.user
  rm ../client/build/client/build-client-Desktop_Qt_5_9_7_GCC_64bit-Debug/ -rf
else
  echo ">>error:parameters wrong!"
fi

rm
