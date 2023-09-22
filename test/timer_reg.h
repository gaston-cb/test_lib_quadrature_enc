

/**
 * Copyright (c) 2021 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
// =============================================================================
// Register block : TIMER
// Version        : 1
// Bus type       : apb
// Description    : Controls time and alarms
//                  time is a 64 bit value indicating the time in usec since
//                  power-on
//                  timeh is the top 32 bits of time & timel is the bottom 32
//                  bits
//                  to change time write to timelw before timehw
//                  to read time read from timelr before timehr
//                  An alarm is set by setting alarm_enable and writing to the
//                  corresponding alarm register
//                  When an alarm is pending, the corresponding alarm_running
//                  signal will be high
//                  An alarm can be cancelled before it has finished by clearing
//                  the alarm_enable
//                  When an alarm fires, the corresponding alarm_irq is set and
//                  alarm_running is cleared
//                  To clear the interrupt write a 1 to the corresponding
//                  alarm_irq
// =============================================================================

// =============================================================================
// Register    : TIMER_TIMEHW
// Description : Write to bits 63:32 of time
//               always write timelw before timehw
#define _REG_(x)
//#define _u(x) x ## u

#define TIMER_TIMEHW_OFFSET _u(0x00000000)
#define TIMER_TIMEHW_BITS   _u(0xffffffff)
#define TIMER_TIMEHW_RESET  _u(0x00000000)
#define TIMER_TIMEHW_MSB    _u(31)
#define TIMER_TIMEHW_LSB    _u(0)
#define TIMER_TIMEHW_ACCESS "WF"
// =============================================================================
// Register    : TIMER_TIMELW
// Description : Write to bits 31:0 of time
//               writes do not get copied to time until timehw is written
#define TIMER_TIMELW_OFFSET _u(0x00000004)
#define TIMER_TIMELW_BITS   _u(0xffffffff)
#define TIMER_TIMELW_RESET  _u(0x00000000)
#define TIMER_TIMELW_MSB    _u(31)
#define TIMER_TIMELW_LSB    _u(0)
#define TIMER_TIMELW_ACCESS "WF"
// =============================================================================
// Register    : TIMER_TIMEHR
// Description : Read from bits 63:32 of time
//               always read timelr before timehr
#define TIMER_TIMEHR_OFFSET _u(0x00000008)
#define TIMER_TIMEHR_BITS   _u(0xffffffff)
#define TIMER_TIMEHR_RESET  _u(0x00000000)
#define TIMER_TIMEHR_MSB    _u(31)
#define TIMER_TIMEHR_LSB    _u(0)
#define TIMER_TIMEHR_ACCESS "RO"
#define TIMER_BASE 0x40054000u

// =============================================================================
// Register    : TIMER_TIMELR
// Description : Read from bits 31:0 of time
#define TIMER_TIMELR_OFFSET _u(0x0000000c)
#define TIMER_TIMELR_BITS   _u(0xffffffff)
#define TIMER_TIMELR_RESET  _u(0x00000000)
#define TIMER_TIMELR_MSB    _u(31)
#define TIMER_TIMELR_LSB    _u(0)
#define TIMER_TIMELR_ACCESS "RO"
// =============================================================================
// Register    : TIMER_ALARM0
// Description : Arm alarm 0, and configure the time it will fire.
//               Once armed, the alarm fires when TIMER_ALARM0 == TIMELR.
//               The alarm will disarm itself once it fires, and can
//               be disarmed early using the ARMED status register.
#define TIMER_ALARM0_OFFSET _u(0x00000010)
#define TIMER_ALARM0_BITS   _u(0xffffffff)
#define TIMER_ALARM0_RESET  _u(0x00000000)
#define TIMER_ALARM0_MSB    _u(31)
#define TIMER_ALARM0_LSB    _u(0)
#define TIMER_ALARM0_ACCESS "RW"
// =============================================================================
// Register    : TIMER_ALARM1
// Description : Arm alarm 1, and configure the time it will fire.
//               Once armed, the alarm fires when TIMER_ALARM1 == TIMELR.
//               The alarm will disarm itself once it fires, and can
//               be disarmed early using the ARMED status register.
#define TIMER_ALARM1_OFFSET _u(0x00000014)
#define TIMER_ALARM1_BITS   _u(0xffffffff)
#define TIMER_ALARM1_RESET  _u(0x00000000)
#define TIMER_ALARM1_MSB    _u(31)
#define TIMER_ALARM1_LSB    _u(0)
#define TIMER_ALARM1_ACCESS "RW"
// =============================================================================
// Register    : TIMER_ALARM2
// Description : Arm alarm 2, and configure the time it will fire.
//               Once armed, the alarm fires when TIMER_ALARM2 == TIMELR.
//               The alarm will disarm itself once it fires, and can
//               be disarmed early using the ARMED status register.
#define TIMER_ALARM2_OFFSET _u(0x00000018)
#define TIMER_ALARM2_BITS   _u(0xffffffff)
#define TIMER_ALARM2_RESET  _u(0x00000000)
#define TIMER_ALARM2_MSB    _u(31)
#define TIMER_ALARM2_LSB    _u(0)
#define TIMER_ALARM2_ACCESS "RW"
// =============================================================================
// Register    : TIMER_ALARM3
// Description : Arm alarm 3, and configure the time it will fire.
//               Once armed, the alarm fires when TIMER_ALARM3 == TIMELR.
//               The alarm will disarm itself once it fires, and can
//               be disarmed early using the ARMED status register.
#define TIMER_ALARM3_OFFSET _u(0x0000001c)
#define TIMER_ALARM3_BITS   _u(0xffffffff)
#define TIMER_ALARM3_RESET  _u(0x00000000)
#define TIMER_ALARM3_MSB    _u(31)
#define TIMER_ALARM3_LSB    _u(0)
#define TIMER_ALARM3_ACCESS "RW"
// =============================================================================
// Register    : TIMER_ARMED
// Description : Indicates the armed/disarmed status of each alarm.
//               A write to the corresponding ALARMx register arms the alarm.
//               Alarms automatically disarm upon firing, but writing ones here
//               will disarm immediately without waiting to fire.
#define TIMER_ARMED_OFFSET _u(0x00000020)
#define TIMER_ARMED_BITS   _u(0x0000000f)
#define TIMER_ARMED_RESET  _u(0x00000000)
#define TIMER_ARMED_MSB    _u(3)
#define TIMER_ARMED_LSB    _u(0)
#define TIMER_ARMED_ACCESS "WC"
// =============================================================================
// Register    : TIMER_TIMERAWH
// Description : Raw read from bits 63:32 of time (no side effects)
#define TIMER_TIMERAWH_OFFSET _u(0x00000024)
#define TIMER_TIMERAWH_BITS   _u(0xffffffff)
#define TIMER_TIMERAWH_RESET  _u(0x00000000)
#define TIMER_TIMERAWH_MSB    _u(31)
#define TIMER_TIMERAWH_LSB    _u(0)
#define TIMER_TIMERAWH_ACCESS "RO"
// =============================================================================
// Register    : TIMER_TIMERAWL
// Description : Raw read from bits 31:0 of time (no side effects)
#define TIMER_TIMERAWL_OFFSET _u(0x00000028)
#define TIMER_TIMERAWL_BITS   _u(0xffffffff)
#define TIMER_TIMERAWL_RESET  _u(0x00000000)
#define TIMER_TIMERAWL_MSB    _u(31)
#define TIMER_TIMERAWL_LSB    _u(0)
#define TIMER_TIMERAWL_ACCESS "RO"
// =============================================================================
// Register    : TIMER_DBGPAUSE
// Description : Set bits high to enable pause when the corresponding debug
//               ports are active
#define TIMER_DBGPAUSE_OFFSET _u(0x0000002c)
#define TIMER_DBGPAUSE_BITS   _u(0x00000006)
#define TIMER_DBGPAUSE_RESET  _u(0x00000007)
// -----------------------------------------------------------------------------
// Field       : TIMER_DBGPAUSE_DBG1
// Description : Pause when processor 1 is in debug mode
#define TIMER_DBGPAUSE_DBG1_RESET  _u(0x1)
#define TIMER_DBGPAUSE_DBG1_BITS   _u(0x00000004)
#define TIMER_DBGPAUSE_DBG1_MSB    _u(2)
#define TIMER_DBGPAUSE_DBG1_LSB    _u(2)
#define TIMER_DBGPAUSE_DBG1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : TIMER_DBGPAUSE_DBG0
// Description : Pause when processor 0 is in debug mode
#define TIMER_DBGPAUSE_DBG0_RESET  _u(0x1)
#define TIMER_DBGPAUSE_DBG0_BITS   _u(0x00000002)
#define TIMER_DBGPAUSE_DBG0_MSB    _u(1)
#define TIMER_DBGPAUSE_DBG0_LSB    _u(1)
#define TIMER_DBGPAUSE_DBG0_ACCESS "RW"
// =============================================================================
// Register    : TIMER_PAUSE
// Description : Set high to pause the timer
#define TIMER_PAUSE_OFFSET _u(0x00000030)
#define TIMER_PAUSE_BITS   _u(0x00000001)
#define TIMER_PAUSE_RESET  _u(0x00000000)
#define TIMER_PAUSE_MSB    _u(0)
#define TIMER_PAUSE_LSB    _u(0)
#define TIMER_PAUSE_ACCESS "RW"
// =============================================================================
// Register    : TIMER_INTR
// Description : Raw Interrupts
#define TIMER_INTR_OFFSET _u(0x00000034)
#define TIMER_INTR_BITS   _u(0x0000000f)
#define TIMER_INTR_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : TIMER_INTR_ALARM_3
// Description : None
#define TIMER_INTR_ALARM_3_RESET  _u(0x0)
#define TIMER_INTR_ALARM_3_BITS   _u(0x00000008)
#define TIMER_INTR_ALARM_3_MSB    _u(3)
#define TIMER_INTR_ALARM_3_LSB    _u(3)
#define TIMER_INTR_ALARM_3_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : TIMER_INTR_ALARM_2
// Description : None
#define TIMER_INTR_ALARM_2_RESET  _u(0x0)
#define TIMER_INTR_ALARM_2_BITS   _u(0x00000004)
#define TIMER_INTR_ALARM_2_MSB    _u(2)
#define TIMER_INTR_ALARM_2_LSB    _u(2)
#define TIMER_INTR_ALARM_2_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : TIMER_INTR_ALARM_1
// Description : None
#define TIMER_INTR_ALARM_1_RESET  _u(0x0)
#define TIMER_INTR_ALARM_1_BITS   _u(0x00000002)
#define TIMER_INTR_ALARM_1_MSB    _u(1)
#define TIMER_INTR_ALARM_1_LSB    _u(1)
#define TIMER_INTR_ALARM_1_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : TIMER_INTR_ALARM_0
// Description : None
#define TIMER_INTR_ALARM_0_RESET  _u(0x0)
#define TIMER_INTR_ALARM_0_BITS   _u(0x00000001)
#define TIMER_INTR_ALARM_0_MSB    _u(0)
#define TIMER_INTR_ALARM_0_LSB    _u(0)
#define TIMER_INTR_ALARM_0_ACCESS "WC"
// =============================================================================
// Register    : TIMER_INTE
// Description : Interrupt Enable
#define TIMER_INTE_OFFSET _u(0x00000038)
#define TIMER_INTE_BITS   _u(0x0000000f)
#define TIMER_INTE_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : TIMER_INTE_ALARM_3
// Description : None
#define TIMER_INTE_ALARM_3_RESET  _u(0x0)
#define TIMER_INTE_ALARM_3_BITS   _u(0x00000008)
#define TIMER_INTE_ALARM_3_MSB    _u(3)
#define TIMER_INTE_ALARM_3_LSB    _u(3)
#define TIMER_INTE_ALARM_3_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : TIMER_INTE_ALARM_2
// Description : None
#define TIMER_INTE_ALARM_2_RESET  _u(0x0)
#define TIMER_INTE_ALARM_2_BITS   _u(0x00000004)
#define TIMER_INTE_ALARM_2_MSB    _u(2)
#define TIMER_INTE_ALARM_2_LSB    _u(2)
#define TIMER_INTE_ALARM_2_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : TIMER_INTE_ALARM_1
// Description : None
#define TIMER_INTE_ALARM_1_RESET  _u(0x0)
#define TIMER_INTE_ALARM_1_BITS   _u(0x00000002)
#define TIMER_INTE_ALARM_1_MSB    _u(1)
#define TIMER_INTE_ALARM_1_LSB    _u(1)
#define TIMER_INTE_ALARM_1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : TIMER_INTE_ALARM_0
// Description : None
#define TIMER_INTE_ALARM_0_RESET  _u(0x0)
#define TIMER_INTE_ALARM_0_BITS   _u(0x00000001)
#define TIMER_INTE_ALARM_0_MSB    _u(0)
#define TIMER_INTE_ALARM_0_LSB    _u(0)
#define TIMER_INTE_ALARM_0_ACCESS "RW"
// =============================================================================
// Register    : TIMER_INTF
// Description : Interrupt Force
#define TIMER_INTF_OFFSET _u(0x0000003c)
#define TIMER_INTF_BITS   _u(0x0000000f)
#define TIMER_INTF_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : TIMER_INTF_ALARM_3
// Description : None
#define TIMER_INTF_ALARM_3_RESET  _u(0x0)
#define TIMER_INTF_ALARM_3_BITS   _u(0x00000008)
#define TIMER_INTF_ALARM_3_MSB    _u(3)
#define TIMER_INTF_ALARM_3_LSB    _u(3)
#define TIMER_INTF_ALARM_3_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : TIMER_INTF_ALARM_2
// Description : None
#define TIMER_INTF_ALARM_2_RESET  _u(0x0)
#define TIMER_INTF_ALARM_2_BITS   _u(0x00000004)
#define TIMER_INTF_ALARM_2_MSB    _u(2)
#define TIMER_INTF_ALARM_2_LSB    _u(2)
#define TIMER_INTF_ALARM_2_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : TIMER_INTF_ALARM_1
// Description : None
#define TIMER_INTF_ALARM_1_RESET  _u(0x0)
#define TIMER_INTF_ALARM_1_BITS   _u(0x00000002)
#define TIMER_INTF_ALARM_1_MSB    _u(1)
#define TIMER_INTF_ALARM_1_LSB    _u(1)
#define TIMER_INTF_ALARM_1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : TIMER_INTF_ALARM_0
// Description : None
#define TIMER_INTF_ALARM_0_RESET  _u(0x0)
#define TIMER_INTF_ALARM_0_BITS   _u(0x00000001)
#define TIMER_INTF_ALARM_0_MSB    _u(0)
#define TIMER_INTF_ALARM_0_LSB    _u(0)
#define TIMER_INTF_ALARM_0_ACCESS "RW"
// =============================================================================
// Register    : TIMER_INTS
// Description : Interrupt status after masking & forcing
#define TIMER_INTS_OFFSET _u(0x00000040)
#define TIMER_INTS_BITS   _u(0x0000000f)
#define TIMER_INTS_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : TIMER_INTS_ALARM_3
// Description : None
#define TIMER_INTS_ALARM_3_RESET  _u(0x0)
#define TIMER_INTS_ALARM_3_BITS   _u(0x00000008)
#define TIMER_INTS_ALARM_3_MSB    _u(3)
#define TIMER_INTS_ALARM_3_LSB    _u(3)
#define TIMER_INTS_ALARM_3_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TIMER_INTS_ALARM_2
// Description : None
#define TIMER_INTS_ALARM_2_RESET  _u(0x0)
#define TIMER_INTS_ALARM_2_BITS   _u(0x00000004)
#define TIMER_INTS_ALARM_2_MSB    _u(2)
#define TIMER_INTS_ALARM_2_LSB    _u(2)
#define TIMER_INTS_ALARM_2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TIMER_INTS_ALARM_1
// Description : None
#define TIMER_INTS_ALARM_1_RESET  _u(0x0)
#define TIMER_INTS_ALARM_1_BITS   _u(0x00000002)
#define TIMER_INTS_ALARM_1_MSB    _u(1)
#define TIMER_INTS_ALARM_1_LSB    _u(1)
#define TIMER_INTS_ALARM_1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TIMER_INTS_ALARM_0
// Description : None
#define TIMER_INTS_ALARM_0_RESET  _u(0x0)
#define TIMER_INTS_ALARM_0_BITS   _u(0x00000001)
#define TIMER_INTS_ALARM_0_MSB    _u(0)
#define TIMER_INTS_ALARM_0_LSB    _u(0)
#define TIMER_INTS_ALARM_0_ACCESS "RO"
// =============================================================================

typedef volatile uint32_t io_rw_32;
typedef volatile uint32_t io_wo_32;
typedef const volatile uint32_t io_ro_32;







typedef struct {
    _REG_(TIMER_TIMEHW_OFFSET) // TIMER_TIMEHW
    // Write to bits 63:32 of time
    io_wo_32 timehw;

    _REG_(TIMER_TIMELW_OFFSET) // TIMER_TIMELW
    // Write to bits 31:0 of time
    io_wo_32 timelw;

    _REG_(TIMER_TIMEHR_OFFSET) // TIMER_TIMEHR
    // Read from bits 63:32 of time
    io_ro_32 timehr;

    _REG_(TIMER_TIMELR_OFFSET) // TIMER_TIMELR
    // Read from bits 31:0 of time
    io_ro_32 timelr;

    _REG_(TIMER_ALARM0_OFFSET) // TIMER_ALARM0
    // (Description copied from array index 0 register TIMER_ALARM0 applies similarly to other array indexes)
    //
    // Arm alarm 0, and configure the time it will fire
    io_rw_32 alarm[4]; // 4

    _REG_(TIMER_ARMED_OFFSET) // TIMER_ARMED
    // Indicates the armed/disarmed status of each alarm
    // 0x0000000f [3:0]   : ARMED (0)
    io_rw_32 armed;

    _REG_(TIMER_TIMERAWH_OFFSET) // TIMER_TIMERAWH
    // Raw read from bits 63:32 of time (no side effects)
    io_ro_32 timerawh;

    _REG_(TIMER_TIMERAWL_OFFSET) // TIMER_TIMERAWL
    // Raw read from bits 31:0 of time (no side effects)
    io_ro_32 timerawl;

    _REG_(TIMER_DBGPAUSE_OFFSET) // TIMER_DBGPAUSE
    // Set bits high to enable pause when the corresponding debug ports are active
    // 0x00000004 [2]     : DBG1 (1): Pause when processor 1 is in debug mode
    // 0x00000002 [1]     : DBG0 (1): Pause when processor 0 is in debug mode
    io_rw_32 dbgpause;

    _REG_(TIMER_PAUSE_OFFSET) // TIMER_PAUSE
    // Set high to pause the timer
    // 0x00000001 [0]     : PAUSE (0)
    io_rw_32 pause;

    _REG_(TIMER_INTR_OFFSET) // TIMER_INTR
    // Raw Interrupts
    // 0x00000008 [3]     : ALARM_3 (0)
    // 0x00000004 [2]     : ALARM_2 (0)
    // 0x00000002 [1]     : ALARM_1 (0)
    // 0x00000001 [0]     : ALARM_0 (0)
    io_rw_32 intr;

    _REG_(TIMER_INTE_OFFSET) // TIMER_INTE
    // Interrupt Enable
    // 0x00000008 [3]     : ALARM_3 (0)
    // 0x00000004 [2]     : ALARM_2 (0)
    // 0x00000002 [1]     : ALARM_1 (0)
    // 0x00000001 [0]     : ALARM_0 (0)
    io_rw_32 inte;

    _REG_(TIMER_INTF_OFFSET) // TIMER_INTF
    // Interrupt Force
    // 0x00000008 [3]     : ALARM_3 (0)
    // 0x00000004 [2]     : ALARM_2 (0)
    // 0x00000002 [1]     : ALARM_1 (0)
    // 0x00000001 [0]     : ALARM_0 (0)
    io_rw_32 intf;

    _REG_(TIMER_INTS_OFFSET) // TIMER_INTS
    // Interrupt status after masking & forcing
    // 0x00000008 [3]     : ALARM_3 (0)
    // 0x00000004 [2]     : ALARM_2 (0)
    // 0x00000002 [1]     : ALARM_1 (0)
    // 0x00000001 [0]     : ALARM_0 (0)
    io_ro_32 ints;
} timer_hw_t;

#define timer_hw ((timer_hw_t *)TIMER_BASE)

