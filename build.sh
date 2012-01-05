gcc -I. \
	zeo_malloc.c \
	zeo_event.c \
	zeo_connection.c \
	zeo_packet.c \
	zeo_packet_loader.c \
	zeo_splice.c \
	zeo_splice_loader.c \
	sw_serial_port.c \
	sleep_waffle.c \
	main.c -g -o main
