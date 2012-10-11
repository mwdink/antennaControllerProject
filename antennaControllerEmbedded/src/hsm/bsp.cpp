/*****************************************************************************/
/** \file
    bsp.cpp

!PURPOSE:       interface between hardware and state machines
!SUBSYSTEM:
!DEPENDENCIES, LIMITATIONS & DESIGN NOTES:

!REVISION:
    \verbatim
    $File:  $
    $Change:  $
    $Revision:  $
    $DateTime:  $
    $Author:  $
    \endverbatim

<em> </em>
***************************************************************************/
/*----------------------------- Nested includes -----------------------------*/
#include "qp_port.h"
#include "stdio.h"
#include "wiring.h"
#include "bsp.h"
#include "switchControl.hpp"
#include "realStorage.hpp"
#include "realDisplay.hpp"

/*----------------- Symbolic Constants and Macros (defines) -----------------*/
Q_DEFINE_THIS_FILE
#define TICK_DIVIDER       ((F_CPU / BSP_TICKS_PER_SEC / 1024) - 1)

/*-------------------------- Typedefs and structs ---------------------------*/
/*----------------------- Declarations (externs only) -----------------------*/
/*------------------------------ Declarations -------------------------------*/
uint8_t l_TIMER2_COMPA;
uint32_t tickCount = 0;
int16_t currentCount = 0;

uint32_t lastInterruptTick = 0;
const uint32_t ticksFor20mSec = 2;              /* 20 msec */
const uint32_t ticksFor30mSec = 3;              /* 30 msec */
const uint32_t ticksFor100mSec = 10;            /* 100 msec */

/* this needs to be an external interrupt supported pin */
const uint8_t reedRelay =   2;      //Arduino digital pin 2
//note: interrupt 0 is digital pin 2
const uint8_t interrupt0 = 0;
const uint8_t onBoardLED = 13;      //Arduino digital pin 13

RealSwitches realSwitches;
RealMotor realMotor;
RealStorage realStorage;
RealDisplay *pRealDisplay;

void reedRelayISR(void);
void processSwitches(void);
void processCountUpdate(void);
void initializeDisplay(void);
void toggleTestLED(void);

/*---------------------------------- Functions ------------------------------*/


//Timer interrupt running at 10mS rate
/*!Function         ISR(TIMER2_COMPA_vect)
*   \param          void
*   \return         void
*   \par Purpose    timer Interrupt Service Routine
*   \note           No need to clear the interrupt source since
*                   the Timer2 compare interrupt is automatically
*                   cleared in hardware when the ISR runs
*/
ISR(TIMER2_COMPA_vect)
{

    // process all armed time events
    QP::QF::TICK(&l_TIMER2_COMPA);

    ++tickCount;

    //process any reed relay closure if it's time
    if(lastInterruptTick > 0)
    {
        --lastInterruptTick;
        if(0 == lastInterruptTick)
        {
            if(!digitalRead(reedRelay))
            {
                //do this
                //pMotorEvent->sig = (QP::QSignal)(MOTOR_MAGNET_SIG);
                //motorControlHsmObj->dispatch(pMotorEvent);
                BSP_addMotorEvent(MOTOR_MAGNET_SIG);
            }
        }
    }
}



/*!Function         reedRelayISR
*   \param          void
*   \return         void
*   \par Purpose    Interrupt Service Routine for reed relay closures
*   \note           set timer when reed relay closes, MOTOR_MAGNET_SIG
*                   event will be delayed until reed relay has been
*                   stable 10-20mSec
*/
void
reedRelayISR(void)
{
    //setup an event to occur once the reed relay has quit bouncing
    lastInterruptTick = ticksFor20mSec;
}


/*!Function         BSP_init
*   \param          void
*   \return         void
*   \par Purpose    perform system shutdown activities
*/
void BSP_init(void)
{
    DDRB  = 0xFF;   // All PORTB pins are outputs (user LED)
    PORTB = 0x00;   // drive all pins low

    if (QS_INIT((void *)0) == 0)
    {   // initialize the QS software tracing
        Q_ERROR();
    }

    QS_OBJ_DICTIONARY(&l_SysTick_Handler);
}


/*!Function         BSP_display
*   \param          char const *msg
*   \return         void
*   \par Purpose    wrapper for printf
*/
void
BSP_display(char const *msg)
{
    printf(msg);
}

/*!Function         BSP_display
*   \param          char *msg
*   \return         void
*   \par Purpose    wrapper for printf
*/
void
BSP_display(char *msg)
{
    printf(msg);
}

/*!Function         BSP_exit
*   \param          void
*   \return         void
*   \par Purpose    perform system shutdown activities
*/
void
BSP_exit(void)
{
    //none
}

