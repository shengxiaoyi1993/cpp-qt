#!/bin/sh
mkdir build && cd build
  touch deploy.sh
  mkdir deploy_config && cd deploy_config
    touch deploy.config
  cd ..
cd ..

mkdir lib && cd lib
  touch lib
cd ..
mkdir src && cd src
  touch src
cd ..

mkdir doc && cd doc
  touch doc
cd ..
mkdir resources && cd resources
  touch resources
cd ..
mkdir test && cd test
  touch test
cd ..
