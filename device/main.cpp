#include <stdio.h>
#include "CpuDevice.h"
//#include "SerialDevice.h"
//#include "WatchdogDevice.h"
// #include "GpioDevice.h"
#include "MySerial.h"
#include "Instance.h"

int main(int argc, char *argv[])
{
    //MySerial::getInstance();
    Instance<SerialDevice>::getInstance();
    return 0;
};
