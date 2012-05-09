#!/bin/bash
cat $1 | grep "__stacks_total_size__ = (__main_stack_size__ + __process_stack_size__)"
STACK=`cat $1 | grep "__stacks_total_size__ = (__main_stack_size__ + __process_stack_size__)" | awk '{print$1}'`

cat $1 | grep "__ram_start__ ="
RAM_START=`cat $1 | grep "__ram_start__ =" | awk '{print$1}'`

cat $1 | grep "__ram_size__ ="
RAM_MCU=`cat $1 | grep "__ram_size__ =" | awk '{print$1}'`

cat $1 | grep ".data" | grep "load address"
cat $1 | grep ".bss" | grep "load address"

cat $1 | grep "_end = ."
PROGRAM=`cat $1 | grep "_end = ." | awk '{print$1}'`

cat $1 | grep "__heap_end__ = (__ram_end__ - __stacks_total_size__)"

echo "------------------------------------------------------------------------------------------------------------"

let "STACK = STACK"
let "RAM_MCU = RAM_MCU"
let "PROGRAM = PROGRAM - RAM_START"
let "RAM_USED = PROGRAM + STACK"
let "RAM_PERCENT = (100 * RAM_USED)/RAM_MCU"

echo "SRAM:  stacks = $STACK, program = $PROGRAM, total = $RAM_USED, mcu_ram = $RAM_MCU, utilization = $RAM_PERCENT%"