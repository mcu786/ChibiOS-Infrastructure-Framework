/*
 * simplifiedNewOperator.cpp
 *
 *  Created on: Sep 21, 2011
 *      Author: mabl
 */

#include <stdlib.h>
#include <ch.h>

void *operator new(size_t size) {
    return chHeapAlloc(NULL, size);
}

void *operator new[](size_t size) {
  return chHeapAlloc(NULL, size);
}

void operator delete(void *p) {
    chHeapFree(p);
}

void operator delete[](void *p) {
  chHeapFree(p);
}


namespace __gnu_cxx {
  void __verbose_terminate_handler() {
    chSysHalt();
  }
}
