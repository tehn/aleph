# --- config.mk
#
# --- customized makefile for aleph-avr32 application.
# --- this is included via the ASF utility makefile.

APP = lppr

#include boilerplate
include ../../core_config.mk
include ../../core_src.mk

# app sources
CSRCS += \
	$(APP_DIR)/src/app_lppr.c \
	$(APP_DIR)/src/app_timers.c \
	$(APP_DIR)/src/files.c \
	$(APP_DIR)/src/grid.c \
	$(APP_DIR)/src/handler.c \
	$(APP_DIR)/src/ctl.c \
	$(APP_DIR)/src/inputs.c	\
	$(APP_DIR)/src/render.c

# List of assembler source files.
ASSRCS +=

# List of include paths.	
INC_PATH += \
	$(APP_DIR)	           \
	$(APP_DIR)/src

# Additional search paths for libraries.
LIB_PATH = 

# List of libraries to use during linking.
LIBS = 
