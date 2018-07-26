#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define IN  0
#define OUT 1

#define LOW  0
#define HIGH 1

#define PIN 969
#define POUT 968

#define VALUE_MAX 30
#define BUFFER_MAX 3

#define MAX_PATH 64


static int pin_export(int pin)
{
	char shell[MAX_PATH];
	sprintf(shell,"echo %d > /sys/class/gpio/export", pin);
	system(shell);
	return 0;
}

static int pin_unexport(int pin)
{
        char shell[MAX_PATH];
        sprintf(shell,"echo %d > /sys/class/gpio/unexport", pin);
        system(shell);

	return 0;
}

static int pin_direction(int pin, int dir){

	char shell[MAX_PATH];
	snprintf(shell, MAX_PATH, "echo %s > /sys/class/gpio/gpio%d/direction",((dir==IN)?"in":"out"),pin);
	system(shell);

	return 0;
}

static int pin_read(int pin){

	char path[VALUE_MAX];
	char value_str[3];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);

	// get pin file descriptor for reading its state
	fd = open(path, O_RDONLY);
	if (-1 == fd) {
		fprintf(stderr, "Unable to open gpio sysfs pin value file %s for reading\n",path);
		return -1;
	}

	// read value
	if (-1 == read(fd, value_str, 3)) {
		fprintf(stderr, "Unable to read value\n");
		return -1;
	}

	// close file
	close(fd);

	// return integar value
	return atoi(value_str);
}

static int pin_write(int pin, int value)
{
	char path[VALUE_MAX];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	// get pin value file descrptor
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Unable to to open sysfs pins value file %s for writing\n",path);
		return -1;
	}
	if(value==LOW){
		//write low
		if (1 != write(fd, "0", 1)) {
			fprintf(stderr, "Unable to write value\n");
			return -1;
		}
	}
        else if(value==HIGH){
		//write high
		if (1 != write(fd, "1", 1)) {
                	fprintf(stderr, "Unable to write value\n");
                	return -1;
		}
	}else fprintf(stderr, "Nonvalid pin value requested\n");

	//close file
	close(fd);
	return 0;
}

int main(int argc, char *argv[])
{
	// apply required fpga bitstream mercury or classic
	// system("cat /opt/redpitaya/fpga/fpga_classic.bit > /dev/xdevcfg");
	
	// int repeat = 10;
	// if (argc==2)
		// repeat=atoi(argv[1]);
	// export pins
	if (-1 == pin_export(POUT) || -1 == pin_export(PIN)) return 1;

	// set pin direction
	if (-1 == pin_direction(POUT, OUT) || -1 == pin_direction(PIN, IN)) return 2;
	int i, flag=1;
	// for (i = 1; i <= repeat; i++){
		
		// if (-1 == pin_write( POUT, i % 2)) return 3;
		// printf("Setting pin %d to %d\n", POUT,i % 2);
		
		// printf("Reading pin %d got %d\n", PIN,pin_read(PIN));
		// usleep(10000);
	// }
	pin_write( POUT, 0);
	while(1)
	{
		i = pin_read(PIN);
		if(i==1 && flag==1)
		{
			flag = 0;
			pin_write( POUT, 1);
			// printf("enter1\n");
		}
		else if(i==1 && flag==0)
		{
			pin_write( POUT, 0);
			// printf("enter2\n");
		}
		else 
		{
			pin_write( POUT, 0);
			// printf("enter3\n");
			flag = 1;
		}
		// printf("Reading %d\n",i);
		
	}
	// unexport pins on exit
	if (-1 == pin_unexport(POUT) || -1 == pin_unexport(PIN))
		return 4;
	return 0;
}
