BBB_SYSROT = ~/bbb-sysroot/ 
PRU_SWPKG = $(PRU_SUPPORT)
PRU_CGT = /home/dhiren/ti-cgt-pru_2.3.3

CC = clpru
LD = lnkpru
CFLAGS = --include_path=$(PRU_SWPKG)/include \
	 --include_path=$(PRU_SWPKG)/include/am335x \
	 --include_path=$(PRU_CGT)/include
LDFLAGS = $(PRU_SWPKG)/labs/Hands_on_Labs/lab_2/AM335x_PRU.cmd

all: am335x-pru0-fw

hello-pru0.o: hello-pru0.c
	$(CC) $(CFLAGS) $^ --output_file $@

am335x-pru0-fw: hello-pru0.o
	$(LD) $(LDFLAGS) $^ -o $@
