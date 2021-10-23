#include <stdio.h>
#include <iostream>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "hardware/clocks.h"
#include "pico/multicore.h"
#include "hardware/clocks.h"
#include "hardware/pll.h"

#include "display/include/ili9341.h"
#include "display/include/displayDriver.h"
#include "display/include/characterDisplay.h"

#include "./common/include/common.h"

/*
int64_t alarm_callback(alarm_id_t id, void *user_data) {
    // Put your timeout handler code in here
    return 0;
}
*/
uint8_t counter = 0;
void gpio_callback(uint gpio, uint32_t events)
{
    std::cout << "miso: " << gpio_get(4) << " cs: " << gpio_get(5) << " mosi: " << gpio_get(7) << " dc " << gpio_get(9) << std::endl;
    counter++;
    if (counter == 8)
    {
        std::cout << std::endl;
        counter = 0;
    }
}

void core1_entry()
{
    gpio_set_irq_enabled_with_callback(6, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);
    while (1)
    {
        sleep_ms(1000);
    }
}
/*
void resus_callback(void) {
    // Reconfigure PLL sys back to the default state of 1500 / 6 / 2 = 125MHz
    pll_init(pll_sys, 1, 400 * MHZ, 2, 1);

    // CLK SYS = PLL SYS (125MHz) / 1 = 125MHz
    clock_configure(clk_sys,
                    CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX,
                    CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS,
                    200 * MHZ,
                    200 * MHZ);

    // Reconfigure uart as clocks have changed
    stdio_init_all();
}
*/

int main()
{

    stdio_init_all();
    printf("Hello resus\n");
    /*
    clocks_enable_resus(&resus_callback);
    // Break PLL sys
    pll_deinit(pll_sys);
*/
    std::cout << "Test\n";
    sleep_ms(500);
    std::cout << "Test\n";
    COMMON* common = new COMMON();
    //multicore_launch_core1(core1_entry);
    /*
    DISPLAYDRIVER *driver = new DISPLAYDRIVER();
    
    driver->fillColor();
    sleep_ms(1000);
    std::cout<<"fekete\n";
    driver->fillColor(common->swap_bytes(0x0000));
    sleep_ms(1000);
    std::cout<<"feher\n";
    driver->fillColor(common->swap_bytes(0xFFFF));
    sleep_ms(1000);
    std::cout<<"piros\n";
    driver->fillColor(common->swap_bytes(0xF800));
    sleep_ms(1000);
    std::cout<<"Sarga\n";
    driver->fillColor(common->swap_bytes(0xFE60));
    sleep_ms(1000);
    std::cout<<"kek\n";
    driver->fillColor(common->swap_bytes(0x081F));
    */
    sleep_ms(1000);
    CHARACTERDISPLAY *charDriver = new CHARACTERDISPLAY( 0x0000 , 0xFFFF );
    charDriver->fillColor(common->swap_bytes(0x081F));
    charDriver->printLine("!!!!");
    charDriver->printLine("!!!!");
    charDriver->printLine("!!!!");
    charDriver->printLine("=");
    charDriver->printLine("almafa");


    while (1)
    {
        sleep_ms(5000);
        std::cout << "Sleeping\n";
    }

    return 0;
}

/*
int main()
{
    

    // Set up our UART
    uart_init(UART_ID, BAUD_RATE);
    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    

    // GPIO initialisation.
    // We will make this GPIO an input, and pull it up by default
    gpio_init(GPIO);
    gpio_set_dir(GPIO, GPIO_IN);
    gpio_pull_up(GPIO);
    

    // Example of using the HW divider. The pico_divider library provides a more user friendly set of APIs 
    // over the divider (and support for 64 bit divides), and of course by default regular C language integer
    // divisions are redirected thru that library, meaning you can just use C level `/` and `%` operators and
    // gain the benefits of the fast hardware divider.
    int32_t dividend = 123456;
    int32_t divisor = -321;
    // This is the recommended signed fast divider for general use.
    divmod_result_t result = hw_divider_divmod_s32(dividend, divisor);
    printf("%d/%d = %d remainder %d\n", dividend, divisor, to_quotient_s32(result), to_remainder_s32(result));
    // This is the recommended unsigned fast divider for general use.
    int32_t udividend = 123456;
    int32_t udivisor = 321;
    divmod_result_t uresult = hw_divider_divmod_u32(udividend, udivisor);
    printf("%d/%d = %d remainder %d\n", udividend, udivisor, to_quotient_u32(uresult), to_remainder_u32(uresult));

    // SPI initialisation. This example will use SPI at 1MHz.
    spi_init(SPI_PORT, 1000*1000);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_CS,   GPIO_FUNC_SIO);
    gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    
    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);
    



    // Interpolator example code
    interp_config cfg = interp_default_config();
    // Now use the various interpolator library functions for your use case
    // e.g. interp_config_clamp(&cfg, true);
    //      interp_config_shift(&cfg, 2);
    // Then set the config 
    interp_set_config(interp0, 0, &cfg);

    // Timer example code - This example fires off the callback after 2000ms
    add_alarm_in_ms(2000, alarm_callback, NULL, false);



    puts("Hello, world!");

    return 0;
}
*/
