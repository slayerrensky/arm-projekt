/**
  @page "STM32F4xx current Consumption Measuring" is a Current Consumption measuring firmware for STM32F4xx MCUs
  
  @verbatim
  ******************** (C) COPYRIGHT 2014 STMicroelectronics *******************
  * @file    STM32F4xx_Current_Consumption_Measuring/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    17-January-2014
  * @brief   Description of STM32F4 current consumption measuring firmware
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
   @endverbatim

@par firmware Description 

This STM32F4xx firmware consumption gives a measuring application that can be 
customized to measure the current consumption of different STM32F4xx Power modes 
and shows how to well configure system for each mode.

Firmware supports all STM32F4x MCUS: 
- STM32F427x/437x/429x/439x
- STM32F405x/415x/407x/417x 
- STM32F401x

The Main Power modes are:
  - RUN mode
  - Sleep mode
  - STOP mode 
  - STANDBY mode
  
For Run and sleep modes, we have chosen to measure the dynamic current consumption
for few samples of frequencies. For Stop and standby modes, you can measure all 
available configurations. 
By default, all modes and sub-modes are selected to be executed and measured by 
the user one by one.(All available sub-modes are described in Tables bellow according 
to the used voltage supply).

To start using the firmware you should proceed as follow:

  1- Select one of the available workspaces dedicated to the STM32F4xx device 
    (STM32F429-Disco MB1075B or STM32F401-Disco MB1115B or STM32F429-Disco MB997C)
     
  2- Compile, run and execute this firmware on one STM32F4 discovery board of
     the selected device.
     
  3- Start measuring sub-modes' current consumption and pass from one mode/sub-mode
     to another by pushing the user-button and following the LED toggling number 
     of times. (The number of Led toggling times according to each sub-mode is 
     detailed in tables below)

You can customize the measuring firmware to select only few modes/sub-modes to 
be measured. In this case, you have to follow these steps:

 1. Select the main power modes that you want to measure by uncommenting only the
    corresponding macros inside the "main.h" file. 
    (By default, all main power modes are selected)

 2. Select sub-power modes that you want to measure for each selected main power 
    mode by uncommenting the corresponding macros inside the "pwr_modes.h" file.  
   (By default, all power sub-modes are selected)   
    We note that for standby mode, you can select and measure only one Standby sub-mode.   

 3. Enable or disable the ART accelerator for RUN & sleep modes by uncommenting 
    the corresponding macros in the top of "pwr_modes.h" file.
    
    We note that the ART accelerator can be enabled without enabling prefetch buffer.
    by keeping the "Prefetch_Enable" macro commented.

 4. Select the used supply voltage by uncommenting the corresponding macro
    in the top of "pwr_modes.h" file.
	
 5. In case you want to Debug the code, you should uncomment the corresponding 
    macro in "main.h" file: 
    

@note1	On the STM32F4DISCOVERY MB997C, an extra power consumption (~500uA) is
        added due to the resistor R31 connected to BOOT0 pin and due to resistors
        R34 connected to BOOT1 pin. So, to reach the correct power consumption 
        values, R31 and R34 resistors must be removed.

Here below a description of the measuring application:
	
@verbatim

 1. Functional description: 
 
    After Power-ON, Red and Green LEDS are ON indicating that application is waiting
    the press of the user-button to start and enter the first mode and first sub mode.
    (Run mode by default). 
    Once the button is pressed, a LEDs' toggling routines is executed to indicate which 
    Power mode and sub mode will be entered: 
	  - The Red LED's number of times toggling indicates the power mode.
   	- The green LED's number of times toggling indicates the sub_mode.
    (The number of time of each Led toggling is detailed in tables bellow)
    
    Once the Leds toggling routine stops and the Leds are switched-off, you can 
    read the current consumption value from the ammeter. To accomplish the measuring 
    process correctly, you should procced as follow according to each power mode case:
    
   - For Run mode case: 
     User should measure the Run sub-mode current consumption before LEDs are back 
     ON when CoreMark executing is finished (after almost 10 s). To pass to the 
     next sub-mode, you should press the user-button, and a Leds toggling routine 
     is executed indicating the new sub-mode. 
      
    - For Sleep mode case:
      You measure the Sleep sub-mode current consumption once the Leds are switched-off. 
      Once you finish measurement, you should press the button to wake-up from 
      sleep mode and then the LEDs are back lit. You press again the button to 
      pass to the next sub-mode.
      
    - For Stop mode case:
      You measure the Stop sub-mode current consumption once the Leds are switched-off. 
      Once you finish measurement, you should press the button to wake-up from 
      sleep mode and then the LEDs are back lit. You press again the button to 
      pass to the next sub-mode.
      
    - For Standby mode case:
      You measure the Standby sub-mode current consumption once the Leds are switched-off.
      Once you finish measurement, you should press the button to wake-up from 
      standby mode and a reset is occurred. LEDs are back lit and you start from 
      the beginning of the measuring application.
      We note that in case of standby with RTC sub-modes, and if the user-button is 
      not pressed, an RTC interruption will be generated after 30s and will wake-up 
      automatically and reset the device.
      
      We note that if any standby mode is defined, when you finish executing the 
      sub-modes of the other power modes Leds are kept it. To restart the measuring 
      application you should press the RESET button.

 2. Power modes description:
   - RUN Mode
    ============
            - System Running at PLL 
            - Code running from Internal FLASH/RAM (User selection)
            - All peripherals disabled/ENABLED.	(User selection)

    - Sleep Mode
    ============  
            - System Running at PLL 
            - Code running from Internal FLASH
            - All peripherals disabled/ENABLED.
            - Wakeup using EXTI Line (User Button PA.00)

    - STOP Mode
    ===========
            - RTC Clocked by LSI
            - Voltage Regulator in Main regulator / in low power regulator mode
            - HSI, HSE OFF and LSI & RTC are not used 
            - No IWDG
            - FLASH in main regulator / in deep power down mode
            - Wakeup using EXTI Line (User Button PA.00)

    - STANDBY Mode
    ==============
            - Backup SRAM OFF/ON and RTC & LSI OFF/ON
            - IWDG OFF
            - Wakeup using Wake-up Pin (PA.00)
            - RTC if used is clocked by LSI           
	    - Automatic Wakeup using RTC clocked by LSI (after ~30s)

@endverbatim

@note2	We note that, while compiling the firmware, two warning messages are still
        generated due to the CoreMark source code.

We give, in the 3 following tables, all power modes/sub-modes that you will 
be able to measure by this firmware for each STM32F4xx device.

In these tables, we note which sub-modes are available according to Vdd value. 
we give, also, Red and Green Led toggling number for each sub-mode, in order to 
follow your measuring sequence by counting both led toggling times.

@note2 	The power sub-mode format is defined like this:

* For RUN and sleep modes: (Power Mode + Peripherals status + Clock Frequency) 
Example: 
RunPeriphDisabled180Mhz = Run mode, Peripherals disabled, Frequency = 180 MHz
			
* For Stop mode: (Power Mode + Regulator status + Flash status)
Example: 
StopLowPwrRegFlashPwrDown = Stop mode, Low power regulator, Flash in power down 
mode
			
* For Standby mode: (Power Mode + Backup SRAM status + RTC status)
Example:
StandbyBkpSramOnRtcOff = Standby mode, Backup SRAM is ON , RTC is OFF
			

First table by STM32F429x/39x device: 

  *=============================================================================|
  Device:         **************   STM32F429/39   **************
  *=============================================================================|
  *  Power  |             Power                  |      Vdd      | LED toggling |
     mode   |            sub-modes               | 3,3 V | 1,8 V | Red  | Green |
  *-----------------------------------------------------------------------------|
  *         | RunPeriphDisabled180Mhz            |  YES  |   -   |  1   |   1   |
  *         -------------------------------------|-------|-------|------|-------|
  *         | RunPeriphEnabled180Mhz             |  YES  |   -   |  1   |   2   |
  *         -------------------------------------|-------|-------|------|-------|
  *         | RunPeriphDisabled168Mhz            |  YES  |  YES  |  1   |   3   |
  *         -------------------------------------|-------|-------|------|-------|
  *  Run    | RunPeriphEnabled168Mhz             |  YES  |  YES  |  1   |   4   |		
  *  mode   -------------------------------------|-------|-------|------|-------|
  *         | RunPeriphDisabled144Mhz            |  YES  |  YES  |  1   |   5   |			
  *         -------------------------------------|-------|-------|------|-------|
  *         | RunPeriphEnabled144Mhz             |  YES  |  YES  |  1   |   6   |		
  *         -------------------------------------|-------|-------|------|-------|
  *         | RunPeriphDisabled120Mhz            |  YES  |  YES  |  1   |   7   |					
  *         -------------------------------------|-------|-------|------|-------|
  *         | RunPeriphEnabled120Mhz             |  YES  |  YES  |  1   |   8   |				
  *-----------------------------------------------------------------------------|				
  *         | SleepPeriphDisabled180Mhz          |  YES  |   -   |  2   |   1   |
  *         -------------------------------------|-------|-------|------|-------|
  *         | SleepPeriphEnabled180Mhz           |  YES  |   -   |  2   |   2   |
  *         -------------------------------------|-------|-------|------|-------|
  *         | SleepPeriphDisabled168Mhz          |  YES  |  YES  |  2   |   3   |
  *         -------------------------------------|-------|-------|------|-------|
  *  Sleep  | SleepPeriphEnabled168Mhz           |  YES  |  YES  |  2   |   4   |		
  *  mode   -------------------------------------|-------|-------|------|-------|
  *         | SleepPeriphDisabled144Mhz          |  YES  |  YES  |  2   |   5   |			
  *         -------------------------------------|-------|-------|------|-------|
  *         | SleepPeriphEnabled144Mhz           |  YES  |  YES  |  2   |   6   |		
  *         -------------------------------------|-------|-------|------|-------|
  *         | SleepPeriphDisabled120Mhz          |  YES  |  YES  |  2   |   7   |					
  *         -------------------------------------|-------|-------|------|-------|
  *         | SleepPeriphEnabled120Mhz           |  YES  |  YES  |  2   |   8   |
  *-----------------------------------------------------------------------------|
  *         | StopMainRegFlashStop               |  YES  |  YES  |  3   |   1   |
  *         -------------------------------------|-------|-------|------|-------|
  *         | StopMainRegFlashPwrDown            |  YES  |  YES  |  3   |   2   |
  *         -------------------------------------|-------|-------|------|-------|
  *         | StopLowPwrRegFlashStop             |  YES  |  YES  |  3   |   3   |
  *         -------------------------------------|-------|-------|------|-------|
  *  Stop   | StopLowPwrRegFlashPwrDown          |  YES  |  YES  |  3   |   4   |				
  *  mode   -------------------------------------|-------|-------|------|-------|
  *         | StopMainRegUnderDriveFlashPwrDown  |  YES  |  YES  |  3   |   5   |				
  *         -------------------------------------|-------|-------|------|-------|
  *         | StopLowPwrRegUnderDriveFlashPwrDown|  YES  |  YES  |  3   |   6   |				
  *-----------------------------------------------------------------------------|
  *         | StandbyBkpSramOnRtcON              |  YES  |  YES  |  4   |   1   |
  *         -------------------------------------|-------|-------|------|-------|
  *         | StandbyBkpSramOffRtcOn             |  YES  |  YES  |  4   |   2   |
  * Standby -------------------------------------|-------|-------|------|-------|
  * mode    | StandbyBkpSramOnRtcOff             |  YES  |  YES  |  4   |   3   |
  *         -------------------------------------|-------|-------|------|-------|
  *         | StandbyBkpSramOffRtcOff            |  YES  |  YES  |  4   |   4   |				
  *-----------------------------------------------------------------------------|

Second table is for STM32F40x/41x device:

  *=============================================================================|
  Device:         **************   STM32F40x/41x   **************
  *=============================================================================|
  *  Power  |             Power                  |      Vdd      | LED toggling |
     modes  |            sub-modes               | 3,3 V | 1,8 V | Red  | Green |
  *-----------------------------------------------------------------------------|
  *         | RunPeriphDisabled168Mhz            |  YES  |   -   |  1   |   1   |
  *         -------------------------------------|-------|-------|------|-------|
  *         | RunPeriphEnabled168Mhz             |  YES  |   -   |  1   |   2   |
  *         -------------------------------------|-------|-------|------|-------|
  *         | RunPeriphDisabled160Mhz            |   -   |  YES  |  1   |   1   |
  *         -------------------------------------|-------|-------|------|-------|
  *  Run    | RunPeriphEnabled160Mhz             |   -   |  YES  |  1   |   2   |		
  *  mode   -------------------------------------|-------|-------|------|-------|
  *         | RunPeriphDisabled144Mhz            |  YES  |  YES  |  1   |   3   |			
  *         -------------------------------------|-------|-------|------|-------|
  *         | RunPeriphEnabled144Mhz             |  YES  |  YES  |  1   |   4   |		
  *         -------------------------------------|-------|-------|------|-------|
  *         | RunPeriphDisabled120Mhz            |  YES  |  YES  |  1   |   5   |					
  *         -------------------------------------|-------|-------|------|-------|
  *         | RunPeriphEnabled120Mhz             |  YES  |  YES  |  1   |   6   |				
  *-----------------------------------------------------------------------------|				
  *         | SleepPeriphDisabled168Mhz          |  YES  |   -   |  2   |   1   |
  *         -------------------------------------|-------|-------|------|-------|
  *         | SleepPeriphEnabled168Mhz           |  YES  |   -   |  2   |   2   |
  *         -------------------------------------|-------|-------|------|-------|
  *         | SleepPeriphDisabled160Mhz          |   -   |  YES  |  2   |   1   |
  *         -------------------------------------|-------|-------|------|-------|
  *  Sleep  | SleepPeriphEnabled160Mhz           |   -   |  YES  |  2   |   2   |		
  *  mode   -------------------------------------|-------|-------|------|-------|
  *         | SleepPeriphDisabled144Mhz          |  YES  |  YES  |  2   |   3   |			
  *         -------------------------------------|-------|-------|------|-------|
  *         | SleepPeriphEnabled144Mhz           |  YES  |  YES  |  2   |   4   |		
  *         -------------------------------------|-------|-------|------|-------|
  *         | SleepPeriphDisabled120Mhz          |  YES  |  YES  |  2   |   5   |					
  *         -------------------------------------|-------|-------|------|-------|
  *         | SleepPeriphEnabled120Mhz           |  YES  |  YES  |  2   |   6   |
  *-----------------------------------------------------------------------------|
  *         | StopMainRegFlashStop               |  YES  |  YES  |  3   |   1   |
  *         -------------------------------------|-------|-------|------|-------|
  *         | StopMainRegFlashPwrDown            |  YES  |  YES  |  3   |   2   |
  *  Stop   -------------------------------------|-------|-------|------|-------|
  *  mode   | StopLowPwrRegFlashStop             |  YES  |  YES  |  3   |   3   |
  *         -------------------------------------|-------|-------|------|-------|
  *         | StopLowPwrRegFlashPwrDown          |  YES  |  YES  |  3   |   4   |							
  *-----------------------------------------------------------------------------|
  *         | StandbyBkpSramOnRtcON              |  YES  |  YES  |  4   |   1   |
  *         -------------------------------------|-------|-------|------|-------|
  *         | StandbyBkpSramOffRtcOn             |  YES  |  YES  |  4   |   2   |
  * Standby -------------------------------------|-------|-------|------|-------|
  *  mode   | StandbyBkpSramOnRtcOff             |  YES  |  YES  |  4   |   3   |
  *         -------------------------------------|-------|-------|------|-------|
  *         | StandbyBkpSramOffRtcOff            |  YES  |  YES  |  4   |   4   |				
  *-----------------------------------------------------------------------------|

Third table is for STM32F401:

  *=============================================================================|
  Device:         **************   STM32F401   **************
  *=============================================================================|
  *  Power  |             Power                  |      Vdd      | LED toggling |
     modes  |            sub-modes               | 3,3 V | 1,8 V | Red  | Green |
  *-----------------------------------------------------------------------------|
  *         | RunPeriphDisabled84Mhz             |  YES  |  YES  |  1   |   1   |
  *         -------------------------------------|-------|-------|------|-------|
  *         | RunPeriphEnabled84Mhz              |  YES  |  YES  |  1   |   2   |
  *   Run   -------------------------------------|-------|-------|------|-------|
  *   mode  | RunPeriphDisabled60Mhz             |  YES  |  YES  |  1   |   3   |
  *         -------------------------------------|-------|-------|------|-------|
  *         | RunPeriphEnable60Mhz              |  YES  |  YES  |  1   |   4   |						
  *-----------------------------------------------------------------------------|				
  *         | SleepPeriphDisabled84Mhz           |  YES  |  YES  |  2   |   1   |
  *         -------------------------------------|-------|-------|------|-------|
  *         | SleepPeriphEnabled84Mhz            |  YES  |  YES  |  2   |   2   |
  *  Sleep  -------------------------------------|-------|-------|------|-------|
  *  mode   | SleepPeriphDisabled60Mhz           |  YES  |  YES  |  2   |   3   |
  *         -------------------------------------|-------|-------|------|-------|
  *         | SleepPeriphEnabled60Mhz            |  YES  |  YES  |  2   |   4   |		
  *-----------------------------------------------------------------------------|
  *         | StopMainRegFlashStop               |  YES  |  YES  |  3   |   1   |
  *         -------------------------------------|-------|-------|------|-------|
  *         | StopMainRegFlashPwrDown            |  YES  |  YES  |  3   |   2   |
  *  Stop   -------------------------------------|-------|-------|------|-------|
  *  mode   | StopLowPwrRegFlashStop             |  YES  |  YES  |  3   |   3   |
  *         -------------------------------------|-------|-------|------|-------|
  *         | StopLowPwrRegFlashPwrDown          |  YES  |  YES  |  3   |   4   |							
  *-----------------------------------------------------------------------------|
  *         | StandbyBkpSramOffRtcOn             |  YES  |  YES  |  4   |   1   |
  * Standby -------------------------------------|-------|-------|------|-------|
  *  mode   | StandbyBkpSramOffRtcOff            |  YES  |  YES  |  4   |   2   |			
  *-----------------------------------------------------------------------------|
  


@par Hardware and Software environment

  - This example runs on these devices :
     * STM32F427x/437x/429x/439x
     * STM32F405/415x/407x/417x 
     * STM32F401xx devices
     *      
  - This example runs on these discovery boards :
     * STM32F4DISCOVERY MB997C
     * STM32F401C-DISCO MB1115B 
     * STM32F429I-DISCO MB1075B    

@note3  Green and Red Leds are the following according to used discovery:
        - For STM32F4DISCOVERY MB997C: Green Led is LD4 , Red Led is LD5
        - For STM32F401C-DISCO MB1115B: Green Led is LD4 , Red Led is LD5
        - For STM32F429I-DISCO MB1075B: Green Led is LD3 , Red Led is LD4       

 For more details about Hardware/software environment setup description, you refer
 to the application note AN4365 available on ST web site.

@par Directory contents 

  - STM32F4xx_Current_Consumption_Measuring/src/main.c             Main program
  - STM32F4xx_Current_Consumption_Measuring/src/stm32f4xx_it.c     Interrupt handlers
  - STM32F4xx_Current_Consumption_Measuring/src/system_stm32f4xx.c STM32F4xx system source file
  - STM32F4xx_Current_Consumption_Measuring/src/pwr_modes.c        Power modes functions 
  - STM32F4xx_Current_Consumption_Measuring/src/core_portme.c      CoreMark command file
  - STM32F4xx_Current_Consumption_Measuring/src/core_list_join.c   core list file
  - STM32F4xx_Current_Consumption_Measuring/src/core_main.c        CoreMark main routine
  - STM32F4xx_Current_Consumption_Measuring/src/core_matrix.c      CoreMark matrix 
  - STM32F4xx_Current_Consumption_Measuring/src/core_state.c       CoreMark state file
  - STM32F4xx_Current_Consumption_Measuring/src/core_util.c        CoreMark util file
  - STM32F4xx_Current_Consumption_Measuring/inc/main.h             Main Header file
  - STM32F4xx_Current_Consumption_Measuring/inc/pwr_modes.h        Headers of test consiguration file
  - STM32F4xx_Current_Consumption_Measuring/inc/stm32f4xx_it.h     Interrupt handler's header file
  - STM32F4xx_Current_Consumption_Measuring/inc/stm32f4xx_conf.h   Library Configuration file
  - STM32F4xx_Current_Consumption_Measuring/inc/core_portme.h      Configuration constants
  - STM32F4xx_Current_Consumption_Measuring/inc/coremark.h         Declarations of the various benchmark functions 
     
@par How to use it ? 
In order to make the program work, you must do the following :

 + EWARM
    - Open the STM32F4xx current consumption measurement.eww workspace 
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)

 + MDK-ARM
    - Open the STM32F4xx current consumption measurement.uvproj project
    - Rebuild all files: Project->Rebuild all target files
    - Load project image: Debug->Start/Stop Debug Session
    - Run program: Debug->Run (F5)    

 * <h3><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h3>
 */
