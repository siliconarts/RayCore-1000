include ./config.mk

SRC_PATH		:= $(PWD)

#-------------------------------------------------
# 		library
#-------------------------------------------------
TARGETPATH		:= $(PWD)
TARGETNAME		:= linux_test
TARGET_EXE		:= $(TARGETPATH)/$(TARGETNAME)
LIBDIR			:= $(LIBDIR) -lX11

#-------------------------------------------------
# 
#-------------------------------------------------
INC  :=	 \
	-I$(SRC_PATH)


SRCS := \
	$(SRC_PATH)/main.cpp

OBJS := $(SRCS:.cpp=.o)

DEPS := $(OBJS:.o=.d)

#-------------------------------------------------
# 
#-------------------------------------------------

all : $(OBJS) $(TARGET_EXE)

%.o: %.cpp
	@echo ' -Compiling...'
	$(CXX) $(CDEFS) $(CFLAGS) -fPIC $(INC) -c $< -o $@

$(TARGET_EXE):
	@echo
	@echo '*** Build execute file ***'
	$(CXX) $(LDFLAGS) -O2 -o $@ $(OBJS) $(LIBDIR)
        
clean:
	@$(RM) $(OBJS)
	@$(RM) $(TARGET_EXE)
	@$(RM) -f $(DEPS) $(DEPS:.d=.d.*)
