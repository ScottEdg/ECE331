#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <bcm2835.h>
#include <unistd.h>
#include <stdlib.h>

#define CLK 25
#define BLUE 24
#define GREEN 23
#define RED 22

int main(int argc, char * argv[])
{
        if (argc != 4){
                printf("error:  please send correct RGB format\n");
                return -1;
        }

        int r = atoi(argv[1]);
        int g = atoi(argv[2]);
        int b = atoi(argv[3]);


        if (r < 0|| r > 2047 || b < 0 || b > 2047 || g < 0 || g > 2047){ //check the inputs

                printf("error:  Please enter a value between 0 and 2047\n");
                return -1;
        }
        r = ~r; //then complement them
        b = ~b;
        g = ~g;

        int check;
        int i;
        if((check=bcm2835_init()) == 0){
                printf("error:  bcm library not initialized\n");
                return -1;
        }

        bcm2835_gpio_clr(CLK); //initialize the clock to zero
        bcm2835_gpio_clr(BLUE); //initialize the BLUE GPIO pin to zero
        bcm2835_gpio_clr(GREEN); //initialize the GREEN GPIO pin to zero
        bcm2835_gpio_clr(22); //initialize the RED GPIO pin to zero

        bcm2835_gpio_fsel(CLK,BCM2835_GPIO_FSEL_OUTP); //set GPIO25 as an output
        bcm2835_gpio_fsel(BLUE,BCM2835_GPIO_FSEL_OUTP); //set GPIO24 as an output
        bcm2835_gpio_fsel(GREEN,BCM2835_GPIO_FSEL_OUTP); //set GPIO23 as an output
        bcm2835_gpio_fsel(RED,BCM2835_GPIO_FSEL_OUTP); //set GPIO22 as an output

        for(i=0;i<    11;i++){  //delay
                bcm2835_gpio_write(RED, (r >> (10-i)) & 1);
                bcm2835_gpio_write(BLUE, (b >> (10-i)) & 1);
                bcm2835_gpio_write(GREEN, (g >> (10-i)) & 1); //set appropriate bit

                bcm2835_gpio_set(CLK);  //set clock high
                bcm2835_delayMicroseconds(15);

                bcm2835_gpio_clr(CLK);  //set clock low
                bcm2835_delayMicroseconds(15);
        }
        bcm2835_close();
        return 0;
}
