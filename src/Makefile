# C compiler
CC:= mipsel-openwrt-linux-gcc
# path where the toolchain is
TOOLCHAIN_ROOT_DIR:= /home/max/source/staging_dir/target-mipsel_24kc_musl

# additional includes from toolchain
INCLUDE_DIRS:=$(TOOLCHAIN_ROOT_DIR)/usr/include
LIB_DIRS:=$(TOOLCHAIN_ROOT_DIR)/usr/lib

#links to link against
LDFLAGS:= -loniondebug -lonioni2c
CFLAGS:= -O3 -ggdb -g -Wall

SOURCES:= main.c WiiClassic.c Debug.c
EXECUTABLE:= main

all:
	$(CC) -o $(EXECUTABLE) $(CFLAGS) -I $(INCLUDE_DIRS) -I omega_includes -L $(LIB_DIRS) -L omega_libs $(SOURCES) $(LDFLAGS)

upload:
	sshpass -p "onioneer" scp $(EXECUTABLE) root@192.168.1.131:/root/.

clean:
	rm -rf $(EXECUTABLE)
