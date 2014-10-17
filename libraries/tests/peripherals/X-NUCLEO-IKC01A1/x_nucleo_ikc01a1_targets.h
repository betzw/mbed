/* Copyright (C) STMicroelectronics Srl 2014 */
/*
 * @(#)x_nucleo_ikc01a1_targets.h
 *
 * This header file is intended to manage the differences between 
 * the different supported base-boards which might mount the
 * X_NUCLEO_IKC01A1 battery management expansion board.
 *
 */

/* Define to prevent from recursive inclusion --------------------------------*/
#ifndef _X_NUCLEO_IKC01A1_TARGETS_H_
#define _X_NUCLEO_IKC01A1_TARGETS_H_

/* Macros --------------------------------------------------------------------*/
#ifdef TARGET_NUCLEO_F401RE
#define CHARGER_PIN_ST1          (PC_2)
#define CHARGER_PIN_ST2          (PC_3) 
#define CHARGER_PIN_DISCHARGE    (PB_0)
#else // !TARGET_NUCLEO_F401RE
#error "Platform not supported!"
#endif // !TARGET_NUCLEO_F401RE

#endif // _X_NUCLEO_IKC01A1_TARGETS_H_
