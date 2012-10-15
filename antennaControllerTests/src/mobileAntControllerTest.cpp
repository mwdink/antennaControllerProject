/*
 * mobileAntControllerTest.cpp
 *
 *  Created on: Aug 23, 2012
 *      Author: dinkem1
 */
/*----------------------------- Nested includes -----------------------------*/
#include "stdio.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "qp_port.h"
#include "WProgram.h"
#include "mobileAntController.hpp"
#include "switchControl.hpp"
#include "mockMotorControl.hpp"
#include "mockSwitchControl.hpp"
#include "mockMotor.hpp"
#include "mockSwitches.hpp"
#include "mockStorage.hpp"

/*----------------- Symbolic Constants and Macros (defines) -----------------*/


/*-------------------------- Typedefs and structs ---------------------------*/
/*----------------------- Declarations (externs only) -----------------------*/
/*------------------------------ Declarations -------------------------------*/
mock_motorControlHsm *pMotorControl = 0;
QP::QEvt testMotorEvent = QEVT_INITIALIZER(0);

int mockDigitalPins[20];
uint8_t mockPinModes[20];
int8_t mockEEProm[maxStorageSizeInBytes+20];

/*---------------------------------- Functions ------------------------------*/
void BSP_display(char const *msg){}

void BSP_exit(void){}

void BSP_addMotorEvent(uint16_t sig)
{
	testMotorEvent.sig = (QP::QSignal)(sig);
	pMotorControl->dispatch(&testMotorEvent);
}

void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line){}

void pinMode(uint8_t addr, uint8_t value)
{
    mockPinModes[addr] = value;
}

void digitalWrite(uint8_t addr, uint8_t value)
{
    mockDigitalPins[addr] = value;
}

int digitalRead(uint8_t addr)
{
    return mockDigitalPins[addr];
}


TEST_GROUP(noSetUp){};


TEST_GROUP(testSetUp)
{
    mock_motorControlHsm motorControl;
    mock_switchControlHsm switchControl;
    mockMotor motor;

    QP::QEvt *pMotorEvent;
    QP::QEvt *pSwitchEvent;

    void setup()
    {
        motorControl.setMotorInterface(&motor);
    	motorControl.init();
        switchControl.init();

        QP::QEvt motorEvent = QEVT_INITIALIZER(0);
        QP::QEvt switchEvent = QEVT_INITIALIZER(0);
        pMotorEvent = &motorEvent;
        pSwitchEvent = &switchEvent;

    	pMotorControl = &motorControl;
    }

    void teardown()
    {
    	pMotorControl = 0;
    }
};

TEST(noSetUp, storeCountWraparound)
{
    mockStorage *pStorage = new mockStorage;
    pStorage->resetStorage();

    pStorage->setOffset(maxStorageSizeInBytes - 4);
    CHECK_EQUAL((int)196, pStorage->getOffset());
    pStorage->storeCount(68);
    CHECK_EQUAL((int)198, pStorage->getOffset());
    pStorage->storeCount(-1);
    CHECK_EQUAL((int)200, pStorage->getOffset());

    pStorage->storeCount(879);
    CHECK_EQUAL((int)2, pStorage->getOffset());
    CHECK_EQUAL((int)879, pStorage->getStoredCount());
    CHECK_EQUAL((int8_t)0x03, mockEEProm[0]);
    CHECK_EQUAL((int8_t)0x6f, mockEEProm[1]);
    CHECK_EQUAL((int8_t)0x03, mockEEProm[2]);
    CHECK_EQUAL((int8_t)0xe7, mockEEProm[3]);

    delete pStorage;
}


