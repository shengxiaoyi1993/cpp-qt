
amd:build/toml.amd.o

build/toml.amd.o:src/toml.cpp
	g++ $^ -c -o $@

arm:build/toml.arm.o


build/toml.arm.o:src/toml.cpp
	arm-arago-linux-gnueabi-g++ $^ -c -o $@ -std=c++0x

clean:
	rm build/* -rf
	