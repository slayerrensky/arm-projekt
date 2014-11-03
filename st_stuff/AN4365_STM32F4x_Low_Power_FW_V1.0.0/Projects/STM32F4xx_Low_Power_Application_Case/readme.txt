/**
  @page STM32F4xx Low Power Application Case
  
  @verbatim
  ******************** (C) COPYRIGHT 2014 STMicroelectronics *******************
  * @file    STM32F4xx_Low_Power_Application_Case/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    17-January-2014
  * @brief   Description of the STM32F4 Low Power Application Case
  ******************************************************************************
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

@par Example Description 

This is an example of a sensor hub application based on ADC, I2C and DMA 
periherals using STM32F4xx low power modes and features.

a) Hardware Description:  
  =====================
  This example requires two boards(Master and Slave)


@verbatim
*------------------------------------------------------------------------------*
|                BOARD A                                BOARD B                |
|         ____________________                   ____________________          |
|        |                    |                 |                    |         |
|        |                    |                 |                    |         | 
|        |     __________     |                 |     __________     |         |
|        |    |I2C Master|____|_______SDA_______|____|I2C Slave |    |         |
|        |    |  Device  |____|_______SCL_______|____|  Device  |    |         |
|        |    |__________|    |                 |    |__________|    |         |
|        |                    |                 |                    |         |
|        |                 PA5|_________________|PA0                 |         |
|        |                    |                 |                    |         |
|        |     __             |                 |                    |         |
|        |    |__|            |                 |                    |         |         
|        |   Button           |                 |                    |         |
|        |    USER            |                 |                    |         |
|        |             GND O--|-----------------|--O GND             |         |
|        |____________________|                 |____________________|         |
|                                                                              |
|                                                                              |
*------------------------------------------------------------------------------*
@endverbatim

b)Software Description:
=====================
 
To use this example, you need to load it on two STM32F4 discovery boards. 
In the toolchain interface you choose the dedicated workspace to configure the
I2C peripheral as Master or as Slave device .

The Master board I2C peripheral is configured as Master receiver with DMA
and in slave board I2C peripheral is configured as Salve transmitter with DMA.

The application begins with pressing button "USER" from master side to wakeup 
the two devices from standby and to start the communication.

In slave side the ADC starts sampling VBat voltages and DMA transfers the samples
to memory. During this operation the CPU stays in sleep mode. Once the operation 
is finished the CPU configures the I2C to take in charge the data transfer to the 
Master with DMA and goes to sleep again.

In master side the data reception starts via I2C and DMA. The CPU stills in sleep 
while the reception is not completed yet. When the transfer is finished and the 
data is already in the memory, the CPU is waken up and takes in charge the FFT 
transformation of the data. At the end, the CPU goes another time to a low power
mode (Stand-by)for a period controlled by the RTC peripheral. 

At the end of this period the RTC wakes up the CPU to launch again the operations.

A defined communication timeout is insuring that the application will not remain 
stuck in case of I2C communication is corrupted.
You can adjust this timeout through the USER_TIMEOUT define inside main.h file
depending on CPU frequency and application conditions (interrupts routines, 
number of data to transfer, speed, CPU frequency...).

When a given operation is not finished within the defined timeout, a callback is 
made to the function "TimeOut_UserCallback()", this function is used to manage
timeout communication failure. In this example, this callback implements a simple 
infinite loop, but user can add his own implementation to manage timeout failure.

Note: For more details about Hardware/software environment setup description, 
you can refer to the application note AN4365 available on ST web site.
 
@par Directory contents 

  - STM32F4xx_Low_Power_Application_Case/src/main.c              Main program
  - STM32F4xx_Low_Power_Application_Case/src/stm32f4xx_it.c      Interrupt handlers
  - STM32F4xx_Low_Power_Application_Case/src/system_stm32f4xx.c  STM32F4xx system source file
  - STM32F4xx_Low_Power_Application_Case/inc/main.h              Main Header file
  - STM32F4xx_Low_Power_Application_Case/inc/stm32f4xx_it.h      Interrupt handlers header file
  - STM32F4xx_Low_Power_Application_Case/inc/stm32f4xx_conf.h    Library Configuration file

@par Hardware and Software environment

  - This example runs on STM32F405xx/407xx, STM32F415xx/417xx, STM32F42xxx/437xx, 
    STM32F429xx/439xx and STM32F401x devices
    
  - This example has been tested with STMicroelectronics STM32F4DISCOVERY MB997C, 
    STM32F401C-DISCO MB1115B and STM32F429I-DISCO MB1075B kits.

  - Discoveries Set-up 
    - Connect I2C1 Master SCL pin (PB.06) to I2C1 Slave SCL (PB.06)
    - Connect I2C1 Master SDA pin (PB.09) to I2C1 Slave SDA (PB.09)
    - Connect PA5(master side) to PA0(slave side)
    - Connect GND(master side) to GND(slave side)

   @notes  
    - Pull-up resistances should be connected to I2C SCL and SDA Pins in case of 
      master/slave STM32F429I-DISCO MB1075B use.
    - The R30 resistance should be removed from STM32F401C-DISCO MB1115B discovery.
    - The R34 and R31 resistances should be removed from STM32F4DISCOVERY MB997C
      discovery.      

@par How to use it ? 

In order to make the program work, you must do the following :

 + EWARM
    - Open the Project.eww workspace
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)

 + MDK-ARM
    - Open the Project.uvproj project
    - Rebuild all files: Project->Rebuild all target files
    - Load project image: Debug->Start/Stop Debug Session
    - Run program: Debug->Run (F5)    
 
 * <h3><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h3>
 */

