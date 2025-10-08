# Project structure:
# project/
# ├── src/          (source files)
# ├── include/      (header files)
# ├── build/        (build outputs)
# └── Makefile

BBB_SYSROT = ~/bbb-sysroot/ 
PRU_SWPKG = /usr/lib/ti/pru-software-support-package-v6.0
PRU_CGT = /usr/share/ti/cgt-pru
PRU=0
TARGET=three_phase_waveform_generator
LNKFILE=$(PRU_SWPKG)/labs/Hands_on_Labs/lab_2/AM335x_PRU.cmd

# Directories
SRC_DIR=src
BUILD_DIR=build
INCLUDE_DIR=include

CC = clpru
LD = lnkpru
CFLAGS = --include_path=$(PRU_SWPKG)/include \
	 --include_path=$(PRU_SWPKG)/include/am335x 
	 --include_path=$(INCLUDE_DIR)

LDFLAGS=-z -i/usr/share/ti/cgt-pru/lib \
        -i/usr/share/ti/cgt-pru/include

# Source and output files
SRC=$(SRC_DIR)/$(TARGET).c
OUT=$(BUILD_DIR)/$(TARGET).out

all: $(BUILD_DIR) $(OUT)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $< $(LDFLAGS) $(LNKFILE) -o $@

install: $(OUT)
	sudo cp $(OUT) /lib/firmware/am335x-pru$(PRU)-fw

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all install clean