TEST(noSetUp, storeCount)
{
    mockStorage *pStorage = new mockStorage;
    pStorage->resetStorage();
    CHECK_EQUAL((int)0, pStorage->getStoredCount());
    CHECK_EQUAL((int)0, pStorage->getOffset());

    pStorage->storeCount(21);
    pStorage->storeCount(33);
    CHECK_EQUAL((int)33, pStorage->getStoredCount());
    CHECK_EQUAL((int)4, pStorage->getOffset());

    pStorage->storeCount(-68);
    CHECK_EQUAL((int)-68, pStorage->getStoredCount());
    CHECK_EQUAL((int)6, pStorage->getOffset());

    pStorage->storeCount(-843);
    CHECK_EQUAL((int)-843, pStorage->getStoredCount());
    CHECK_EQUAL((int)8, pStorage->getOffset());

    pStorage->storeCount(997);
    CHECK_EQUAL((int)997, pStorage->getStoredCount());
    CHECK_EQUAL((int)10, pStorage->getOffset());

    pStorage->storeCount(-2);
    CHECK_EQUAL((int)-2, pStorage->getStoredCount());
    CHECK_EQUAL((int)12, pStorage->getOffset());

    pStorage->resetStorage();
    CHECK_EQUAL((int)0, pStorage->getStoredCount());
    CHECK_EQUAL((int)0, pStorage->getOffset());

    delete pStorage;
}



TEST(noSetUp, getStoredCount)
{
    mockStorage *pStorage = new mockStorage;
    pStorage->resetStorage();
    CHECK_EQUAL((int)0, pStorage->getStoredCount());
    CHECK_EQUAL((int)0, pStorage->getOffset());

    int16_t addr = 0x0000;
    int16_t value = 25;
    mockEEProm[addr] = (int8_t)(value >> 8) & 0xFF;
    mockEEProm[addr+1] = (int8_t)value & 0xFF;
    value = 31;
    addr+=2;
    mockEEProm[addr] = (int8_t)(value >> 8) & 0xFF;
    mockEEProm[addr+1] = (int8_t)value & 0xFF;
    CHECK_EQUAL((int)31, pStorage->getStoredCount());
    CHECK_EQUAL((int)addr+2, pStorage->getOffset());

    value = -67;
    addr+=2;
    mockEEProm[addr] = (int8_t)(value >> 8) & 0xFF;
    mockEEProm[addr+1] = (int8_t)value & 0xFF;
    CHECK_EQUAL((int)-67, pStorage->getStoredCount());
    CHECK_EQUAL((int)addr+2, pStorage->getOffset());

    value = -877;
    addr+=2;
    mockEEProm[addr] = (int8_t)(value >> 8) & 0xFF;
    mockEEProm[addr+1] = (int8_t)value & 0xFF;
    CHECK_EQUAL((int)-877, pStorage->getStoredCount());
    CHECK_EQUAL((int)addr+2, pStorage->getOffset());

    value = 943;
    addr+=2;
    mockEEProm[addr] = (int8_t)(value >> 8) & 0xFF;
    mockEEProm[addr+1] = (int8_t)value & 0xFF;
    CHECK_EQUAL((int)943, pStorage->getStoredCount());
    CHECK_EQUAL((int)addr+2, pStorage->getOffset());

    value = -1;
    addr+=2;
    mockEEProm[addr] = (int8_t)(value >> 8) & 0xFF;
    mockEEProm[addr+1] = (int8_t)value & 0xFF;
    CHECK_EQUAL((int)-1, pStorage->getStoredCount());
    CHECK_EQUAL((int)addr+2, pStorage->getOffset());

    pStorage->resetStorage();
    CHECK_EQUAL((int)0, pStorage->getStoredCount());
    CHECK_EQUAL((int)0, pStorage->getOffset());

    delete pStorage;
}



TEST(noSetUp, resetStorage)
{
    mockStorage *pStorage = new mockStorage;

    CHECK_EQUAL((int8_t)0xff, mockEEProm[0]);
    CHECK_EQUAL((int8_t)0xff, mockEEProm[1]);
    CHECK_EQUAL((int8_t)0xff, mockEEProm[pStorage->getMaxStorageSizeInBytes()]);
    CHECK_EQUAL(200, pStorage->getMaxStorageSizeInBytes());
    //CHECK_EQUAL(0, pStorage->getStoredCount());
    CHECK_EQUAL(0, pStorage->getOffset());

    pStorage->resetStorage();
    CHECK_EQUAL((int8_t)0x03, mockEEProm[0]);
    CHECK_EQUAL((int8_t)0xe7, mockEEProm[1]);
    CHECK_EQUAL((int8_t)0x03, mockEEProm[pStorage->getMaxStorageSizeInBytes()-2]);
    CHECK_EQUAL((int8_t)0xe7, mockEEProm[pStorage->getMaxStorageSizeInBytes()-1]);
    CHECK_EQUAL((int8_t)0xff, mockEEProm[pStorage->getMaxStorageSizeInBytes()]);

    delete pStorage;
}


