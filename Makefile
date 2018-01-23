# -*- Makefile -*-

PROJECT_NAME = lmark_gui

# Define Directories
BUILD_DIR = build
BINARIES_DIR = bin
INCLUDE_DIR = inc
SOURCES_DIR = src

# Include QT dependancies
QT5_INC = /usr/include/x86_64-linux-gnu/qt5
QT5WIDGETS_INC = /usr/include/x86_64-linux-gnu/qt5/QtWidgets
QT5GUI_INC = /usr/include/x86_64-linux-gnu/qt5/QtGui
QT5CORE_INC = /usr/include/x86_64-linux-gnu/qt5/QtCore

# QT libraries and defines
QT_LIBS = -L/usr/X11R6/lib64 -lQt5Widgets -lQt5Gui -lQt5Core -lGL -lpthread -L/usr/lib/x86_64-linux-gnu
QT_DEFINES = -DQT_NO_DEBUG -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB

# All .cpp sources relevant for GUI
SOURCES_USER += $(SOURCES_DIR)/main.cpp \
				$(SOURCES_DIR)/moc_morus_main_window.cpp \
				$(SOURCES_DIR)/morus_main_window.cpp \
				$(SOURCES_DIR)/node_handler.cpp \
				$(SOURCES_DIR)/platform_linux.cpp


# Include libuavcan .mk file - Initialize libuavcan sources / directories / includes
include libuavcan/libuavcan/include.mk
SOURCES_LIBUAVCAN += $(LIBUAVCAN_SRC)
INCLUDES_ALL += -I$(LIBUAVCAN_INC)

# Invoke DSDL compiler - translate morus_uavcan_msgs to .hpp files and compile uavcan
$(info $(shell $(LIBUAVCAN_DSDLC) "./morus_uavcan_msgs" $(UAVCAN_DSDL_DIR)))
# dsdlc_generated - output file for dsdl_compiler
INCLUDES_ALL += -Idsdlc_generated

# Application preprocessor definitions - define NDEBUG
UAVCAN_DEFINES += -DNDEBUG

# Construct object file names for recipes
OBJECTS_USER = $(addprefix $(BUILD_DIR)/, $(addsuffix .o, $(basename $(SOURCES_USER))))
OBJECTS_LIBUAVCAN = $(addprefix $(BUILD_DIR)/, $(addsuffix .o, $(basename $(SOURCES_LIBUAVCAN))))

# Add rest of the includes
INCLUDES_ALL += -I$(INCLUDE_DIR) \
				-I$(QT5_INC) \
				-I$(QT5WIDGETS_INC) \
				-I$(QT5GUI_INC) \
				-I$(QT5CORE_INC) \
				-Ilibuavcan/libuavcan_drivers/linux/include \
				-I.

CC = gcc
CPP = g++
LD = gcc
LDPP = g++
AR = ar
OBJCOPY = objcopy
GDB = gdb

