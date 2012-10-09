#makefile for antennaControlTest

CODE_BASE =		C:/Users/dinkem1/Codebase/antennaControllerProject/
#CODE_BASE =			C:/Codebase/eclipse_avr/antennaController/
SRC_CODE_PATH =		$(CODE_BASE)antennaControllerEmbedded/
TEST_CODE_PATH =	$(CODE_BASE)antennaControllerTests/
CPPUTEST_LIB_PATH =	lib/CppUTest/
QP_LIB_PATH =		lib/cygwin/dbg/

CXXPATH =  
INCLUDE_PATH =	-I$(TEST_CODE_PATH)incl \
				-I$(TEST_CODE_PATH)incl/qc \
				-I$(SRC_CODE_PATH)incl \
				 

CC_COMPILER	= -O2 -Wall -fmessage-length=0
CC_INCLUDE	= $(INCLUDE_PATH) 

CXXFLAGS = 	$(CC_COMPILER) $(CC_INCLUDE)

OBJS =		src/runAllTests.o \
			src/mobileAntControllerTest.o \
            $(SRC_CODE_PATH)src/motorControl.o \
            $(SRC_CODE_PATH)src/switchControl.o \
            $(SRC_CODE_PATH)src/realMotor.o \
            $(SRC_CODE_PATH)src/realSwitches.o \
            $(SRC_CODE_PATH)src/realStorage.o \
 

LIBS = $(CPPUTEST_LIB_PATH)libCppUTest.a \
       $(CPPUTEST_LIB_PATH)libCppUTestExt.a \
       $(QP_LIB_PATH)libqp.a \

AUTO_IMPORT = -Xlinker --enable-auto-import

TARGET =		build/runAllTests.exe


$(TARGET):	$(OBJS)
	$(CXX)  $(AUTO_IMPORT) -o $(TARGET) $(OBJS)  $(LIBS) $(INCLUDE_PATH)

all:	$(TARGET)	

debug:	CXX += -DDEBUG -g
debug:	$(TARGET)	

clean:
	rm -f $(OBJS) $(TARGET)
	rm -f $(OBJS) $(DEBUGTARGET)