TEST(noSetUp, motorSwitches)
{
    uint8_t switchUp            = 14;   // A0 Arduino analog port 0
    uint8_t switchDown          = 15;   // A1 Arduino analog port 1
    mockSwitches switches;

    CHECK_EQUAL(0xffff, mockDigitalPins[switchUp]);
    CHECK_EQUAL(0xffff, mockDigitalPins[switchDown]);
    CHECK_EQUAL(0xff, mockPinModes[switchUp]);
    CHECK_EQUAL(0xff, mockPinModes[switchDown]);

    switches.initializeSwitchHardware();
    CHECK_EQUAL(HIGH, mockDigitalPins[switchUp]);
    CHECK_EQUAL(HIGH, mockDigitalPins[switchDown]);
    CHECK_EQUAL(INPUT, mockPinModes[switchUp]);
    CHECK_EQUAL(INPUT, mockPinModes[switchDown]);

    switches.flipSwitch(switchUp, 1);
    switches.flipSwitch(switchDown, 1);
    CHECK_EQUAL(MOTOR_SWITCH_RELEASED, switches.getMotorControlSwitches());

    switches.flipSwitch(switchUp, 0);
    switches.flipSwitch(switchDown, 1);
    CHECK_EQUAL(MOTOR_SWITCH_UP, switches.getMotorControlSwitches());

    switches.flipSwitch(switchUp, 1);
    switches.flipSwitch(switchDown, 0);
    CHECK_EQUAL(MOTOR_SWITCH_DOWN, switches.getMotorControlSwitches());

    switches.flipSwitch(switchUp, 0);
    switches.flipSwitch(switchDown, 0);
    CHECK_EQUAL(MOTOR_SWITCH_DOWN, switches.getMotorControlSwitches());
}


TEST(noSetUp, resetCountSwitch)
{
    uint8_t resetCountSwitch    = 18;   // A4 Arduino analog port 4
    mockSwitches switches;

    CHECK_EQUAL(0xffff, mockDigitalPins[resetCountSwitch]);
    CHECK_EQUAL(0xff, mockPinModes[resetCountSwitch]);

    switches.initializeSwitchHardware();
    CHECK_EQUAL(HIGH, mockDigitalPins[resetCountSwitch]);
    CHECK_EQUAL(INPUT, mockPinModes[resetCountSwitch]);

    switches.flipSwitch(resetCountSwitch, 1);
    CHECK_EQUAL(1, switches.getResetCountSwitch());

    switches.flipSwitch(resetCountSwitch, 0);
    CHECK_EQUAL(0, switches.getResetCountSwitch());
}



TEST(noSetUp, motorHardware)
{
    uint8_t motorUp   =   16;         // A2 Arduino analog port 2
    uint8_t motorDown =   17;         // A3 Arduino analog port 3
    mockMotor motor;

    CHECK_EQUAL(0xffff, mockDigitalPins[motorUp]);
    CHECK_EQUAL(0xff, mockPinModes[motorUp]);
    CHECK_EQUAL(0xffff, mockDigitalPins[motorDown]);
    CHECK_EQUAL(0xff, mockPinModes[motorDown]);

    motor.initializeMotorHardware();
    CHECK_EQUAL(LOW, mockDigitalPins[motorUp]);
    CHECK_EQUAL(LOW, mockDigitalPins[motorDown]);
    CHECK_EQUAL(OUTPUT, mockPinModes[motorUp]);
    CHECK_EQUAL(OUTPUT, mockPinModes[motorDown]);

    motor.runMotorDown();
    CHECK_EQUAL(HIGH, mockDigitalPins[motorDown]);
    CHECK_EQUAL(LOW, mockDigitalPins[motorUp]);

    motor.runMotorUp();
    CHECK_EQUAL(LOW, mockDigitalPins[motorDown]);
    CHECK_EQUAL(HIGH, mockDigitalPins[motorUp]);

    motor.setMotorIdle();
    CHECK_EQUAL(LOW, mockDigitalPins[motorDown]);
    CHECK_EQUAL(LOW, mockDigitalPins[motorUp]);
}


