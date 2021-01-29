# mac-hid-dump

`mac-hid-dump` is a small command-line app to output the __HID Report Descriptors__ of all connected HID devices.  Think of it as sort of a MacOS version of [`usbhid-dump`](https://github.com/DIGImend/usbhid-dump). It is designed to inspect HID devices for use with [`hidapi`](https://github.com/libusb/hidapi/), [`node-hid`](https://github.com/node-hid/node-hid), or similar HID libraries.  The output can be parsed directly by the [USB Descriptor Parser](https://eleccelerator.com/usbdescreqparser/).

`mac-hid-dump` is based on the Mac-specific fucntions in [libusb/hidapi](https://github.com/libusb/hidapi/), a cross-platform library for communicating with HID devies.


## Usage:

To use `mac-hid-dump`, download the zip file from the Releases page, unzip it, and run it.

When run, its output looks like the below.

```
tod@demo ~/Downloads/mac-hid-dump$ ./mac-hid-dump
mac-hid-dump:
16C0 0486:  - Teensyduino RawHID Device
DESCRIPTOR:
  06  c9  ff  09  04  a1  5c  75  08  15  00  26  ff  00  95  40
  09  75  81  02  95  20  09  76  91  02  95  04  09  76  b1  02
  c0
  (33 bytes)
16C0 0486:  - Teensyduino RawHID Device
DESCRIPTOR:
  06  ab  ff  0a  00  02  a1  01  75  08  15  00  26  ff  00  95
  40  09  01  81  02  95  40  09  02  91  02  c0
  (28 bytes)
27B8 01ED: ThingM - blink(1) mk3
DESCRIPTOR:
  06  ab  ff  0a  00  20  a1  01  15  00  26  ff  00  75  08  85
  01  95  08  09  00  b2  02  01  75  08  85  02  95  3c  09  00
  b2  02  01  c0
  (36 bytes)

```

This example shows a
[Teensy RawHID](https://www.pjrc.com/teensy/rawhid.html) and a [blink(1) USB LED](https://blink1.thingm.com/).  The Teensy RawHID descriptor shows a simple "RawHID" example of a single 64-byte report using no reportIDs for both Input and Output. The blink(1) descriptor shows an example that defines two reportID-based Feature reports: an 8-byte one and a 60-byte one.
