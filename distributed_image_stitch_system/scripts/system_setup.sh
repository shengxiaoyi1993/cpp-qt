
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
  make -f thirdlib.mk linux

elif [[ $1 == "win" ]]; then
  make -f thirdlib.mk win

else
  echo ">>error:parameters wrong!"
fi
