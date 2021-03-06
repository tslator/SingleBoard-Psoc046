/*******************************************************************************
* File Name: Infrared_ADC.c
* Version 2.30
*
* Description:
*  This file provides the source code to the API for the Sequencing Successive
*  Approximation ADC Component Component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Infrared_ADC.h"


/***************************************
* Global data allocation
***************************************/
uint8 Infrared_ADC_initVar = 0u;
volatile int16 Infrared_ADC_offset[Infrared_ADC_TOTAL_CHANNELS_NUM];
volatile int32 Infrared_ADC_countsPer10Volt[Infrared_ADC_TOTAL_CHANNELS_NUM];   /* Gain compensation */


/***************************************
* Local data allocation
***************************************/
/* Channels configuration generated by customiser */
static const uint32 CYCODE Infrared_ADC_channelsConfig[] = { 0x00000000u };


/*******************************************************************************
* Function Name: Infrared_ADC_Start
********************************************************************************
*
* Summary:
*  Performs all required initialization for this component
*  and enables the power. The power will be set to the appropriate
*  power based on the clock frequency.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  The Infrared_ADC_initVar variable is used to indicate when/if initial
*  configuration of this component has happened. The variable is initialized to
*  zero and set to 1 the first time ADC_Start() is called. This allows for
*  component Re-Start without re-initialization in all subsequent calls to the
*  Infrared_ADC_Start() routine.
*  If re-initialization of the component is required the variable should be set
*  to zero before call of Infrared_ADC_Start() routine, or the user may call
*  Infrared_ADC_Init() and Infrared_ADC_Enable() as done in the
*  Infrared_ADC_Start() routine.
*
*******************************************************************************/
void Infrared_ADC_Start(void)
{
    /* If not Initialized then initialize all required hardware and software */
    if(Infrared_ADC_initVar == 0u)
    {
        Infrared_ADC_Init();
        Infrared_ADC_initVar = 1u;
    }
    Infrared_ADC_Enable();
}


