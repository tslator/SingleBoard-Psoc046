/*******************************************************************************
* File Name: HCSR04_Trigger_10.c  
* Version 2.20
*
* Description:
*  This file contains APIs to set up the Pins component for low power modes.
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "HCSR04_Trigger_10.h"

static HCSR04_Trigger_10_BACKUP_STRUCT  HCSR04_Trigger_10_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: HCSR04_Trigger_10_Sleep
****************************************************************************//**
*
* \brief Stores the pin configuration and prepares the pin for entering chip 
*  deep-sleep/hibernate modes. This function must be called for SIO and USBIO
*  pins. It is not essential if using GPIO or GPIO_OVT pins.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None 
*  
* \sideeffect
*  For SIO pins, this function configures the pin input threshold to CMOS and
*  drive level to Vddio. This is needed for SIO pins when in device 
*  deep-sleep/hibernate modes.
*
* \funcusage
*  \snippet HCSR04_Trigger_10_SUT.c usage_HCSR04_Trigger_10_Sleep_Wakeup
*******************************************************************************/
void HCSR04_Trigger_10_Sleep(void)
{
    #if defined(HCSR04_Trigger_10__PC)
        HCSR04_Trigger_10_backup.pcState = HCSR04_Trigger_10_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            HCSR04_Trigger_10_backup.usbState = HCSR04_Trigger_10_CR1_REG;
            HCSR04_Trigger_10_USB_POWER_REG |= HCSR04_Trigger_10_USBIO_ENTER_SLEEP;
            HCSR04_Trigger_10_CR1_REG &= HCSR04_Trigger_10_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(HCSR04_Trigger_10__SIO)
        HCSR04_Trigger_10_backup.sioState = HCSR04_Trigger_10_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        HCSR04_Trigger_10_SIO_REG &= (uint32)(~HCSR04_Trigger_10_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: HCSR04_Trigger_10_Wakeup
****************************************************************************//**
*
* \brief Restores the pin configuration that was saved during Pin_Sleep().
*
* For USBIO pins, the wakeup is only triggered for falling edge interrupts.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None
*  
* \funcusage
*  Refer to HCSR04_Trigger_10_Sleep() for an example usage.
*******************************************************************************/
void HCSR04_Trigger_10_Wakeup(void)
{
    #if defined(HCSR04_Trigger_10__PC)
        HCSR04_Trigger_10_PC = HCSR04_Trigger_10_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            HCSR04_Trigger_10_USB_POWER_REG &= HCSR04_Trigger_10_USBIO_EXIT_SLEEP_PH1;
            HCSR04_Trigger_10_CR1_REG = HCSR04_Trigger_10_backup.usbState;
            HCSR04_Trigger_10_USB_POWER_REG &= HCSR04_Trigger_10_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(HCSR04_Trigger_10__SIO)
        HCSR04_Trigger_10_SIO_REG = HCSR04_Trigger_10_backup.sioState;
    #endif
}


/* [] END OF FILE */