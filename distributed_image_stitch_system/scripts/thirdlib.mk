TOP_ROOT=$(shell cd ../ && pwd)


rmall:
	rm $(TOP_ROOT)/common_lib/sskj_lib/com_client -rf
	rm $(TOP_ROOT)/common_lib/sskj_lib/com_server -rf
	rm $(TOP_ROOT)/common_lib/third_party_lib/curl-7.64.0 -rf
	rm $(TOP_ROOT)/common_lib/third_party_lib/restclient-cpp -rf

rmall_sudo:
	sudo rm $(TOP_ROOT)/common_lib/sskj_lib/com_client -rf
	sudo rm $(TOP_ROOT)/common_lib/sskj_lib/com_server -rf
	sudo rm $(TOP_ROOT)/common_lib/third_party_lib/curl-7.64.0 -rf
	sudo rm $(TOP_ROOT)/common_lib/third_party_lib/restclient-cpp -rf
	sudo rm $(TOP_ROOT)/common_lib/third_party_lib/opencv-3.4.1/opencv-3.4.1 -rf
	sudo rm $(TOP_ROOT)/common_lib/third_party_lib/opencv-3.4.1/lib -rf
	sudo rm $(TOP_ROOT)/common_lib/third_party_lib/opencv-3.4.1/opencv-3.4.1.zip -rf


getall:
	wget ftp://pub:9900@192.168.1.238/lib/com_client.zip -O $(TOP_ROOT)/common_lib/sskj_lib/com_client.zip
	unzip $(TOP_ROOT)/common_lib/sskj_lib/com_client.zip -d $(TOP_ROOT)/common_lib/sskj_lib/
	wget ftp://pub:9900@192.168.1.238/lib/com_server.zip -O $(TOP_ROOT)/common_lib/sskj_lib/com_server.zip
	unzip $(TOP_ROOT)/common_lib/sskj_lib/com_server.zip -d $(TOP_ROOT)/common_lib/sskj_lib/
	wget ftp://pub:9900@192.168.1.238/lib/restclient-cpp.zip -O $(TOP_ROOT)/common_lib/third_party_lib/restclient-cpp.zip
	unzip $(TOP_ROOT)/common_lib/third_party_lib/restclient-cpp.zip -d $(TOP_ROOT)/common_lib/third_party_lib/
	wget ftp://pub:9900@192.168.1.238/lib/curl-7.64.0.zip -O $(TOP_ROOT)/common_lib/third_party_lib/curl-7.64.0.zip
	unzip $(TOP_ROOT)/common_lib/third_party_lib/curl-7.64.0.zip -d $(TOP_ROOT)/common_lib/third_party_lib/


win: rmall getall
	cd $(TOP_ROOT)/common_lib/third_party_lib/curl-7.64.0/ && ./configure && make && make install
	cd $(TOP_ROOT)/common_lib/third_party_lib/restclient-cpp/ && ./autogen.sh && ./configure --prefix=`pwd`/lib/ && make && make install
	cd $(TOP_ROOT)/common_lib/third_party_lib/opencv-3.4.1 && make

linux: rmall_sudo getall
	sudo yum install -y gcc gcc-c++ make boost openssl git autoconf automake libtool
	cd $(TOP_ROOT)/common_lib/third_party_lib/curl-7.64.0/ && chmod +x configure  && ./configure && make && sudo make install
	cd $(TOP_ROOT)/common_lib/third_party_lib/restclient-cpp/ &&chmod +x autogen.sh && ./autogen.sh && ./configure --prefix=`pwd`/lib/ && make && sudo make install
	cd $(TOP_ROOT)/common_lib/third_party_lib/opencv-3.4.1 && make