/*******************************************************************************
* Function Name: Infrared_ADC_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the
*  customizer of the component placed onto schematic. Usually called in
*  Infrared_ADC_Start().
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  The Infrared_ADC_offset variable is initialized.
*
*******************************************************************************/
void Infrared_ADC_Init(void)
{
    uint32 chNum;
    uint32 tmpRegVal;
    int32 counts;

    #if(Infrared_ADC_TOTAL_CHANNELS_NUM > 1u)
        static const uint8 CYCODE Infrared_ADC_InputsPlacement[] =
        {
            (uint8)(Infrared_ADC_cy_psoc4_sarmux_8__CH_0_PORT << 4u) |
            (uint8)Infrared_ADC_cy_psoc4_sarmux_8__CH_0_PIN
            ,(uint8)(Infrared_ADC_cy_psoc4_sarmux_8__CH_1_PORT << 4u) |
             (uint8)Infrared_ADC_cy_psoc4_sarmux_8__CH_1_PIN
            #if(Infrared_ADC_TOTAL_CHANNELS_NUM > 2u)
                ,(uint8)(Infrared_ADC_cy_psoc4_sarmux_8__CH_2_PORT << 4u) |
                 (uint8)Infrared_ADC_cy_psoc4_sarmux_8__CH_2_PIN
            #endif /* End Infrared_ADC_TOTAL_CHANNELS_NUM > 2u */
            #if(Infrared_ADC_TOTAL_CHANNELS_NUM > 3u)
                ,(uint8)(Infrared_ADC_cy_psoc4_sarmux_8__CH_3_PORT << 4u) |
                 (uint8)Infrared_ADC_cy_psoc4_sarmux_8__CH_3_PIN
            #endif /* End Infrared_ADC_TOTAL_CHANNELS_NUM > 3u */
            #if(Infrared_ADC_TOTAL_CHANNELS_NUM > 4u)
                ,(uint8)(Infrared_ADC_cy_psoc4_sarmux_8__CH_4_PORT << 4u) |
                 (uint8)Infrared_ADC_cy_psoc4_sarmux_8__CH_4_PIN
            #endif /* End Infrared_ADC_TOTAL_CHANNELS_NUM > 4u */
            #if(Infrared_ADC_TOTAL_CHANNELS_NUM > 5u)
                ,(uint8)(Infrared_ADC_cy_psoc4_sarmux_8__CH_5_PORT << 4u) |
                 (uint8)Infrared_ADC_cy_psoc4_sarmux_8__CH_5_PIN
            #endif /* End Infrared_ADC_TOTAL_CHANNELS_NUM > 5u */
            #if(Infrared_ADC_TOTAL_CHANNELS_NUM > 6u)
                ,(uint8)(Infrared_ADC_cy_psoc4_sarmux_8__CH_6_PORT << 4u) |
                 (uint8)Infrared_ADC_cy_psoc4_sarmux_8__CH_6_PIN
            #endif /* End Infrared_ADC_TOTAL_CHANNELS_NUM > 6u */
            #if(Infrared_ADC_TOTAL_CHANNELS_NUM > 7u)
                ,(uint8)(Infrared_ADC_cy_psoc4_sarmux_8__CH_7_PORT << 4u) |
                 (uint8)Infrared_ADC_cy_psoc4_sarmux_8__CH_7_PIN
            #endif /* End Infrared_ADC_TOTAL_CHANNELS_NUM > 7u */
            #if(Infrared_ADC_TOTAL_CHANNELS_NUM > 8u)
                ,(uint8)(Infrared_ADC_cy_psoc4_sarmux_8__CH_8_PORT << 4u) |
                 (uint8)Infrared_ADC_cy_psoc4_sarmux_8__CH_8_PIN
            #endif /* End Infrared_ADC_TOTAL_CHANNELS_NUM > 8u */
            #if(Infrared_ADC_TOTAL_CHANNELS_NUM > 9u)
                ,(uint8)(Infrared_ADC_cy_psoc4_sarmux_8__CH_9_PORT << 4u) |
                 (uint8)Infrared_ADC_cy_psoc4_sarmux_8__CH_9_PIN
            #endif /* End Infrared_ADC_TOTAL_CHANNELS_NUM > 9u */
            #if(Infrared_ADC_TOTAL_CHANNELS_NUM > 10u)
                ,(uint8)(Infrared_ADC_cy_psoc4_sarmux_8__CH_10_PORT << 4u) |
                 (uint8)Infrared_ADC_cy_psoc4_sarmux_8__CH_10_PIN
            #endif /* End Infrared_ADC_TOTAL_CHANNELS_NUM > 10u */
            #if(Infrared_ADC_TOTAL_CHANNELS_NUM > 11u)
                ,(uint8)(Infrared_ADC_cy_psoc4_sarmux_8__CH_11_PORT << 4u) |
                 (uint8)Infrared_ADC_cy_psoc4_sarmux_8__CH_11_PIN
            #endif /* End Infrared_ADC_TOTAL_CHANNELS_NUM > 11u */
            #if(Infrared_ADC_TOTAL_CHANNELS_NUM > 12u)
                ,(uint8)(Infrared_ADC_cy_psoc4_sarmux_8__CH_12_PORT << 4u) |
                 (uint8)Infrared_ADC_cy_psoc4_sarmux_8__CH_12_PIN
            #endif /* End Infrared_ADC_TOTAL_CHANNELS_NUM > 12u */
            #if(Infrared_ADC_TOTAL_CHANNELS_NUM > 13u)
                ,(uint8)(Infrared_ADC_cy_psoc4_sarmux_8__CH_13_PORT << 4u) |
                 (uint8)Infrared_ADC_cy_psoc4_sarmux_8__CH_13_PIN
            #endif /* End Infrared_ADC_TOTAL_CHANNELS_NUM > 13u */
            #if(Infrared_ADC_TOTAL_CHANNELS_NUM > 14u)
                ,(uint8)(Infrared_ADC_cy_psoc4_sarmux_8__CH_14_PORT << 4u) |
                 (uint8)Infrared_ADC_cy_psoc4_sarmux_8__CH_14_PIN
            #endif /* End Infrared_ADC_TOTAL_CHANNELS_NUM > 14u */
            #if(Infrared_ADC_TOTAL_CHANNELS_NUM > 15u)
                ,(uint8)(Infrared_ADC_cy_psoc4_sarmux_8__CH_15_PORT << 4u) |
                 (uint8)Infrared_ADC_cy_psoc4_sarmux_8__CH_15_PIN
            #endif /* End Infrared_ADC_TOTAL_CHANNELS_NUM > 15u */
            #if(Infrared_ADC_TOTAL_CHANNELS_NUM > 16u)
                ,(uint8)(Infrared_ADC_cy_psoc4_sarmux_8__CH_16_PORT << 4u) |
                 (uint8)Infrared_ADC_cy_psoc4_sarmux_8__CH_16_PIN
            #endif /* End Infrared_ADC_TOTAL_CHANNELS_NUM > 16u */
        };
    #endif /* End Infrared_ADC_TOTAL_CHANNELS_NUM > 1u */

    #if(Infrared_ADC_IRQ_REMOVE == 0u)
        /* Start and set interrupt vector */
        CyIntSetPriority(Infrared_ADC_INTC_NUMBER, Infrared_ADC_INTC_PRIOR_NUMBER);
        (void)CyIntSetVector(Infrared_ADC_INTC_NUMBER, &Infrared_ADC_ISR);
    #endif   /* End Infrared_ADC_IRQ_REMOVE */

    /* Init SAR and MUX registers */
    Infrared_ADC_SAR_CHAN_EN_REG = Infrared_ADC_DEFAULT_EN_CHANNELS;
    Infrared_ADC_SAR_CTRL_REG |= Infrared_ADC_DEFAULT_CTRL_REG_CFG | 
        /* Enable the SAR internal pump when global pump is enabled */
        (((Infrared_ADC_PUMP_CTRL_REG & Infrared_ADC_PUMP_CTRL_ENABLED) != 0u) ? 
        Infrared_ADC_BOOSTPUMP_EN : 0u);
    Infrared_ADC_SAR_SAMPLE_CTRL_REG = Infrared_ADC_DEFAULT_SAMPLE_CTRL_REG_CFG;
    Infrared_ADC_SAR_RANGE_THRES_REG = Infrared_ADC_DEFAULT_RANGE_THRES_REG_CFG;
    Infrared_ADC_SAR_RANGE_COND_REG  = Infrared_ADC_COMPARE_MODE;
    Infrared_ADC_SAR_SAMPLE_TIME01_REG = Infrared_ADC_DEFAULT_SAMPLE_TIME01_REG_CFG;
    Infrared_ADC_SAR_SAMPLE_TIME23_REG = Infrared_ADC_DEFAULT_SAMPLE_TIME23_REG_CFG;
    
    /* Connect Vm to VSSA when even one channel is single-ended or multiple channels configured */
    #if(Infrared_ADC_DEFAULT_MUX_SWITCH0 != 0u)
        Infrared_ADC_MUX_SWITCH0_REG |= Infrared_ADC_DEFAULT_MUX_SWITCH0;
        /* Set MUX_HW_CTRL_VSSA in MUX_SWITCH_HW_CTRL when multiple channels enabled */
        #if(Infrared_ADC_TOTAL_CHANNELS_NUM > 1u)
            Infrared_ADC_MUX_SWITCH_HW_CTRL_REG |= Infrared_ADC_DEFAULT_MUX_SWITCH0;
        #endif /* Infrared_ADC_TOTAL_CHANNELS_NUM > 1u */
    #endif /*Infrared_ADC_CHANNELS_MODE !=0 */

    Infrared_ADC_SAR_SATURATE_INTR_MASK_REG = 0u;
    Infrared_ADC_SAR_RANGE_INTR_MASK_REG = 0u;
    Infrared_ADC_SAR_INTR_MASK_REG = Infrared_ADC_SAR_INTR_MASK;

    #if(Infrared_ADC_CY_SAR_IP_VER == Infrared_ADC_CY_SAR_IP_VER0)
        Infrared_ADC_ANA_TRIM_REG = Infrared_ADC_TRIM_COEF;
    #endif /* (Infrared_ADC_CY_SAR_IP_VER == Infrared_ADC_CY_SAR_IP_VER0) */

    /* Read and modify default configuration based on characterization */
    tmpRegVal = Infrared_ADC_SAR_DFT_CTRL_REG;
    tmpRegVal &= (uint32)~Infrared_ADC_DCEN;
    
    #if(Infrared_ADC_CY_SAR_IP_VER == Infrared_ADC_CY_SAR_IP_VER0)
        #if(Infrared_ADC_NOMINAL_CLOCK_FREQ > (Infrared_ADC_MAX_FREQUENCY / 2))
            tmpRegVal |= Infrared_ADC_SEL_CSEL_DFT_CHAR;
        #else  /* clock speed < 9 Mhz */
            tmpRegVal |= Infrared_ADC_DLY_INC;
        #endif /* clock speed > 9 Mhz */
    #else
        #if ((Infrared_ADC_DEFAULT_VREF_SEL == Infrared_ADC__INTERNAL1024) || \
             (Infrared_ADC_DEFAULT_VREF_SEL == Infrared_ADC__INTERNALVREF))
            tmpRegVal |= Infrared_ADC_DLY_INC;
        #else
            tmpRegVal |= Infrared_ADC_DCEN;
            tmpRegVal &= (uint32)~Infrared_ADC_DLY_INC;
        #endif /* ((Infrared_ADC_DEFAULT_VREF_SEL == Infrared_ADC__INTERNAL1024) || \
                   (Infrared_ADC_DEFAULT_VREF_SEL == Infrared_ADC__INTERNALVREF)) */
    #endif /* (Infrared_ADC_CY_SAR_IP_VER == Infrared_ADC_CY_SAR_IP_VER0) */
    
    Infrared_ADC_SAR_DFT_CTRL_REG = tmpRegVal;

    #if(Infrared_ADC_MAX_RESOLUTION != Infrared_ADC_RESOLUTION_12)
        Infrared_ADC_WOUNDING_REG = Infrared_ADC_ALT_WOUNDING;
    #endif /* Infrared_ADC_MAX_RESOLUTION != Infrared_ADC_RESOLUTION_12 */

    for(chNum = 0u; chNum < Infrared_ADC_TOTAL_CHANNELS_NUM; chNum++)
    {
        tmpRegVal = (Infrared_ADC_channelsConfig[chNum] & Infrared_ADC_CHANNEL_CONFIG_MASK);
        #if(Infrared_ADC_TOTAL_CHANNELS_NUM > 1u)
            tmpRegVal |= Infrared_ADC_InputsPlacement[chNum];
        #endif /* End Infrared_ADC_TOTAL_CHANNELS_NUM > 1u */
        
        
        /* When the part is limited to 10-bit then the SUB_RESOLUTION bit
        *  will be ignored and the RESOLUTION bit selects between 10-bit
        *  (0) and 8-bit (1) resolution.
        */
        #if((Infrared_ADC_MAX_RESOLUTION != Infrared_ADC_RESOLUTION_12) && \
            (Infrared_ADC_ALT_WOUNDING == Infrared_ADC_WOUNDING_10BIT))
            tmpRegVal &= (uint32)(~Infrared_ADC_ALT_RESOLUTION_ON);
        #endif /* Infrared_ADC_MAX_RESOLUTION != Infrared_ADC_RESOLUTION_12 */

        #if(Infrared_ADC_INJ_CHANNEL_ENABLED)
        if(chNum < Infrared_ADC_SEQUENCED_CHANNELS_NUM)
        #endif /* Infrared_ADC_INJ_CHANNEL_ENABLED */
        {
            CY_SET_REG32((reg32 *)(Infrared_ADC_SAR_CHAN_CONFIG_IND + (uint32)(chNum << 2)), tmpRegVal);

            if((Infrared_ADC_channelsConfig[chNum] & Infrared_ADC_IS_SATURATE_EN_MASK) != 0u)
            {
                Infrared_ADC_SAR_SATURATE_INTR_MASK_REG |= (uint16)((uint16)1 << chNum);
            }

            if((Infrared_ADC_channelsConfig[chNum] & Infrared_ADC_IS_RANGE_CTRL_EN_MASK) != 0u)
            {
                Infrared_ADC_SAR_RANGE_INTR_MASK_REG |= (uint16)((uint16)1 << chNum);
            }
        }
        #if(Infrared_ADC_INJ_CHANNEL_ENABLED)
            else
            {
                CY_SET_REG32(Infrared_ADC_SAR_INJ_CHAN_CONFIG_PTR, tmpRegVal | Infrared_ADC_INJ_TAILGATING);

                if((Infrared_ADC_channelsConfig[chNum] & Infrared_ADC_IS_SATURATE_EN_MASK) != 0u)
                {
                    Infrared_ADC_SAR_INTR_MASK_REG |= Infrared_ADC_INJ_SATURATE_MASK;
                }

                if((Infrared_ADC_channelsConfig[chNum] & Infrared_ADC_IS_RANGE_CTRL_EN_MASK) != 0u)
                {
                    Infrared_ADC_SAR_INTR_MASK_REG |= Infrared_ADC_INJ_RANGE_MASK;
                }
            }
        #endif /* Infrared_ADC_INJ_CHANNEL_ENABLED */

        if((Infrared_ADC_channelsConfig[chNum] & Infrared_ADC_ALT_RESOLUTION_ON) != 0u)
        {
               counts = (int32)Infrared_ADC_DEFAULT_MAX_WRK_ALT;
        }
        else
        {
               counts = (int32)Infrared_ADC_SAR_WRK_MAX_12BIT;
        }

        if((Infrared_ADC_channelsConfig[chNum] & Infrared_ADC_DIFFERENTIAL_EN) == 0u)
        {
            #if((Infrared_ADC_DEFAULT_SE_RESULT_FORMAT_SEL == Infrared_ADC__FSIGNED) && \
                (Infrared_ADC_DEFAULT_NEG_INPUT_SEL == Infrared_ADC__VREF))
                /* Set offset to the minus half scale to convert results to unsigned format */
                Infrared_ADC_offset[chNum] = (int16)(counts / -2);
            #else
                Infrared_ADC_offset[chNum] = 0;
            #endif /* end DEFAULT_SE_RESULT_FORMAT_SEL == Infrared_ADC__FSIGNED */
        }
        else    /* Differential channel */
        {
            #if(Infrared_ADC_DEFAULT_DIFF_RESULT_FORMAT_SEL == Infrared_ADC__FUNSIGNED)
                /* Set offset to the half scale to convert results to signed format */
                Infrared_ADC_offset[chNum] = (int16)(counts / 2);
            #else
                Infrared_ADC_offset[chNum] = 0;
            #endif /* end Infrared_ADC_DEFAULT_DIFF_RESULT_FORMAT_SEL == Infrared_ADC__FUNSIGNED */
        }
        /* Calculate gain in counts per 10 volts with rounding */
        Infrared_ADC_countsPer10Volt[chNum] = (int16)(((counts * Infrared_ADC_10MV_COUNTS) +
                            Infrared_ADC_DEFAULT_VREF_MV_VALUE) / (Infrared_ADC_DEFAULT_VREF_MV_VALUE * 2));
    }
}