/*!Function         QP::QF::onStartup
*   \param          void
*   \return         void
*   \par Purpose    called from hsm, perform system
*                   startup activities
*/
void
QP::QF::onStartup(void)
{
    // set Timer2 in CTC mode, 1/1024 prescaler, start the timer ticking
    TCCR2A = (1 << WGM21) | (0 << WGM20);
    TCCR2B = (1 << CS22 ) | (1 << CS21) | (1 << CS20);  // 1/2^10
    ASSR &= ~(1 << AS2);
    TIMSK2 = (1 << OCIE2A);   //Enable TIMER2 compare Interrupt
    TCNT2 = 0;
    OCR2A = TICK_DIVIDER;     //must be loaded last for Atmega168 and friends

    attachInterrupt(interrupt0, reedRelayISR, FALLING);
    realSwitches.initializeSwitchHardware();
    realMotor.initializeMotorHardware();
    currentCount = pMotorControlObj->setMotorPulseCount(realStorage.getStoredCount());
}


/*!Function         QP::QF::onCleanup
*   \param          void
*   \return         void
*   \par Purpose    not used
*/
void
QP::QF::onCleanup(void)
{
}

/*!Function         QP::QF::onIdle
*   \param          void
*   \return         void
*   \par Purpose    idle processing loop
*/
void
QP::QF::onIdle()
{
    static uint32_t lastTickCount = 0;
    static bool displayInitialized = false;

    BSP_dispatchSwitchEvent();
    BSP_dispatchMotorEvent();

    if(tickCount != lastTickCount)
    {
        lastTickCount = tickCount;

        if(false == displayInitialized)
        {
            if(tickCount > ticksFor100mSec)
            {
                initializeDisplay();
                displayInitialized = true;
            }
        }

        //process switches every 30 mSeconds
        if(0 == (tickCount%ticksFor30mSec))
        {
            processSwitches();
        }

        //process count update every 100 mSeconds
        if(0 == (tickCount%ticksFor100mSec))
        {
            processCountUpdate();
        }



//      if(0 == (tickCount%ticksFor500mSec))
//      {
//          realHardware.toggleTestPin();
//      }
    }



#ifdef SAVE_POWER

    SMCR = (0 << SM0) | (1 << SE);  // idle sleep mode, adjust to your project

    // never separate the following tw assembly instructions, see NOTE2
    __asm__ __volatile__ ("sei" "\n\t" :: );
    __asm__ __volatile__ ("sleep" "\n\t" :: );

    SMCR = 0;                                              // clear the SE bit

#else
    QF_INT_ENABLE();
#endif
}



//............................................................................
void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    QF_INT_DISABLE();                                // disable all interrupts
    //USER_LED_ON();                                  // User LED permanently ON
    asm volatile ("jmp 0x0000");    // perform a software reset of the Arduino
}

//////////////////////////////////////////////////////////////////////////////
// NOTE:
// The QF_onIdle() callback is called with interrupts *locked* to prevent
// a race condtion of posting a new event from an interrupt while the
// system is already committed to go to sleep. The only *safe* way of
// going to sleep mode is to do it ATOMICALLY with enabling interrupts.
// As described in the "AVR Datasheet" in Section "Reset and Interrupt
// Handling", when using the SEI instruction to enable interrupts, the
// instruction following SEI will be executed before any pending interrupts.
// As the Datasheet shows in the assembly example, the pair of instructions
//     SEI       ; enable interrupts
//     SLEEP     ; go to the sleep mode
// executes ATOMICALLY, and so *no* interrupt can be serviced between these
// instructions. You should NEVER separate these two lines.
//


/*!Function         BSP_addSwitchEvent
*   \param          uint16_t sig
*   \return         void
*   \par Purpose    add to the switch event queue
*/
void
BSP_addSwitchEvent(uint16_t sig)
{
    if(IGNORE_SIG == switchEventSigPool[switchEventHead])
    {
        switchEventSigPool[switchEventHead] = sig;
        ++switchEventHead;
        if(MAXEVENTS == switchEventHead)
        {
            switchEventHead = 0;
        }
    }
}

/*!Function         BSP_addMotorEvent
*   \param          uint16_t sig
*   \return         void
*   \par Purpose    add to the motor event queue
*/
void
BSP_addMotorEvent(uint16_t sig)
{
    if(IGNORE_SIG == motorEventSigPool[motorEventHead])
    {
        motorEventSigPool[motorEventHead] = sig;
        ++motorEventHead;
        if(MAXEVENTS == motorEventHead)
        {
            motorEventHead = 0;
        }
    }
}

/*!Function         BSP_dispatchSwitchEvent
*   \param          void
*   \return         void
*   \par Purpose    process any switch events that have queued up
*/
void
BSP_dispatchSwitchEvent(void)
{
    if(IGNORE_SIG != switchEventSigPool[switchEventTail])
    {
        pSwitchEvent->sig = (QP::QSignal)switchEventSigPool[switchEventTail];
        switchControlHsmObj->dispatch(pSwitchEvent);
        switchEventSigPool[switchEventTail] = IGNORE_SIG;
        ++switchEventTail;
        if(MAXEVENTS == switchEventTail)
        {
            switchEventTail = 0;
        }
    }
}