TEST(testSetUp, switchDownFromUp)
{
    pSwitchEvent->sig = (QP::QSignal)(SWITCH_UP_PUSHED_SIG);
    switchControl.dispatch(pSwitchEvent);
    CHECK_EQUAL(SWITCH_CONTROL_HSM_UP, switchControl.getSwitchControlState());
    CHECK_EQUAL(MOTOR_CONTROL_HSM_UP, motorControl.getMotorControlState());
    CHECK_EQUAL(MOTOR_UP, motorControl.getMotorDirection());

    pSwitchEvent->sig = (QP::QSignal)(SWITCH_DOWN_PUSHED_SIG);
    switchControl.dispatch(pSwitchEvent);
    CHECK_EQUAL(SWITCH_CONTROL_HSM_IDLE, switchControl.getSwitchControlState());
    CHECK_EQUAL(MOTOR_UP, motorControl.getMotorDirection());
    CHECK_EQUAL(MOTOR_CONTROL_HSM_IDLE, motorControl.getMotorControlState());
}

TEST(testSetUp, switchUpFromDown)
{
    pSwitchEvent->sig = (QP::QSignal)(SWITCH_DOWN_PUSHED_SIG);
    switchControl.dispatch(pSwitchEvent);
    CHECK_EQUAL(SWITCH_CONTROL_HSM_DOWN, switchControl.getSwitchControlState());
    CHECK_EQUAL(MOTOR_CONTROL_HSM_DOWN, motorControl.getMotorControlState());
    CHECK_EQUAL(MOTOR_DOWN, motorControl.getMotorDirection());

    pSwitchEvent->sig = (QP::QSignal)(SWITCH_UP_PUSHED_SIG);
    switchControl.dispatch(pSwitchEvent);
    CHECK_EQUAL(SWITCH_CONTROL_HSM_IDLE, switchControl.getSwitchControlState());
    CHECK_EQUAL(MOTOR_DOWN, motorControl.getMotorDirection());
    CHECK_EQUAL(MOTOR_CONTROL_HSM_IDLE, motorControl.getMotorControlState());
}



TEST(testSetUp, switchUpReleased)
{
    pSwitchEvent->sig = (QP::QSignal)(SWITCH_UP_PUSHED_SIG);
    switchControl.dispatch(pSwitchEvent);
    CHECK_EQUAL(SWITCH_CONTROL_HSM_UP, switchControl.getSwitchControlState());
    CHECK_EQUAL(MOTOR_CONTROL_HSM_UP, motorControl.getMotorControlState());
    CHECK_EQUAL(MOTOR_UP, motorControl.getMotorDirection());

    pSwitchEvent->sig = (QP::QSignal)(SWITCH_RELEASED_SIG);
    switchControl.dispatch(pSwitchEvent);
    CHECK_EQUAL(SWITCH_CONTROL_HSM_IDLE, switchControl.getSwitchControlState());
    CHECK_EQUAL(MOTOR_UP, motorControl.getMotorDirection());
    CHECK_EQUAL(MOTOR_CONTROL_HSM_IDLE, motorControl.getMotorControlState());

    pMotorEvent->sig = (QP::QSignal)(MOTOR_MAGNET_SIG);
    motorControl.dispatch(pMotorEvent);
    motorControl.dispatch(pMotorEvent);
    motorControl.dispatch(pMotorEvent);
    CHECK_EQUAL(-3, motorControl.getPulseCount());

}

TEST(testSetUp, switchDownReleased)
{
    pSwitchEvent->sig = (QP::QSignal)(SWITCH_DOWN_PUSHED_SIG);
    switchControl.dispatch(pSwitchEvent);
    CHECK_EQUAL(SWITCH_CONTROL_HSM_DOWN, switchControl.getSwitchControlState());
    CHECK_EQUAL(MOTOR_CONTROL_HSM_DOWN, motorControl.getMotorControlState());
    CHECK_EQUAL(MOTOR_DOWN, motorControl.getMotorDirection());

    pSwitchEvent->sig = (QP::QSignal)(SWITCH_RELEASED_SIG);
    switchControl.dispatch(pSwitchEvent);
    CHECK_EQUAL(SWITCH_CONTROL_HSM_IDLE, switchControl.getSwitchControlState());
    CHECK_EQUAL(MOTOR_DOWN, motorControl.getMotorDirection());
    CHECK_EQUAL(MOTOR_CONTROL_HSM_IDLE, motorControl.getMotorControlState());

    pMotorEvent->sig = (QP::QSignal)(MOTOR_MAGNET_SIG);
    motorControl.dispatch(pMotorEvent);
    motorControl.dispatch(pMotorEvent);
    CHECK_EQUAL(2, motorControl.getPulseCount());
}