/*******************************************************************************
* Function Name: ADC_SAR_1_Enable
********************************************************************************
*
* Summary:
*  Enables the clock and analog power for SAR ADC.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void Infrared_ADC_Enable(void)
{
    if (0u == (Infrared_ADC_SAR_CTRL_REG & Infrared_ADC_ENABLE))
    {
        #if(Infrared_ADC_CY_SAR_IP_VER != Infrared_ADC_CY_SAR_IP_VER0)

            while (0u != (Infrared_ADC_SAR_STATUS_REG & Infrared_ADC_STATUS_BUSY))
            {
                /* wait for SAR to go idle to avoid deadlock */
            }
        #endif /* (Infrared_ADC_CY_SAR_IP_VER != Infrared_ADC_CY_SAR_IP_VER0) */
        
        Infrared_ADC_SAR_CTRL_REG |= Infrared_ADC_ENABLE;
        
        /* The block is ready to use 10 us after the enable signal is set high. */
        CyDelayUs(Infrared_ADC_10US_DELAY);         
    }
}


/*******************************************************************************
* Function Name: Infrared_ADC_Stop
********************************************************************************
*
* Summary:
*  This function stops ADC conversions and puts the ADC into its lowest power
*  mode.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void Infrared_ADC_Stop(void)
{
    Infrared_ADC_SAR_CTRL_REG &= (uint32)~Infrared_ADC_ENABLE;
}


/*******************************************************************************
* Function Name: Infrared_ADC_StartConvert
********************************************************************************
*
* Summary:
*  Description:
*  For free running mode, this API starts the conversion process and it
*  runs continuously.
*
*  In a triggered mode, this routine triggers every conversion by
*  writing into the FW_TRIGGER bit in SAR_START_CTRL reg. In triggered mode,
*  every conversion has to start by this API.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void Infrared_ADC_StartConvert(void)
{
    #if(Infrared_ADC_DEFAULT_SAMPLE_MODE_SEL == Infrared_ADC__FREERUNNING)
        Infrared_ADC_SAR_SAMPLE_CTRL_REG |= Infrared_ADC_CONTINUOUS_EN;
    #else /* Firmware trigger */
        Infrared_ADC_SAR_START_CTRL_REG = Infrared_ADC_FW_TRIGGER;
    #endif /* End Infrared_ADC_DEFAULT_SAMPLE_MODE == Infrared_ADC__FREERUNNING */

}


