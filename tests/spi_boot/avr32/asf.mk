# atmel software framework includes...
ASF_INC += -I../../../avr/asf-2.6.0/common
ASF_INC += -I./src/SOFTWARE_FRAMEWORK/DRIVERS/GPIO 
ASF_INC += -I./src/SOFTWARE_FRAMEWORK/UTILS/LIBS/NEWLIB_ADDONS/INCLUDE 
ASF_INC += -I./src/SOFTWARE_FRAMEWORK/UTILS/PREPROCESSOR 
ASF_INC += -I./src/SOFTWARE_FRAMEWORK/UTILS 
ASF_INC += -I./src/SOFTWARE_FRAMEWORK/DRIVERS/INTC 
ASF_INC += -I./src/SOFTWARE_FRAMEWORK/BOARDS 
ASF_INC += -I./src/SOFTWARE_FRAMEWORK/DRIVERS/SPI 
ASF_INC += -I./src/SOFTWARE_FRAMEWORK/DRIVERS/PM 
ASF_INC += -I./src/SOFTWARE_FRAMEWORK/DRIVERS/EIC 
ASF_INC += -I./src/SOFTWARE_FRAMEWORK/DRIVERS/TC 

# ASF sources
ASF_SRC += src/SOFTWARE_FRAMEWORK/UTILS/STARTUP_FILES/GCC/crt0.x
ASF_SRC += src/SOFTWARE_FRAMEWORK/DRIVERS/TC/tc.c
ASF_SRC += src/SOFTWARE_FRAMEWORK/DRIVERS/SPI/spi.c
ASF_SRC += src/SOFTWARE_FRAMEWORK/DRIVERS/PM/power_clocks_lib.c
ASF_SRC += src/SOFTWARE_FRAMEWORK/DRIVERS/PM/pm_conf_clocks.c
ASF_SRC += src/SOFTWARE_FRAMEWORK/DRIVERS/PM/pm.c
ASF_SRC += src/SOFTWARE_FRAMEWORK/DRIVERS/INTC/intc.c
ASF_SRC += src/SOFTWARE_FRAMEWORK/DRIVERS/INTC/exception.x
ASF_SRC += src/SOFTWARE_FRAMEWORK/DRIVERS/GPIO/gpio.c
ASF_SRC += src/SOFTWARE_FRAMEWORK/DRIVERS/EIC/eic.c 
ASF_SRC += src/SOFTWARE_FRAMEWORK/BOARDS/EVK1100/led.c 
ASF_SRC += src/SOFTWARE_FRAMEWORK/ASM/trampoline.c

# ASF objects
ASF_OBJ += src/SOFTWARE_FRAMEWORK/UTILS/STARTUP_FILES/GCC/crt0.o
ASF_OBJ += src/SOFTWARE_FRAMEWORK/DRIVERS/TC/tc.o
ASF_OBJ += src/SOFTWARE_FRAMEWORK/DRIVERS/SPI/spi.o
ASF_OBJ += src/SOFTWARE_FRAMEWORK/DRIVERS/PM/power_clocks_lib.o
ASF_OBJ += src/SOFTWARE_FRAMEWORK/DRIVERS/PM/pm_conf_clocks.o
ASF_OBJ += src/SOFTWARE_FRAMEWORK/DRIVERS/PM/pm.o
ASF_OBJ += src/SOFTWARE_FRAMEWORK/DRIVERS/INTC/intc.o
ASF_OBJ += src/SOFTWARE_FRAMEWORK/DRIVERS/INTC/exception.o
ASF_OBJ += src/SOFTWARE_FRAMEWORK/DRIVERS/GPIO/gpio.o
ASF_OBJ += src/SOFTWARE_FRAMEWORK/DRIVERS/EIC/eic.o
ASF_OBJ += src/SOFTWARE_FRAMEWORK/BOARDS/EVK1100/led.o
ASF_OBJ += src/SOFTWARE_FRAMEWORK/ASM/trampoline.o
