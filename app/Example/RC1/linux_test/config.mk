#
# Embedded Linux Build Enviornment:
#

ARCHNAME   	:= rc

#
#	Toolchain.
#
#CROSS	= arm-none-linux-gnueabi-
CROSS	=
CC 		= $(CROSS)gcc
CXX		= $(CROSS)g++
AR 		= $(CROSS)ar
LD 		= $(CROSS)ld
NM 		= $(CROSS)nm
RANLIB 	= $(CROSS)ranlib
OBJCOPY	= $(CROSS)objcopy
STRIP	= $(CROSS)strip

#
#	Library & Include
#
LIBDIR   = -lstdc++ -lpthread
INCDIR   = 

#
# 	Build flags.
#
#OPTFLAGS = -w -O3 -mfloat-abi=softfp
OPTFLAGS = -w -O3
DBGFLAGS =	# -g
CFLAGS 	 = $(OPTFLAGS) $(DBGFLAGS) $(INCDIR)
ARFLAGS  = crv
CDEFS    = -Wall -D__int64="long long" -DGL_GLEXT_PROTOTYPES
DEP      = $(CC)-M