/*******************************************************************************
* Function Name: Infrared_ADC_StopConvert
********************************************************************************
*
* Summary:
*  Forces the ADC to stop all conversions.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void Infrared_ADC_StopConvert(void)
{
    #if(Infrared_ADC_DEFAULT_SAMPLE_MODE_SEL == Infrared_ADC__FREERUNNING)
        Infrared_ADC_SAR_SAMPLE_CTRL_REG &= (uint32)(~Infrared_ADC_CONTINUOUS_EN);
    #endif /* Infrared_ADC_DEFAULT_SAMPLE_MODE == Infrared_ADC__FREERUNNING */
}


/*******************************************************************************
* Function Name: Infrared_ADC_IsEndConversion
********************************************************************************
*
* Summary:
*  Description: Checks for ADC end of conversion for the case one
*  channel and end of scan for the case of multiple channels. It acts
*  as a software version of the EOC. This function provides the
*  programmer with two options. In one mode this function
*  immediately returns with the conversion status. In the other mode,
*  the function does not return (blocking) until the conversion has
*  completed.
*
* Parameters:
*  ADC_RETURN_STATUS        -> Immediately returns conversion result status
*  ADC_WAIT_FOR_RESULT      -> Does not return until ADC complete
*  ADC_RETURN_STATUS_INJ    -> Immediately returns conversion result status
*                              for injection channel
*  ADC_WAIT_FOR_RESULT_INJ  -> Does not return until ADC completes injection
*                              channel conversion
*
* Return:
*  If a non-zero value is returned, the last conversion or scan has completed.
*  If the returned value is zero, the ADC is still in the process of a scan.
*
*******************************************************************************/
uint32 Infrared_ADC_IsEndConversion(uint32 retMode)
{
    uint32 status = 0u;

    if((retMode & (Infrared_ADC_RETURN_STATUS | Infrared_ADC_WAIT_FOR_RESULT)) != 0u)
    {
        do
        {
            status = Infrared_ADC_SAR_INTR_REG & Infrared_ADC_EOS_MASK;
        }while((status == 0u) && ((retMode & Infrared_ADC_WAIT_FOR_RESULT) != 0u));

        if(status != 0u)
        {
            /* Clear EOS bit */
            Infrared_ADC_SAR_INTR_REG = Infrared_ADC_EOS_MASK;
        }
    }

    #if(Infrared_ADC_INJ_CHANNEL_ENABLED)
        if((retMode & (Infrared_ADC_RETURN_STATUS_INJ | Infrared_ADC_WAIT_FOR_RESULT_INJ)) != 0u)
        {
            do
            {
                status |= Infrared_ADC_SAR_INTR_REG & Infrared_ADC_INJ_EOC_MASK;
            }while(((status & Infrared_ADC_INJ_EOC_MASK) == 0u) &&
                   ((retMode & Infrared_ADC_WAIT_FOR_RESULT_INJ) != 0u));

            if((status & Infrared_ADC_INJ_EOC_MASK) != 0u)
            {
                /* Clear Injection EOS bit */
                Infrared_ADC_SAR_INTR_REG = Infrared_ADC_INJ_EOC_MASK;
            }
        }
    #endif /* Infrared_ADC_INJ_CHANNEL_ENABLED */

    return (status);
}


