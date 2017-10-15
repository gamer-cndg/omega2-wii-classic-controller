# C compiler
CC:= mipsel-openwrt-linux-gcc
# path where the toolchain is
TOOLCHAIN_ROOT_DIR:= /home/max/source/staging_dir/target-mipsel_24kc_musl

# additional includes from toolchain
INCLUDE_DIRS:=$(TOOLCHAIN_ROOT_DIR)/usr/include
LIB_DIRS:=$(TOOLCHAIN_ROOT_DIR)/usr/lib

#links to link against
LDFLAGS_LIB:= -loniondebug -lonioni2c 
LDFLAGS_PROGRAM = -lwiiclassic
CFLAGS:= -O3 -ggdb -g -Wall

LIBRARY_SOURCES:= WiiClassic.c Debug.c
PROGRAM_SOURCES = main.c
EXECUTABLE:= main
LIB_NAME:= libwiiclassic.so

.PHONY : library program all clean all

#compile the libwiiclassic.so
library:
	$(CC) -o $(LIB_NAME) -shared -fPIC $(CLFAGS) -I $(INCLUDE_DIRS) -I omega_includes -L $(LIB_DIRS) -L omega_libs $(LIBRARY_SOURCES) $(LDFLAGS_LIB)

program:
	$(CC) -o $(EXECUTABLE) $(CFLAGS) -I $(INCLUDE_DIRS) -I omega_includes -L $(LIB_DIRS) -L omega_libs -L. $(PROGRAM_SOURCES) $(LDFLAGS_PROGRAM) $(LDFLAGS_LIB)

all: | library program

upload:
	sshpass -p "onioneer" scp $(EXECUTABLE) root@192.168.1.131:/root/.
	#upload .so file to /usr/lib so that lib is found by executable
	sshpass -p "onioneer" scp $(LIB_NAME) root@192.168.1.131:/usr/lib/.

clean:
	rm -rf $(EXECUTABLE)
	rm -rf $(LIB_NAME)