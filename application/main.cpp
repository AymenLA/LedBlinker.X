#include "System.h"
#include "AppCustomDevice.h"

MAIN_RETURN main(void)
{
    SYSTEM_Initialize(SYSTEM_STATE_START);

    while(1)
    {
        SYSTEM_Tasks();

        //Application specific tasks
        APP_DeviceTasks();

    }//end while
}//end main