/*******************************************************************************
* Function Name: Infrared_ADC_GetResult16
********************************************************************************
*
* Summary:
*  Gets the data available in the SAR DATA register.
*
* Parameters:
*  chan: The ADC channel in which to return the result. The first channel
*  is 0 and the injection channel if enabled is the number of valid channels.
*
* Return:
*  Returns converted data as a signed 16-bit integer
*
*******************************************************************************/
int16 Infrared_ADC_GetResult16(uint32 chan)
{
    uint32 result;

    /* Halt CPU in debug mode if channel is out of valid range */
    CYASSERT(chan < Infrared_ADC_TOTAL_CHANNELS_NUM);

    if(chan < Infrared_ADC_SEQUENCED_CHANNELS_NUM)
    {
        result = CY_GET_REG32((reg32 *)(Infrared_ADC_SAR_CHAN_RESULT_IND + (uint32)(chan << 2u))) &
                Infrared_ADC_RESULT_MASK;
    }
    else
    {
        #if(Infrared_ADC_INJ_CHANNEL_ENABLED)
            result = Infrared_ADC_SAR_INJ_RESULT_REG & Infrared_ADC_RESULT_MASK;
        #else
            result = 0u;
        #endif /* Infrared_ADC_INJ_CHANNEL_ENABLED */
    }

    return ( (int16)result );
}


