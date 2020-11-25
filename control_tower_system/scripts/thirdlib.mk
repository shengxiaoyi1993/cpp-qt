TOP_ROOT=$(shell cd ../ && pwd)
win:
	cd $(TOP_ROOT)/common_lib/third_party_lib/curl-7.64.0/ && ./configure && make && make install
	cd $(TOP_ROOT)/common_lib/third_party_lib/restclient-cpp/ && ./autogen.sh && ./configure --prefix=`pwd`/lib/ && make && make install


linux:
	sudo yum install -y gcc gcc-c++ make boost openssl git autoconf automake libtool
	cd $(TOP_ROOT)/lib/curl-7.64.0/ && chmod +x configure  && ./configure && make && sudo make install
	cd $(TOP_ROOT)/lib/restclient-cpp/ &&chmod +x autogen.sh && ./autogen.sh && ./configure --prefix=`pwd`/lib/ && make && sudo make install