TEST(testSetUp, switchPushedUpFromIdle)
{
    pSwitchEvent->sig = (QP::QSignal)(SWITCH_UP_PUSHED_SIG);
    switchControl.dispatch(pSwitchEvent);
    pMotorEvent->sig = (QP::QSignal)(MOTOR_MAGNET_SIG);
    motorControl.dispatch(pMotorEvent);
    CHECK_EQUAL(SWITCH_CONTROL_HSM_UP, switchControl.getSwitchControlState());
    CHECK_EQUAL(MOTOR_CONTROL_HSM_UP, motorControl.getMotorControlState());
    CHECK_EQUAL(-1, motorControl.getPulseCount());
    CHECK_EQUAL(MOTOR_UP, motorControl.getMotorDirection());

    pSwitchEvent->sig = (QP::QSignal)(SWITCH_UP_PUSHED_SIG);
    switchControl.dispatch(pSwitchEvent);
    CHECK_EQUAL(SWITCH_CONTROL_HSM_UP, switchControl.getSwitchControlState());
    CHECK_EQUAL(MOTOR_CONTROL_HSM_UP, motorControl.getMotorControlState());
    CHECK_EQUAL(MOTOR_UP, motorControl.getMotorDirection());
}

TEST(testSetUp, switchPushedDownFromIdle)
{
    pSwitchEvent->sig = (QP::QSignal)(SWITCH_DOWN_PUSHED_SIG);
    switchControl.dispatch(pSwitchEvent);
    pMotorEvent->sig = (QP::QSignal)(MOTOR_MAGNET_SIG);
    motorControl.dispatch(pMotorEvent);
    CHECK_EQUAL(SWITCH_CONTROL_HSM_DOWN, switchControl.getSwitchControlState());
    CHECK_EQUAL(MOTOR_CONTROL_HSM_DOWN, motorControl.getMotorControlState());
    CHECK_EQUAL(1, motorControl.getPulseCount());
    CHECK_EQUAL(MOTOR_DOWN, motorControl.getMotorDirection());

    pSwitchEvent->sig = (QP::QSignal)(SWITCH_DOWN_PUSHED_SIG);
    switchControl.dispatch(pSwitchEvent);
    CHECK_EQUAL(SWITCH_CONTROL_HSM_DOWN, switchControl.getSwitchControlState());
    CHECK_EQUAL(MOTOR_CONTROL_HSM_DOWN, motorControl.getMotorControlState());
    CHECK_EQUAL(MOTOR_DOWN, motorControl.getMotorDirection());
}



TEST(testSetUp, motorMagnetInRotate)
{
    pMotorEvent->sig = (QP::QSignal)(MOTOR_DOWN_SIG);
    motorControl.dispatch(pMotorEvent);
    pMotorEvent->sig = (QP::QSignal)(MOTOR_MAGNET_SIG);
    motorControl.dispatch(pMotorEvent);
    CHECK_EQUAL(1, motorControl.getPulseCount());
    pMotorEvent->sig = (QP::QSignal)(MOTOR_IDLE_SIG);
    motorControl.dispatch(pMotorEvent);
    pMotorEvent->sig = (QP::QSignal)(MOTOR_MAGNET_SIG);
    motorControl.dispatch(pMotorEvent);
    CHECK_EQUAL(2, motorControl.getPulseCount());

    pMotorEvent->sig = (QP::QSignal)(MOTOR_UP_SIG);
    motorControl.dispatch(pMotorEvent);
    pMotorEvent->sig = (QP::QSignal)(MOTOR_MAGNET_SIG);
    motorControl.dispatch(pMotorEvent);
    motorControl.dispatch(pMotorEvent);
    CHECK_EQUAL(0, motorControl.getPulseCount());
    pMotorEvent->sig = (QP::QSignal)(MOTOR_IDLE_SIG);
    motorControl.dispatch(pMotorEvent);
    pMotorEvent->sig = (QP::QSignal)(MOTOR_MAGNET_SIG);
    motorControl.dispatch(pMotorEvent);
    motorControl.dispatch(pMotorEvent);
    CHECK_EQUAL(-2, motorControl.getPulseCount());
}