/*******************************************************************************
* Function Name: Infrared_ADC_SetChanMask
********************************************************************************
*
* Summary:
*  Sets the channel enable mask.
*
* Parameters:
*  mask: Sets which channels that will be
*  scanned. Setting bits for channels that do not exist will have no
*  effect. For example, if only 6 channels were enabled, setting a
*  mask of 0x0103 would only enable the last two channels (0 and 1).
*  This API will not enable the injection channel.
*  Examples: If the component is setup to sequence through 8
*  channels, a mask of 0x000F would enable channels 0, 1, 2, and 3.
*
* Return:
*  None.
*
*******************************************************************************/
void Infrared_ADC_SetChanMask(uint32 mask)
{
    Infrared_ADC_SAR_CHAN_EN_REG = mask & Infrared_ADC_MAX_CHANNELS_EN_MASK;
}

#if(Infrared_ADC_INJ_CHANNEL_ENABLED)


    /*******************************************************************************
    * Function Name: Infrared_ADC_EnableInjection
    ********************************************************************************
    *
    * Summary:
    *  Enables the injection channel for the next scan only.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void Infrared_ADC_EnableInjection(void)
    {
        Infrared_ADC_SAR_INJ_CHAN_CONFIG_REG |= Infrared_ADC_INJ_CHAN_EN;
    }

#endif /* Infrared_ADC_INJ_CHANNEL_ENABLED */


/*******************************************************************************
* Function Name: Infrared_ADC_SetLowLimit
********************************************************************************
*
* Summary:
*  Sets the low limit parameter for a limit condition.
*
* Parameters:
*  lowLimit: The low limit for a limit condition.
*
* Return:
*  None.
*
*******************************************************************************/
void Infrared_ADC_SetLowLimit(uint32 lowLimit)
{
    Infrared_ADC_SAR_RANGE_THRES_REG &= (uint32)(~Infrared_ADC_RANGE_LOW_MASK);
    Infrared_ADC_SAR_RANGE_THRES_REG |= lowLimit & Infrared_ADC_RANGE_LOW_MASK;
}


/*******************************************************************************
* Function Name: Infrared_ADC_SetHighLimit
********************************************************************************
*
* Summary:
*  Sets the low limit parameter for a limit condition.
*
* Parameters:
*  highLimit: The high limit for a limit condition.
*
* Return:
*  None.
*
*******************************************************************************/
void Infrared_ADC_SetHighLimit(uint32 highLimit)
{
    Infrared_ADC_SAR_RANGE_THRES_REG &= (uint32)(~Infrared_ADC_RANGE_HIGH_MASK);
    Infrared_ADC_SAR_RANGE_THRES_REG |= (uint32)(highLimit << Infrared_ADC_RANGE_HIGH_OFFSET);
}


