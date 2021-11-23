#include "./include/BaseSwithcController.h"
#include "../Global.h"
#include <iostream>
#include "pico/multicore.h"

BASESWITCHCONTROLLER::BASESWITCHCONTROLLER(IASWITCH *sw1, IASWITCH *sw2, IASWITCH *sw3)
    : aswitch1(sw1), aswitch2(sw2), aswitch3(sw3)
{
}

BASESWITCHCONTROLLER::~BASESWITCHCONTROLLER()
{
    delete aswitch1;
    delete aswitch2;
    delete aswitch3;
}
/*
void BASESWITCHCONTROLLER::getSems()
{
    this->startSemaphore = multicore_fifo_pop_blocking();
    this->doneSemaphore = multicore_fifo_pop_blocking();
}
*/

void BASESWITCHCONTROLLER::SameOut3ChannelRepeat(uint8_t sw1P, uint8_t sw2P, uint8_t sw3P)
{
    std::cout << "start BaseController\n";
    //todo input chech, no short circuits
    for (int i = 0; i < 3; i++)
    {
        //std::cout << "queeBase" << queue_get_level(&ADCSelect_queue);
        //queue_try_add(&ADCSelect_queue, &i);
        multicore_fifo_push_blocking(i);
        std::cout << "start BaseController1\n";
        aswitch1->selectOutput(sw1P);
        std::cout << "start BaseController2\n";
        aswitch2->selectOutput(sw2P);
        std::cout << "start BaseController3\n";
        aswitch3->selectOutput(sw3P);
        std::cout << "start BaseController4\n";
        std::cout << &startSemaphore1 << std::endl;
        std::cout << &doneSemaphore1 << std::endl;
        startSemaphore -= 1;
        doneSemaphore -= 1;
        std::cout << &startSemaphore1 << std::endl;
        std::cout << &doneSemaphore1 << std::endl;
        std::cout << sem_available(startSemaphore1);
        std::cout << "Sem Base " << sem_release(startSemaphore1);
        std::cout << sem_available(startSemaphore1);
        std::cout << "start BaseController5\n";
        sem_acquire_blocking(doneSemaphore1);
        std::cout << "start BaseController6\n";
        //drain
        aswitch1->selectOutput(5);
        std::cout << "start BaseController7\n";
        aswitch2->selectOutput(5);
        std::cout << "start BaseController8\n";
        aswitch3->selectOutput(5);
        std::cout << "start BaseController\n";
        sleep_ms(3000);
    }
}
