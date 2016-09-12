all:
	make -j2 all -C libcmsis
	make -j2 all -C libhardware
	make -j2 all -C liblcd
	make -j2 all -C application

clean:
	make clean -C libcmsis
	make clean -C libhardware
	make clean -C liblcd
	make clean -C application

app_all:
	make all -C application

app_clean:
	make clean -C application

# управление библиотекой cmsis
cmsis_all:
	make all -C libcmsis

cmsis_clean:
	make clean -C libcmsis

# управление библиотекой hardware
hardware_all:
	make all -C libhardware

hardware_clean:
	make clean -C libhardware

# управление библиотекой lcd
lcd_all:
	make all -C liblcd

lcd_clean:
	make clean -C liblcd
dsp_all:
	make all -C libdsp

dsp_clean:
	make clean -C libdsp