/*******************************************************************************
* Function Name: Infrared_ADC_SetLimitMask
********************************************************************************
*
* Summary:
*  Sets the channel limit condition mask.
*
* Parameters:
*  mask: Sets which channels that may cause a
*  limit condition interrupt. Setting bits for channels that do not exist
*  will have no effect. For example, if only 6 channels were enabled,
*  setting a mask of 0x0103 would only enable the last two channels (0 and 1).
*
* Return:
*  None.
*
*******************************************************************************/
void Infrared_ADC_SetLimitMask(uint32 mask)
{
    Infrared_ADC_SAR_RANGE_INTR_MASK_REG = mask & Infrared_ADC_MAX_CHANNELS_EN_MASK;
}


/*******************************************************************************
* Function Name: Infrared_ADC_SetSatMask
********************************************************************************
*
* Summary:
*  Sets the channel saturation event mask.
*
* Parameters:
*  mask: Sets which channels that may cause a
*  saturation event interrupt. Setting bits for channels that do not exist
*  will have no effect. For example, if only 8 channels were enabled,
*  setting a mask of 0x01C0 would only enable two channels (6 and 7).
*
* Return:
*  None.
*
*******************************************************************************/
void Infrared_ADC_SetSatMask(uint32 mask)
{
    Infrared_ADC_SAR_SATURATE_INTR_MASK_REG = mask & Infrared_ADC_MAX_CHANNELS_EN_MASK;
}


/*******************************************************************************
* Function Name: Infrared_ADC_SetOffset
********************************************************************************
*
* Summary:
*   Description: Sets the ADC offset which is used by the functions
*   ADC_CountsTo_uVolts, ADC_CountsTo_mVolts and ADC_CountsTo_Volts
*   to substract the offset from the given reading
*   before calculating the voltage conversion.
*
* Parameters:
*  chan: ADC channel number.
*  offset: This value is a measured value when the
*          inputs are shorted or connected to the same input voltage.
*
* Return:
*  None.
*
* Global variables:
*  Infrared_ADC_Offset:  Modified to set the user provided offset.
*
*******************************************************************************/
void Infrared_ADC_SetOffset(uint32 chan, int16 offset)
{
    /* Halt CPU in debug mode if channel is out of valid range */
    CYASSERT(chan < Infrared_ADC_TOTAL_CHANNELS_NUM);

    Infrared_ADC_offset[chan] = offset;
}


/*******************************************************************************
* Function Name: Infrared_ADC_SetGain
********************************************************************************
*
* Summary:
*  Description: Sets the ADC gain in counts per 10 volt for the voltage
*  conversion functions below. This value is set by default by the
*  reference and input range settings. It should only be used to further
*  calibrate the ADC with a known input or if an external reference is
*  used. Affects the ADC_CountsTo_uVolts, ADC_CountsTo_mVolts
*  and ADC_CountsTo_Volts functions by supplying the correct
*  conversion between ADC counts and voltage.
*
* Parameters:
*  chan: ADC channel number.
*  adcGain: ADC gain in counts per 10 volts.
*
* Return:
*  None.
*
* Global variables:
*  Infrared_ADC_CountsPer10Volt:  modified to set the ADC gain in counts
*   per 10 volt.
*
*******************************************************************************/
void Infrared_ADC_SetGain(uint32 chan, int32 adcGain)
{
    /* Halt CPU in debug mode if channel is out of valid range */
    CYASSERT(chan < Infrared_ADC_TOTAL_CHANNELS_NUM);

    Infrared_ADC_countsPer10Volt[chan] = adcGain;
}