/*!Function         BSP_dispatchMotorEvent
*   \param          void
*   \return         void
*   \par Purpose    process any motor events that have queued up
*/
void
BSP_dispatchMotorEvent(void)
{
    if(IGNORE_SIG != motorEventSigPool[motorEventTail])
    {
        pSwitchEvent->sig = (QP::QSignal)motorEventSigPool[motorEventTail];
        motorControlHsmObj->dispatch(pSwitchEvent);

        if(MOTOR_MAGNET_SIG == motorEventSigPool[motorEventTail])
        {
            toggleTestLED();
        }

        motorEventSigPool[motorEventTail] = IGNORE_SIG;
        ++motorEventTail;
        if(MAXEVENTS == motorEventTail)
        {
            motorEventTail = 0;
        }
    }
}


/*!Function         processSwitches
*   \param          void
*   \return         void
*   \par Purpose    check the switches for activity
*/
void
processSwitches(void)
{
    static int16_t lastMotorControlSwitchStatus = MOTOR_SWITCH_RELEASED;
    static int16_t lastResetCountSwitchStatus = RESET_SWITCH_RELEASED;
    int16_t motorSwitchStatus = realSwitches.getMotorControlSwitches();

    //process motor control switches
    if(lastMotorControlSwitchStatus != motorSwitchStatus)
    {
        lastMotorControlSwitchStatus = motorSwitchStatus;
        switch(motorSwitchStatus)
        {
        case MOTOR_SWITCH_UP:
            BSP_addSwitchEvent(SWITCH_UP_PUSHED_SIG);
            break;
        case MOTOR_SWITCH_DOWN:
            BSP_addSwitchEvent(SWITCH_DOWN_PUSHED_SIG);
            break;
        default:
            BSP_addSwitchEvent(SWITCH_RELEASED_SIG);
            break;
        }
    }

    //process reset count switch
    int16_t resetSwitchStatus = realSwitches.getResetCountSwitch();
    if(lastResetCountSwitchStatus != resetSwitchStatus)
    {
        lastResetCountSwitchStatus = resetSwitchStatus;
        if(resetSwitchStatus == RESET_SWITCH_DOWN)
        {
            realStorage.resetStorage();
            currentCount = 0;
            pRealDisplay->updateDisplay(DISPLAY_IDLE, 0);
            pRealDisplay->updateDisplay(DISPLAY_COUNT, currentCount);
        }
    }
}


/*!Function         processCountUpdate
*   \param          void
*   \return         void
*   \par Purpose    check for counter activity
*   \note           this will usually run once at startup and
*                   each time a reset count is performed
*/
void
processCountUpdate(void)
{
    static int16_t lastCount = 0;
    bool countIncreasing = true;
    int16_t count = pMotorControlObj->getMotorPulseCount();

    if(lastCount != count)
    {
        if(count < lastCount)
        {
            countIncreasing = false;
        }

        lastCount = count;
        currentCount = count;
        realStorage.storeCount(count);

        if(pRealDisplay)
        {
            if(true == countIncreasing)
            {
                pRealDisplay->updateDisplay(DISPLAY_DOWN, 0);
            }
            else
            {
                pRealDisplay->updateDisplay(DISPLAY_UP, 0);
            }
            pRealDisplay->updateDisplay(DISPLAY_COUNT, currentCount);
        }
    }
}

/*!Function         initializeDisplay
*   \param          void
*   \return         void
*   \par Purpose    create display object and display the banner
*   \note           this will run once at startup, we need at least
*                   40ms after power rises above 2.7V before sending
*                   commands. Arduino can be active way before this
*                   we'll wait 100ms
*/
void
initializeDisplay(void)
{
    pRealDisplay = new RealDisplay;

    if(pRealDisplay)
    {
        pRealDisplay->updateDisplay(DISPLAY_CONFIG, 0);
        pRealDisplay->updateDisplay(DISPLAY_BANNER, 0);
        pRealDisplay->updateDisplay(DISPLAY_IDLE, 0);
        pRealDisplay->updateDisplay(DISPLAY_COUNT, currentCount);
    }
}


/*!Function         toggleTestLED
*   \param          void
*   \return         void
*   \par Purpose    just gives an indication of activity on the
*                   reed relay - no functional use for device
*/
void
toggleTestLED(void)
{
    static uint8_t toggle = LOW;

    if(LOW == toggle)
    {
        toggle = HIGH;
    }
    else
    {
        toggle = LOW;
    }
    digitalWrite(onBoardLED, toggle);
}
