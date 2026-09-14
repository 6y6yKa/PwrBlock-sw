#include "usbpd_task.h"

#include "dpm/dpm.h"
#include "usbpd_dpm_core.h"
#include "usbpd_hw_if.h"

#include "cmsis_os2.h"

void usbpd_init_task(void* argument)
{
    (void)argument;

    USBPD_HW_IF_GlobalHwInit();

    if (USBPD_OK != USBPD_DPM_InitCore()) {
        while (1) {}
    }

    if (USBPD_OK != USBPD_DPM_UserInit()) {
        while (1) {}
    }

    // USBPD spawns two related tasks CAD and PE
    USBPD_DPM_InitOS();

    osThreadId_t _current_thread_id = osThreadGetId();
    osThreadTerminate(_current_thread_id);
}
