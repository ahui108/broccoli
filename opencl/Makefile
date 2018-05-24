APPNAME = helloworld
APPNAME-CPU = helloworld-cpu
#TOOLCHAIN = /opt/freescale/usr/local/gcc-4.6.2-glibc-2.13-linaro-multilib-2011.12/fsl-linaro-toolchain/ 
#CROSS_COMPILER = $(TOOLCHAIN)/bin/arm-fsl-linux-gnueabi- 
#ROOTFS_DIR=/your_path/ltib/rootfs
ROOTFS_DIR=
CC = $(CROSS_COMPILER)gcc
DEL_FILE = rm -rf
CP_FILE = cp -rf
 
TARGET_PATH_LIB = $(ROOTFS_DIR)/opt/intel/opencl
TARGET_PATH_INCLUDE = $(ROOTFS_DIR)/opt/intel/opencl/include
 
#CFLAGS = -DLINUX -DUSE_SOC_MX6 -Wall -O2 -fsigned-char -march=armv7-a -mfpu=neon -mfloat-abi=softfp \ 
#CFLAGS = -DLINUX -DUSE_SOC_MX6 -Wall -O2 -fsigned-char -mfpu=neon -mfloat-abi=softfp -DEGL_API_FB -DGL_GLEXT_PROTOTYPES -DENABLE_GPU_RENDER_20 -I../include -I$(TARGET_PATH_INCLUDE)
CFLAGS = -DLINUX -Wall -O2 -fsigned-char -DEGL_API_FB -DGL_GLEXT_PROTOTYPES -DENABLE_GPU_RENDER_20 -I../include -I$(TARGET_PATH_INCLUDE)
LFLAGS = -Wl,--library-path=$(TARGET_PATH_LIB),-rpath-link=$(TARGET_PATH_LIB) -L$(TARGET_PATH_LIB) -lm -lOpenCL -ldl -lpthread 
 
OBJECTS = $(APPNAME).o 
 
first: all 
 
all: $(APPNAME) $(APPNAME-CPU)

$(APPNAME): $(OBJECTS)
	$(CC) -o $(APPNAME) $(OBJECTS) $(LFLAGS)

$(APPNAME).o: $(APPNAME).c
	$(CC) $(CFLAGS) -c -o $(APPNAME).o $(APPNAME).c 

$(APPNAME-CPU): $(APPNAME-CPU).o
	$(CC) -o $(APPNAME-CPU) $(APPNAME-CPU).o

$(APPNAME-CPU).o: $(APPNAME-CPU).c
	$(CC) $(CFLAGA) -c -o $(APPNAME-CPU).o $(APPNAME-CPU).c

clean:
	$(DEL_FILE) $(APPNAME) $(OBJECTS) $(APPNAME-CPU) $(APPNAME-CPU).o
