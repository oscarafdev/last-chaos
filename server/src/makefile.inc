.SUFFIXES : .cpp .o
.PHONY: clean dep new cleanall local debug clear test

CFLAGS = -O0 -m64
CC = gcc -O0 -m64
CXX = g++ -g -O0 -m64 -Wno-deprecated-declarations -Wno-return-type -Wno-enum-constexpr-conversion

LIB_DIR=/usr/lib64

OSNAME_STR := $(shell cat /etc/os-release | grep NAME | head -1 -)

ifneq (,$(findstring Ubuntu,$(OSNAME_STR)))
LIB_DIR=/usr/lib/x86_64-linux-gnu
$(info NAME OS is $(OSNAME_STR))
$(info LIB_DIR is $(LIB_DIR))
endif

ifneq (,$(findstring Debian,$(OSNAME_STR)))
LIB_DIR=/usr/lib/x86_64-linux-gnu
$(info NAME OS is $(OSNAME_STR))
$(info LIB_DIR is $(LIB_DIR))
endif

ifneq (,$(findstring Gentoo,$(OSNAME_STR)))
$(info NAME OS is $(OSNAME_STR))
$(info LIB_DIR is $(LIB_DIR))
endif

BOTAN_INCLUDE = /usr/include/botan-2

OPT_CRYPT = -I../ShareLib \
		-I/usr/include \
		-I/usr/include/mysql \
		-I/usr/include/jsoncpp \
		-I$(BOTAN_INCLUDE)
		
OPT_LDOBJ = -L../ShareLib \
			-L$(LIB_DIR)\
			-L$(LIB_DIR)/mysql \
			-lSharelib \
			-lmysqlclient \
			-lboost_thread \
			-lboost_system \
			-llog4cxx \
			-laprutil-1 \
			-lapr-1 \
			-lrt \
			-lstdc++ \
			-lexpat \
			-lz \
			-lpthread \
			-ljsoncpp
			
OPT_VER=-DLC_VER=\"$(LC_VER)\"

CXXFLAGS =  -D_THREAD_SAFE -finline-functions \
			-funroll-loops \
			-fdefer-pop \
			-fstrength-reduce \
			-Wno-write-strings \
			-Wno-char-subscripts \
			$(OPT_BIGENDIAN) \
			$(OPT_LANG) \
			$(OPT_DEBUG) \
			$(OPT_DEF) \
			$(OPT_TEST) \
			$(OPT_VER) \
			$(OPT_CRYPT)
