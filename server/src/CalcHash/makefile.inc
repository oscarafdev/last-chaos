.SUFFIXES : .cpp .o
.PHONY: clean dep new cleanall local debug clear test

DST_DIR=${HOME}/LCS2018.x64/usr/local

CFLAGS = -O0 -m64
CC = gcc -O0 -m64
CXX = g++ -g -O0 -m64 -Wno-deprecated-declarations -Wno-return-type -Wno-enum-constexpr-conversion

BOOST_INCLUDE = ${DST_DIR}/include
BOOST_LIB_VER = -gcc12-mt-x64-1_78

OPT_CRYPT = -I../ShareLib \
		-I${DST_DIR}/include \
		-I/usr/include/mysql \
		-I/usr/local/ssl/include \
		-I/usr/include \
		-I$(BOOST_INCLUDE)

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
