SRC_DIR=./..

APPNAME = image
FULLNAME = $(OUT_DIR)/$(APPNAME).elf
BINNAME = $(OUT_DIR)/$(APPNAME).bin

include $(SRC_DIR)/rules.mk

CXXFLAGS += -I./ \
		-I$(SRC_DIR)/include \
		-I$(SRC_DIR)/libhardware \
		-I$(SRC_DIR)/libcmsis \
		-I$(SRC_DIR)/liblcd \
		-I$(SRC_DIR)/libdsp \
		-DUSE_STDPERIPH_DRIVER \
		$(APP_DEFS) \
		$(FREERTOS_DEFS) \
		$(HARDWARE_DEFS)
		
CFLAGS += -I./ \
		-I$(SRC_DIR)/include \
		-I$(SRC_DIR)/liblcd \
		-I$(SRC_DIR)/libhardware \
		-I$(SRC_DIR)/libcmsis \
		-I$(SRC_DIR)/libdsp \
		-DUSE_STDPERIPH_DRIVER \
		$(APP_DEFS) \
		$(FREERTOS_DEFS) \
		$(HARDWARE_DEFS)

LIBS =  -llcd -lhardware -lcmsis -ldsp -lm 

CUSTOM_RM_FILES=$(OUT_DIR)/image.*






