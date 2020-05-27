/*******************************************************************************
* File Name: INT_LIS3DH.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_INT_LIS3DH_H) /* Pins INT_LIS3DH_H */
#define CY_PINS_INT_LIS3DH_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "INT_LIS3DH_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 INT_LIS3DH__PORT == 15 && ((INT_LIS3DH__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    INT_LIS3DH_Write(uint8 value);
void    INT_LIS3DH_SetDriveMode(uint8 mode);
uint8   INT_LIS3DH_ReadDataReg(void);
uint8   INT_LIS3DH_Read(void);
void    INT_LIS3DH_SetInterruptMode(uint16 position, uint16 mode);
uint8   INT_LIS3DH_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the INT_LIS3DH_SetDriveMode() function.
     *  @{
     */
        #define INT_LIS3DH_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define INT_LIS3DH_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define INT_LIS3DH_DM_RES_UP          PIN_DM_RES_UP
        #define INT_LIS3DH_DM_RES_DWN         PIN_DM_RES_DWN
        #define INT_LIS3DH_DM_OD_LO           PIN_DM_OD_LO
        #define INT_LIS3DH_DM_OD_HI           PIN_DM_OD_HI
        #define INT_LIS3DH_DM_STRONG          PIN_DM_STRONG
        #define INT_LIS3DH_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define INT_LIS3DH_MASK               INT_LIS3DH__MASK
#define INT_LIS3DH_SHIFT              INT_LIS3DH__SHIFT
#define INT_LIS3DH_WIDTH              1u

/* Interrupt constants */
#if defined(INT_LIS3DH__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in INT_LIS3DH_SetInterruptMode() function.
     *  @{
     */
        #define INT_LIS3DH_INTR_NONE      (uint16)(0x0000u)
        #define INT_LIS3DH_INTR_RISING    (uint16)(0x0001u)
        #define INT_LIS3DH_INTR_FALLING   (uint16)(0x0002u)
        #define INT_LIS3DH_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define INT_LIS3DH_INTR_MASK      (0x01u) 
#endif /* (INT_LIS3DH__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define INT_LIS3DH_PS                     (* (reg8 *) INT_LIS3DH__PS)
/* Data Register */
#define INT_LIS3DH_DR                     (* (reg8 *) INT_LIS3DH__DR)
/* Port Number */
#define INT_LIS3DH_PRT_NUM                (* (reg8 *) INT_LIS3DH__PRT) 
/* Connect to Analog Globals */                                                  
#define INT_LIS3DH_AG                     (* (reg8 *) INT_LIS3DH__AG)                       
/* Analog MUX bux enable */
#define INT_LIS3DH_AMUX                   (* (reg8 *) INT_LIS3DH__AMUX) 
/* Bidirectional Enable */                                                        
#define INT_LIS3DH_BIE                    (* (reg8 *) INT_LIS3DH__BIE)
/* Bit-mask for Aliased Register Access */
#define INT_LIS3DH_BIT_MASK               (* (reg8 *) INT_LIS3DH__BIT_MASK)
/* Bypass Enable */
#define INT_LIS3DH_BYP                    (* (reg8 *) INT_LIS3DH__BYP)
/* Port wide control signals */                                                   
#define INT_LIS3DH_CTL                    (* (reg8 *) INT_LIS3DH__CTL)
/* Drive Modes */
#define INT_LIS3DH_DM0                    (* (reg8 *) INT_LIS3DH__DM0) 
#define INT_LIS3DH_DM1                    (* (reg8 *) INT_LIS3DH__DM1)
#define INT_LIS3DH_DM2                    (* (reg8 *) INT_LIS3DH__DM2) 
/* Input Buffer Disable Override */
#define INT_LIS3DH_INP_DIS                (* (reg8 *) INT_LIS3DH__INP_DIS)
/* LCD Common or Segment Drive */
#define INT_LIS3DH_LCD_COM_SEG            (* (reg8 *) INT_LIS3DH__LCD_COM_SEG)
/* Enable Segment LCD */
#define INT_LIS3DH_LCD_EN                 (* (reg8 *) INT_LIS3DH__LCD_EN)
/* Slew Rate Control */
#define INT_LIS3DH_SLW                    (* (reg8 *) INT_LIS3DH__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define INT_LIS3DH_PRTDSI__CAPS_SEL       (* (reg8 *) INT_LIS3DH__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define INT_LIS3DH_PRTDSI__DBL_SYNC_IN    (* (reg8 *) INT_LIS3DH__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define INT_LIS3DH_PRTDSI__OE_SEL0        (* (reg8 *) INT_LIS3DH__PRTDSI__OE_SEL0) 
#define INT_LIS3DH_PRTDSI__OE_SEL1        (* (reg8 *) INT_LIS3DH__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define INT_LIS3DH_PRTDSI__OUT_SEL0       (* (reg8 *) INT_LIS3DH__PRTDSI__OUT_SEL0) 
#define INT_LIS3DH_PRTDSI__OUT_SEL1       (* (reg8 *) INT_LIS3DH__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define INT_LIS3DH_PRTDSI__SYNC_OUT       (* (reg8 *) INT_LIS3DH__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(INT_LIS3DH__SIO_CFG)
    #define INT_LIS3DH_SIO_HYST_EN        (* (reg8 *) INT_LIS3DH__SIO_HYST_EN)
    #define INT_LIS3DH_SIO_REG_HIFREQ     (* (reg8 *) INT_LIS3DH__SIO_REG_HIFREQ)
    #define INT_LIS3DH_SIO_CFG            (* (reg8 *) INT_LIS3DH__SIO_CFG)
    #define INT_LIS3DH_SIO_DIFF           (* (reg8 *) INT_LIS3DH__SIO_DIFF)
#endif /* (INT_LIS3DH__SIO_CFG) */

/* Interrupt Registers */
#if defined(INT_LIS3DH__INTSTAT)
    #define INT_LIS3DH_INTSTAT            (* (reg8 *) INT_LIS3DH__INTSTAT)
    #define INT_LIS3DH_SNAP               (* (reg8 *) INT_LIS3DH__SNAP)
    
	#define INT_LIS3DH_0_INTTYPE_REG 		(* (reg8 *) INT_LIS3DH__0__INTTYPE)
#endif /* (INT_LIS3DH__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_INT_LIS3DH_H */


/* [] END OF FILE */