#if(Infrared_ADC_DEFAULT_JUSTIFICATION_SEL == Infrared_ADC__RIGHT)


    /*******************************************************************************
    * Function Name: Infrared_ADC_CountsTo_mVolts
    ********************************************************************************
    *
    * Summary:
    *  This function converts ADC counts to mVolts
    *  This function is not available when left data format justification selected.
    *
    * Parameters:
    *  chan: The ADC channel number.
    *  adcCounts: Result from the ADC conversion
    *
    * Return:
    *  Results in mVolts
    *
    * Global variables:
    *  Infrared_ADC_countsPer10Volt:  used to convert ADC counts to mVolts.
    *  Infrared_ADC_Offset:  Used as the offset while converting ADC counts
    *   to mVolts.
    *
    *******************************************************************************/
    int16 Infrared_ADC_CountsTo_mVolts(uint32 chan, int16 adcCounts)
    {
        int16 mVolts;

        /* Halt CPU in debug mode if channel is out of valid range */
        CYASSERT(chan < Infrared_ADC_TOTAL_CHANNELS_NUM);

        /* Divide the adcCount when accumulate averaging mode selected */
        #if(Infrared_ADC_DEFAULT_AVG_MODE == Infrared_ADC__ACCUMULATE)
            if((Infrared_ADC_channelsConfig[chan] & Infrared_ADC_AVERAGING_EN) != 0u)
            {
                adcCounts /= Infrared_ADC_DEFAULT_AVG_SAMPLES_DIV;
            }
        #endif /* Infrared_ADC_DEFAULT_AVG_MODE == Infrared_ADC__ACCUMULATE */

        /* Subtract ADC offset */
        adcCounts -= Infrared_ADC_offset[chan];

        mVolts = (int16)((((int32)adcCounts * Infrared_ADC_10MV_COUNTS) + ( (adcCounts > 0) ?
                 (Infrared_ADC_countsPer10Volt[chan] / 2) : (-(Infrared_ADC_countsPer10Volt[chan] / 2)) ))
                 / Infrared_ADC_countsPer10Volt[chan]);

        return( mVolts );
    }


    /*******************************************************************************
    * Function Name: Infrared_ADC_CountsTo_uVolts
    ********************************************************************************
    *
    * Summary:
    *  This function converts ADC counts to micro Volts
    *  This function is not available when left data format justification selected.
    *
    * Parameters:
    *  chan: The ADC channel number.
    *  adcCounts: Result from the ADC conversion
    *
    * Return:
    *  Results in uVolts
    *
    * Global variables:
    *  Infrared_ADC_countsPer10Volt:  used to convert ADC counts to uVolts.
    *  Infrared_ADC_Offset:  Used as the offset while converting ADC counts
    *   to mVolts.
    *
    * Theory:
    *  Care must be taken to not exceed the maximum value for a 31 bit signed
    *  number in the conversion to uVolts and at the same time not loose
    *  resolution.
    *  To convert adcCounts to microVolts it is required to be multiplied
    *  on 10 million and later divide on gain in counts per 10V.
    *
    *******************************************************************************/
    int32 Infrared_ADC_CountsTo_uVolts(uint32 chan, int16 adcCounts)
    {
        int64 uVolts;

        /* Halt CPU in debug mode if channel is out of valid range */
        CYASSERT(chan < Infrared_ADC_TOTAL_CHANNELS_NUM);

        /* Divide the adcCount when accumulate averaging mode selected */
        #if(Infrared_ADC_DEFAULT_AVG_MODE == Infrared_ADC__ACCUMULATE)
            if((Infrared_ADC_channelsConfig[chan] & Infrared_ADC_AVERAGING_EN) != 0u)
            {
                adcCounts /= Infrared_ADC_DEFAULT_AVG_SAMPLES_DIV;
            }
        #endif /* Infrared_ADC_DEFAULT_AVG_MODE == Infrared_ADC__ACCUMULATE */

        /* Subtract ADC offset */
        adcCounts -= Infrared_ADC_offset[chan];

        uVolts = ((int64)adcCounts * Infrared_ADC_10UV_COUNTS) / Infrared_ADC_countsPer10Volt[chan];

        return( (int32)uVolts );
    }


    /*******************************************************************************
    * Function Name: Infrared_ADC_CountsTo_Volts
    ********************************************************************************
    *
    * Summary:
    *  Converts the ADC output to Volts as a floating point number.
    *  This function is not available when left data format justification selected.
    *
    * Parameters:
    *  chan: The ADC channel number.
    *  Result from the ADC conversion
    *
    * Return:
    *  Results in Volts
    *
    * Global variables:
    *  Infrared_ADC_countsPer10Volt:  used to convert ADC counts to Volts.
    *  Infrared_ADC_Offset:  Used as the offset while converting ADC counts
    *   to mVolts.
    *
    *******************************************************************************/
    float32 Infrared_ADC_CountsTo_Volts(uint32 chan, int16 adcCounts)
    {
        float32 volts;

        /* Halt CPU in debug mode if channel is out of valid range */
        CYASSERT(chan < Infrared_ADC_TOTAL_CHANNELS_NUM);

        /* Divide the adcCount when accumulate averaging mode selected */
        #if(Infrared_ADC_DEFAULT_AVG_MODE == Infrared_ADC__ACCUMULATE)
            if((Infrared_ADC_channelsConfig[chan] & Infrared_ADC_AVERAGING_EN) != 0u)
            {
                adcCounts /= Infrared_ADC_DEFAULT_AVG_SAMPLES_DIV;
            }
        #endif /* Infrared_ADC_DEFAULT_AVG_MODE == Infrared_ADC__ACCUMULATE */

        /* Subtract ADC offset */
        adcCounts -= Infrared_ADC_offset[chan];

        volts = ((float32)adcCounts * Infrared_ADC_10V_COUNTS) / (float32)Infrared_ADC_countsPer10Volt[chan];

        return( volts );
    }

#endif /* End Infrared_ADC_DEFAULT_JUSTIFICATION_SEL == Infrared_ADC__RIGHT */


/* [] END OF FILE */
