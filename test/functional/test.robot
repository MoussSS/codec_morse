# test.robot

*** Settings ***
Suite Setup     Setup
Suite Teardown  Teardown
Test Setup      Reset Emulation
Test Teardown   Test Teardown
Resource        ${RENODEKEYWORDS}
Library    Telnet

*** Variables ***
${PROJECT_PATH}        /home/ferrierf/Projects/stm32-from-scratch
${RENODE_CFG_FILE}     ${PROJECT_PATH}/config/nucleo-f446re.repl
${FIRMWARE_FILE}       ${PROJECT_PATH}/bin/firmware_Debug.elf

*** Keywords ***
Create Machine
    Log    "Setup"    console=True

    Execute Command            mach create
    Execute Command            machine LoadPlatformDescription @${RENODE_CFG_FILE}
    Execute Command            sysbus LoadELF @${FIRMWARE_FILE}

*** Test Cases ***
Led_Blinking
    [Documentation]    Test led blinking

    Create Machine

    Log    "Led_Blinking"    console=True

    Execute Command    help

    ${id}=    Create Led Tester    sysbus.gpioPortA.UserLed
    Log    "Led Test Id: ${id}"    console=True

    Start Emulation
    Assert Led Is Blinking    testDuration=1.0    onDuration=0.5    offDuration=0.5    tolerance=0.05    testerId=${id}        
    #Assert Led Duty Cycle    testDuration=1.0    expectedDutyCycle=0.1    tolerance=0.05
