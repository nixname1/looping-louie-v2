#include <stdio.h>
#include <stdlib.h>

#include "stm32f4xx.h"

#include "diag/Trace.h"

#include "ll_system.h"
#include "ll_motor.h"

int
main (int argc, char* argv[])
{
  UNUSED(argc);
  UNUSED(argv);

  ll_system_init();
  ll_system_game_start();

  trace_printf("SystemCoreClock: %lu\n", SystemCoreClock);

//  while (1)
//  {
//	switch()
//	{
//	  case
//
//	  default:
//
//	}

  while(1);

}
