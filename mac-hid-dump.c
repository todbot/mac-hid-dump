/* 
 * mac-hid-dump -- Dump HID Report Descriptors on MacOS
 *                 Sort of a MacOS version of `usbhid-dump`
 *
 * Borrows heavily from libusb/hidapi/mac/hid.c
 *
 * 2021, Tod Kurt, tod@todbot.com
 */

#include <IOKit/hid/IOHIDManager.h>
#include <IOKit/hid/IOHIDKeys.h>
#include <IOKit/IOKitLib.h>
#include <CoreFoundation/CoreFoundation.h>

static int32_t get_int_property(IOHIDDeviceRef device, CFStringRef key)
{
	CFTypeRef ref;
	int32_t value;

	ref = IOHIDDeviceGetProperty(device, key);
	if (ref) {
		if (CFGetTypeID(ref) == CFNumberGetTypeID()) {
			CFNumberGetValue((CFNumberRef) ref, kCFNumberSInt32Type, &value);
			return value;
		}
	}
	return 0;
}

static int get_string_property(IOHIDDeviceRef device, CFStringRef prop, wchar_t *buf, size_t len)
{
	CFStringRef str;

	if (!len)
		return 0;

	str = (CFStringRef) IOHIDDeviceGetProperty(device, prop);

	buf[0] = 0;

	if (str) {
		CFIndex str_len = CFStringGetLength(str);
		CFRange range;
		CFIndex used_buf_len;
		CFIndex chars_copied;

		len --;

		range.location = 0;
		range.length = ((size_t) str_len > len)? len: (size_t) str_len;
		chars_copied = CFStringGetBytes(str,
			range,
			kCFStringEncodingUTF32LE,
			(char) '?',
			FALSE,
			(UInt8*)buf,
			len * sizeof(wchar_t),
			&used_buf_len);

		if (chars_copied <= 0)
			buf[0] = 0;
		else
			buf[chars_copied] = 0;

		return 0;
	}
	else
		return -1;
}

static unsigned short get_vendor_id(IOHIDDeviceRef device)
{
	return get_int_property(device, CFSTR(kIOHIDVendorIDKey));
}

static unsigned short get_product_id(IOHIDDeviceRef device)
{
	return get_int_property(device, CFSTR(kIOHIDProductIDKey));
}

static int get_serial_number(IOHIDDeviceRef device, wchar_t *buf, size_t len)
{
	return get_string_property(device, CFSTR(kIOHIDSerialNumberKey), buf, len);
}

static int get_manufacturer_string(IOHIDDeviceRef device, wchar_t *buf, size_t len)
{
	return get_string_property(device, CFSTR(kIOHIDManufacturerKey), buf, len);
}

static int get_product_string(IOHIDDeviceRef device, wchar_t *buf, size_t len)
{
	return get_string_property(device, CFSTR(kIOHIDProductKey), buf, len);
}

int main(void)
{    
	IOHIDManagerRef mgr;
    int i;
    
	mgr = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
	IOHIDManagerSetDeviceMatching(mgr, NULL);
	IOHIDManagerOpen(mgr, kIOHIDOptionsTypeNone);

	CFSetRef device_set = IOHIDManagerCopyDevices(mgr);

	CFIndex num_devices = CFSetGetCount(device_set);
	IOHIDDeviceRef *device_array = calloc(num_devices, sizeof(IOHIDDeviceRef));
	CFSetGetValues(device_set, (const void **) device_array);

    //printf("got %ld devices\n", num_devices);
    
	for (i = 0; i < num_devices; i++) {
		IOHIDDeviceRef dev = device_array[i];
		wchar_t str1[256], str2[256];

        get_manufacturer_string( dev, str1, sizeof(str1) );
        get_product_string( dev, str2, sizeof(str2) );
        printf("%04hX %04hX: %ls - %ls\n",
               get_vendor_id(dev), get_product_id(dev), str1, str2);

        CFDataRef cfprop = (CFDataRef)IOHIDDeviceGetProperty(dev,
                                                             CFSTR(kIOHIDReportDescriptorKey));
        printf("DESCRIPTOR:\n  ");
        uint8_t pbuf[1024];
        if( cfprop != NULL) {
            long len = CFDataGetLength(cfprop);
            CFDataGetBytes( cfprop, CFRangeMake(0,len), pbuf);
            for( int i=0; i< len; i++) {
                printf("%02x ", pbuf[i]);
                printf( (i % 16 == 15) ? "\n  " : " ");
            }
            printf("\n  (%ld bytes)\n", len);
        }
    }

	return 0;
}
