/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ch.h"
#include "hal.h"
#include "test.h"

#include "lwipthread.h"

#include "Framework/Notifications/notifier.hpp"

#include "Modules/BlinkingLight/blinkingLight.hpp"
#include "Modules/SimpleWebserver/simpleWebserver.hpp"


/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /*
   * Activates the serial driver 3 using the driver default configuration.
   */
  sdStart(&SD3, NULL);

  /*
   * Creates the LWIP threads (it changes priority internally).
   */
  chThdCreateStatic(wa_lwip_thread, LWIP_THREAD_STACK_SIZE, NORMALPRIO + 1,
                    lwip_thread, NULL);

  /*
   * Start and initialize Modules
   *
   * TODO: Move module init to a specific place and make it more generic
   * TODO: Also check returned error codes
   */

  VMODULE_GET_MODULE_REF(SimpleWebserver).init();
  VMODULE_GET_MODULE_REF(BlinkingLight).init();

  VMODULE_GET_MODULE_REF(SimpleWebserver).start();
  VMODULE_GET_MODULE_REF(BlinkingLight).start();

  /*
   * Some basic testing of notifiers.
   */
  fwk::StaticNotifier<int, 3> notifier;
  fwk::StaticListener<int, 2> listener;

  notifier.registerListener(listener);

  int a = 123;
  notifier.broadcast(a);

  a = 321;
  notifier.broadcast(a);

  chDbgAssert(listener.getSize() == 2, "bad message count", "");

  int b;
  b = *listener.getData();
  listener.releaseData();

  b = *listener.getData();
  listener.releaseData();

  notifier.unregisterListener(listener);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state.
   */
  while (TRUE) {
    if (palReadPad(GPIOC, GPIOC_SWITCH_TAMPER) == 0)
      TestThread(&SD3);
    chThdSleepMilliseconds(500);
  }
}
