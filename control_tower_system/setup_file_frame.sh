#!/bin/sh
#系统级文档
mkdir doc && cd doc
  touch system_design.md system_setup.md system_deploy.md system_test.md interface_c_s.md project_manage.md
cd ..
mkdir server && cd server
  cp ../setup_basic_proj.sh .
  bash -e setup_basic_proj.sh
cd ..
mkdir client  && cd client
  cp ../setup_basic_proj.sh .
  bash -e setup_basic_proj.sh
cd ..
mkdir common_lib && cd common_lib
  mkdir third_party_lib && cd third_party_lib
    touch lib
  cd ..
  mkdir sskj_lib && cd sskj_lib
    touch lib
  cd ..
cd ..
mkdir resources && cd resources
  touch icon
cd ..
mkdir scripts && cd scripts
  touch system_setup.sh
  touch system_deploy.sh
  touch system_test.sh
cd ..