TEST(testSetUp, motorMagnetInInitial)
{
	pMotorEvent->sig = (QP::QSignal)(MOTOR_MAGNET_SIG);
    motorControl.dispatch(pMotorEvent);
    CHECK_EQUAL(0, motorControl.getPulseCount());
}

TEST(testSetUp, motorIdleFromIdle)
{
	pMotorEvent->sig = (QP::QSignal)(MOTOR_IDLE_SIG);
    motorControl.dispatch(pMotorEvent);
    CHECK_EQUAL(MOTOR_CONTROL_HSM_IDLE, motorControl.getMotorControlState());
}


TEST(testSetUp, motorIdleFromRotate)
{
    pMotorEvent->sig = (QP::QSignal)(MOTOR_DOWN_SIG);
    motorControl.dispatch(pMotorEvent);
    CHECK_EQUAL(MOTOR_CONTROL_HSM_DOWN, motorControl.getMotorControlState());
    pMotorEvent->sig = (QP::QSignal)(MOTOR_IDLE_SIG);
    motorControl.dispatch(pMotorEvent);
    CHECK_EQUAL(MOTOR_CONTROL_HSM_IDLE, motorControl.getMotorControlState());
    pMotorEvent->sig = (QP::QSignal)(MOTOR_UP_SIG);
    motorControl.dispatch(pMotorEvent);
    CHECK_EQUAL(MOTOR_CONTROL_HSM_UP, motorControl.getMotorControlState());
    pMotorEvent->sig = (QP::QSignal)(MOTOR_IDLE_SIG);
    motorControl.dispatch(pMotorEvent);
    CHECK_EQUAL(MOTOR_CONTROL_HSM_IDLE, motorControl.getMotorControlState());

}


TEST(testSetUp, motorRotateUp)
{
	pMotorEvent->sig = (QP::QSignal)(MOTOR_UP_SIG);
    motorControl.dispatch(pMotorEvent);
    CHECK_EQUAL(MOTOR_CONTROL_HSM_UP, motorControl.getMotorControlState());
    CHECK_EQUAL(0, motorControl.getPulseCount());
    CHECK_EQUAL(MOTOR_UP, motorControl.getMotorDirection());
}



TEST(testSetUp, motorRotateDown)
{
    CHECK_EQUAL(MOTOR_CONTROL_HSM_IDLE, motorControl.getMotorControlState());
    CHECK_EQUAL(0, motorControl.getPulseCount());
    CHECK_EQUAL(MOTOR_IDLE, motorControl.getMotorDirection());

	pMotorEvent->sig = (QP::QSignal)(MOTOR_DOWN_SIG);
    motorControl.dispatch(pMotorEvent);
    CHECK_EQUAL(MOTOR_CONTROL_HSM_DOWN, motorControl.getMotorControlState());
    CHECK_EQUAL(0, motorControl.getPulseCount());
    CHECK_EQUAL(MOTOR_DOWN, motorControl.getMotorDirection());
}



TEST(noSetUp, switchInitial)
{
    mock_switchControlHsm switchControl;
    CHECK_EQUAL(SWITCH_CONTROL_HSM_NULL, switchControl.getSwitchControlState());
    switchControl.init();
    CHECK_EQUAL(SWITCH_CONTROL_HSM_IDLE, switchControl.getSwitchControlState());
}

TEST(noSetUp, motorInitial)
{
	mock_motorControlHsm motorControl;
    mockMotor motor;
    CHECK_EQUAL(MOTOR_CONTROL_HSM_NULL, motorControl.getMotorControlState());

    motorControl.setMotorInterface(&motor);
    motorControl.init();
    CHECK_EQUAL(MOTOR_CONTROL_HSM_IDLE, motorControl.getMotorControlState());
    CHECK_EQUAL(0, motorControl.getPulseCount());
    CHECK_EQUAL(MOTOR_IDLE, motorControl.getMotorDirection());
}




