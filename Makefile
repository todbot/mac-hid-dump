#
# Makefile for 'mac-hid-dump'
# 2021 Tod Kurt, todbot.com
#

all: mac-hid-dump

LIBS=-framework IOKit -framework CoreFoundation 

mac-hid-dump: mac-hid-dump.c
	$(CC) $(CFLAGS) mac-hid-dump.c -o mac-hid-dump $(LIBS)

clean:
	rm mac-hid-dump

