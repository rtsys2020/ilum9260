#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>


int main (void)
{
	int file;
	int adapter_nr = 0;
	char filename[20];
	snprintf(filename, sizeof(filename), "/dev/i2c-%d", adapter_nr);
	file = open(filename, O_RDWR);
	if (file < 0) {
		perror("Could not open device");
		exit(1);
	}
	if (ioctl(file, I2C_SLAVE, 0) < 0) {
		perror("Could not set I2C_SLAVE");
		exit(2);
	}
	__s32 v = 0xdeadbeef;
	v = i2c_smbus_read_byte_data(file);
	if (v < 0) {
		perror("i2c_smbus_read_word failed (2)");
		exit(3);
	}
	printf("Done!!!");
	return 0;
}