# Define colors
NO_COLOR=\033[0m
OK_COLOR=\033[32;01m
ERROR_COLOR=\033[31;01m
WARN_COLOR=\033[33;01m

# Define error strings
OK_STRING=$(OK_COLOR)[OK]$(NO_COLOR)
ERROR_STRING=$(ERROR_COLOR)[ERRORS]$(NO_COLOR)
WARN_STRING=$(WARN_COLOR)[WARNINGS]$(NO_COLOR)

# Define output strings
#AWK_CMD = awk '{ printf "%-30s %-10s\n",$$1, $$2; }'
AWK_CMD = awk '{ printf "%-30s %-10s\n",$$1, $$2; }'
PRINT_ERROR = printf "$@ $(ERROR_STRING)\n" | $(AWK_CMD) && printf "$(CMD)\n$$LOG\n" && false
PRINT_WARNING = printf "$@ $(WARN_STRING)\n" | $(AWK_CMD) && printf "$(CMD)\n$$LOG\n"
PRINT_OK = printf "$@ $(OK_STRING)\n" | $(AWK_CMD)

# LOG prints out compiler to stdout. 
# If it exited with 1 then error occured, else print warninig / nothing
BUILD_CMD = LOG=$$($(CMD) 2>&1) ; if [ $$? -eq 1 ]; then $(PRINT_ERROR); \
	elif [ "$$LOG" != "" ] ; then $(PRINT_WARNING); else $(PRINT_OK); fi;

# CPP compiler flags
CPPFLAGS = -O0 -g -Wall -I. \
	-std=c++11 $(INCLUDES_ALL) \
	-m64 -pipe -W -D_REENTRANT -fPIE -fPIC

# Define output - executable file
OUT_EXEC = $(BUILD_DIR)/$(PROJECT_NAME)

# Output executable recipe
$(OUT_EXEC): $(OBJECTS_USER) $(OBJECTS_LIBUAVCAN)
			 @$(LDPP) -o $@ $(OBJECTS_USER) $(OBJECTS_LIBUAVCAN) $(QT_LIBS)
			 @$(BUILD_CMD) # TODO: Redundant (?)

# Generic user object file recipes 
# Initialize compiler command
$(BUILD_DIR)/src/%.o: CMD = $(CPP) -c $(CPPFLAGS) $< -o $@
# Compiles user - sources to object files
$(BUILD_DIR)/src/%.o: src/%.c**
			@mkdir -p $(dir $@)
			@$(BUILD_CMD)

# Generic libuavcan object file recipes
# Initialize comipler command
$(BUILD_DIR)/libuavcan/%.o: CMD = $(CPP) -c $(CPPFLAGS) $(UAVCANDEFS) $< -o $@
# Compiles libuavcan sources to object files
$(BUILD_DIR)/libuavcan/%.o: libuavcan/%.cpp
			@mkdir -p $(dir $@)
			@$(BUILD_CMD)

libuavcan: $(OBJECTS_LIBUAVCAN)

userlib: $(OBJECTS_USER)

.PHONY: clean
clean:	
		@rm -rf build dsdlc_generated can_test
		@$(PRINT_OK)


# Build can_test
can_test: /build/src/node.o /build/src/platform_linux.o $(OBJECTS_LIBUAVCAN)
			$(LDPP) -o $@ /build/src/node.o /build/src/platform_linux.o $(OBJECTS_LIBUAVCAN)
			$(BUILD_CMD)


/build/src/node.o: CMD = $(CPP) -c $(CPPFLAGS) $(UAVCANDEFS) $< -o $@
/build/src/node.o: $(SOURCES_DIR)/test/node.cpp
			mkdir -p $(dir $@)
			$(BUILD_CMD)

/build/src/platform_linux.o: CMD = $(CPP) -c $(CPPFLAGS) $(UAVCANDEFS) $< -o $@
/build/src/platform_linux.o: $(SOURCES_DIR)/platform_linux.cpp
			mkdir -p $(dir $@)
			$(BUILD_CMD)
			
# Build subscriber / publisher
publisher: /build/src/publisher.o /build/src/platform_linux.o $(OBJECTS_LIBUAVCAN)
			$(LDPP) -o $@ /build/src/publisher.o /build/src/platform_linux.o $(OBJECTS_LIBUAVCAN)
			$(BUILD_CMD)

subscriber: /build/src/subscriber.o /build/src/platform_linux.o $(OBJECTS_LIBUAVCAN)
			$(LDPP) -o $@ /build/src/subscriber.o /build/src/platform_linux.o $(OBJECTS_LIBUAVCAN)
			$(BUILD_CMD)

/build/src/publisher.o: CMD = $(CPP) -c $(CPPFLAGS) $(UAVCANDEFS) $< -o $@
/build/src/publisher.o: $(SOURCES_DIR)/test/publisher.cpp
			mkdir -p $(dir $@)
			$(BUILD_CMD)

/build/src/subscriber.o: CMD = $(CPP) -c $(CPPFLAGS) $(UAVCANDEFS) $< -o $@
/build/src/subscriber.o: $(SOURCES_DIR)/test/subscriber.cpp
			mkdir -p $(dir $@)
			$(BUILD_CMD)

all:	can_test publisher subscriber