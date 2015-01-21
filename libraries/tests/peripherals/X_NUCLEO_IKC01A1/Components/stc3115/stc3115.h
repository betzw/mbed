/**
 ******************************************************************************
 * @file    stc3115.h
 * @author  AST / EST
 * @version V0.0.1
 * @date    22-October-2014
 * @brief   This file contains the common defines and functions prototypes for
 *          the stc3115.cpp driver.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */ 

/* Define to prevent from recursive inclusion --------------------------------*/
#ifndef __STC3115_H
#define __STC3115_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#include "mbed.h"
#include "stc3115_Battery.h"
#include "stc3115_Driver.h"
#include "../../x_nucleo_ikc01a1_i2c.h"
#include "../Common/GasGauge.h"

/* Macros --------------------------------------------------------------------*/
#define VoltageFactor  9011      /* LSB=2.20mV ~9011/4096 - convert to mV */        

/* Classes  ------------------------------------------------------------------*/
/** Implementation of STM's GasGauge component STC3115
 */
class STC3115 : public GasGauge {
 public:
	STC3115(DevI2C &i2c);
 
	virtual int Task(void);
	virtual void Reset(void);
	virtual int Stop(void);

	virtual int GetSOC(void) { return (BatteryData.SOC+5)/10; }
	virtual int GetOCV(void) { return BatteryData.OCV; }
	virtual int GetCurrent(void) { return BatteryData.Current; }
	virtual int GetTemperature(void) { return BatteryData.Temperature; }
	virtual int GetVoltage(void) { return BatteryData.Voltage; }
	virtual int GetChargeValue(void) { return BatteryData.ChargeValue; }
	virtual int GetPresence(void) { return BatteryData.Presence; }
	virtual int GetAlarmStatus(void) { return (BatteryData.status >> 13) & 0x3; }
	virtual int GetITState(void) { return !alm; }

	virtual int AlarmSetVoltageThreshold(int VoltThresh) {
		int max_value = (0xFF * 17.6);
		if(VoltThresh > max_value) VoltThresh = max_value;
		return AlarmSetVoTh(VoltThresh);
	}
	virtual int AlarmSetSOCThreshold(int SOCThresh) {  
		int max_value = (int)(0xFF * 0.5);
		if(SOCThresh > max_value) SOCThresh = max_value;
		return AlarmSetSOCTh(SOCThresh);
	}

	/**
	 * @brief Return the current ALM status (from register)
	 * @param None
	 * @retval ALM status 00 : no alarm
	 *                    01 : SOC alarm
	 *                    10 : Voltage alarm
	 *                    11 : SOC and voltage alarm
	 */
	virtual int GetIT(void) {  
		int res;
		
		/* Read the mode register*/
		res = ReadByte(STC3115_REG_CTRL);
		res = res >> 5;
		
		return (res & 0x3);
	}

	/**
	 * @brief Enable alarm functionality
	 * @param None
	 * @retval error status (OK, !OK)
	 */
	virtual int EnableIT(void) {  
		int res;
		
		/* Read the mode register*/
		res = ReadByte(STC3115_REG_MODE);
		
		/* Set the ALM_ENA bit to 1 */
		res = WriteByte(STC3115_REG_MODE, (res | STC3115_ALM_ENA));
		if (res!= OK) return (!OK);
		
		return (res);
	}

	/**
	 * @brief Stop alarm functionality
	 * @param None
	 * @retval error status (OK, !OK)
	 */
	virtual int DisableIT(void) {  
		int res;
		
		/* Read the mode register*/
		res = ReadByte(STC3115_REG_MODE);
		
		/* Set the ALM_ENA bit to 0 */
		res = WriteByte(STC3115_REG_MODE, (res & ~STC3115_ALM_ENA));
		if (res!= OK) return (!OK);
		
		return (OK);
	}

	/**
	 * @brief Clear the alarm signals
	 * @param None
	 * @retval error status (OK, !OK)
	 */
	virtual int ClearIT(void) {  
		int res;
		
		/* clear ALM bits*/
		res = WriteByte(STC3115_REG_CTRL, 0x01);
		if (res!= OK) return (!OK);
		
		return (res);
	}

	/** 
	 *  @brief Attach a function to call when a falling edge occurs on the input
	 *  @param fptr A pointer to a void function, or 0 to set as none
	 */
	virtual void AttachIT(void (*fptr)(void)) {
		alm.fall(fptr);
	}

 private:
	InterruptIn alm;
	DevI2C &dev_i2c;

 protected:
	union {
		uint8_t db[RAM_SIZE];  /* last byte holds the CRC */
		struct {
			uint16_t TstWord;     /* 0-1 */
			uint16_t HRSOC;       /* 2-3 SOC backup in (1/512%) */
			uint16_t CC_cnf;      /* 4-5 current CC_cnf */
			uint16_t VM_cnf;      /* 6-7 current VM_cnf */
			uint8_t  SOC;              /* 8 SOC (in %) */
			uint8_t  STC3115_Status;   /* 9  STC3115 working state */
			/* bytes ..RAM_SIZE-2 are free, last byte RAM_SIZE-1 is the CRC */
		} reg __attribute__((packed));
	} RAMData;

	STC3115_ConfigData_TypeDef ConfigData;
	STC3115_BatteryData_TypeDef BatteryData;

	/**
	 * @brief Calculate the CRC8
	 * @param pointer to byte array, 
	 * @param n: number of bytes
	 * @retval CRC value
	 */
	static int CalcRamCRC8(unsigned char *data, int n)
	{
		int crc=0;   /* initial value */
		int i, j;
		
		for (i=0;i<n;i++)
			{
				crc ^= data[i];
				for (j=0;j<8;j++) 
					{
						crc <<= 1;
						if (crc & 0x100)  crc ^= 7;
					}
			}
		return(crc & 255);
	}
	
	/**
	 * @brief Convert a raw 16-bit value from STC3115 registers into user units (mA, mAh, mV, °C)
	 * @param value 
	 * @param factor 
	 * @retval Converted value (result = value * factor / 4096)
	 */
	static int conv(short value, unsigned short factor)
	{
		int v;
		
		v= ( (long) value * factor ) >> 11;
		v= (v+1)/2;
		
		return (v);
	}

	/**
	 * @brief utility function to read data from STC3115
	 * @param  pBuffer: pointer to data to be read.
	 * @param  RegisterAddr: specifies internal address register to read from.
	 * @param  NumByteToRead: number of bytes to be read.
	 * @retval 0 if ok, -1 if an I2C error has occured
	 */
	int ReadData(uint8_t* pBuffer, uint8_t RegisterAddr, uint16_t NumByteToRead)
	{
		return dev_i2c.i2c_read(pBuffer,
					STC3115_SLAVE_ADDRESS,
					RegisterAddr,
					NumByteToRead);
	}
	
	/**
	 * @brief utility function to write data to STC3115
	 * @param  pBuffer: pointer to buffer to be filled.
	 * @param  RegisterAddr: specifies internal address register to read from.
	 * @param  NumByteToWrite: number of bytes to write.
	 * @retval 0 if ok, -1 if an I2C error has occured
	 */
	int WriteData(uint8_t* pBuffer, uint8_t RegisterAddr, uint16_t NumByteToWrite)
	{
		return dev_i2c.i2c_write(pBuffer,
					 STC3115_SLAVE_ADDRESS,
					 RegisterAddr,
					 NumByteToWrite);
	}
	
	/** 
	 * @brief Utility function to read the value stored in one register
	 * @param  RegAddress STC3115 register to read
	 * @retval 8-bit value
	 * @note  calls 'error()' in case of I2C error
	 */
	int ReadByte(int RegAddress)
	{
		int value;
		unsigned char data[2];
		int res;

		res = ReadData(data, RegAddress, 1);
		if(res) {
			error("I2C error in %s\n", __func__);
			return 0;
		}
		
		/* no error */
		value = data[0];

		return(value);
	}

	/**
	 * @brief utility function to write a 8-bit value into a register
	 * @param RegAddress STC3115 register
	 * @param Value 8-bit value to write
	 * @retval 0 if ok, -1 if an I2C error has occured
	 */
	int WriteByte(int RegAddress, unsigned char Value)
	{
		int res;
		unsigned char data[2];

		data[0]= Value;
		res = WriteData(data, RegAddress, 1);

		return(res);
	}

	/**
	 * @brief Utility function to read the value stored in one register pair
	 * @param RegAddress STC3115 register
	 * @retval 16-bit value
	 * @note  calls 'error()' in case of I2C error
	 */
	int ReadWord(int RegAddress)
	{
		int value;
		unsigned char data[2];
		int res;

		res = ReadData(data, RegAddress, 2);
		if(res) {
			error("I2C error in %s\n", __func__);
			return 0;
		}
		
		/* no error */
		value = data[1];
		value = (value <<8) + data[0];
		
		return(value);
	}


	/**
	 * @brief Utility function to write a 16-bit value into a register pair
	 * @param RegAddress STC3115 register
	 * @param Value 16-bit value to write
	 * @retval 0 if ok, -1 if an I2C error has occured
	 */
	int WriteWord(int RegAddress, int Value)
	{
		int res;
		unsigned char data[2];

		data[0]= Value & 0xff; 
		data[1]= (Value>>8) & 0xff;
		res = WriteData(data, RegAddress, 2);

		return(res);
	}

	/**
	 * @brief utility function to read the RAM data from STC3115
	 * @param None
	 * @retval 0 if ok, -1 if an I2C error has occured
	 */
	int ReadRamData(void)
	{
		return ReadData((uint8_t*)&RAMData.db, STC3115_REG_RAM, RAM_SIZE);
	}

	/**
	 * @brief Utility function to write the RAM data into STC3115
	 * @param None
	 * @retval 0 if ok, -1 if an I2C error has occured
	 */
	int WriteRamData(void)
	{
		return WriteData((uint8_t*)&RAMData.db, STC3115_REG_RAM, RAM_SIZE);
	}

	/**
	 * @brief Initialize the STC3115 RAM registers with valid test word and CRC
	 * @param None
	 * @retval None
	 */
	void InitRamData(void)
	{
		int index;
		
		//Set full RAM tab to 0
		for (index=0;index<RAM_SIZE;index++) 
			RAMData.db[index]=0;
		
		//Fill RAM regs
		RAMData.reg.TstWord=RAM_TSTWORD;  /* Fixed word to check RAM integrity */
		RAMData.reg.CC_cnf = ConfigData.CC_cnf;
		RAMData.reg.VM_cnf = ConfigData.VM_cnf;
		
		/* update the crc */
		UpdateRamCRC();
	}

	/**
	 * @brief Initialize and start the STC3115 at application startup
	 * @param None
	 * @retval 0 if ok, -1 if an I2C error has occured
	 */
	int Startup(void)
	{
		int res;
		int ocv;
		
		/* check STC310x status */
		res = Status();
		if (res<0) return(res);
		
		/* read OCV */
		ocv = ReadWord(STC3115_REG_OCV);
		
		SetParam();  /* set STC3115 parameters  */
		
		/* rewrite ocv to start SOC with updated OCV curve */
		res = WriteWord(STC3115_REG_OCV, ocv);
		
		return(res);
	}

	/**
	 * @brief Read the STC3115 status
	 * @param None
	 * @retval status word (REG_MODE / REG_CTRL), -1 if error
	 */
	int Status(void)
	{
		int value;
		
		/* first, check the presence of the STC3115 by reading first byte of dev. ID */
		if (ReadByte(STC3115_REG_ID) != STC3115_ID) return (-1);
		
		/* read REG_MODE and REG_CTRL */
		value = ReadWord(STC3115_REG_MODE);
		value &= 0x7fff;   
		
		return (value);
	}

	/**
	 * @brief Initialize the STC3115 parameters
	 * @param None
	 * @retval None
	 * @note  calls 'error()' in case of I2C error
	 */
	void SetParam(void)
	{
		int value;
		int ret;

		/* set GG_RUN=0 before changing algo parameters */
		ret = WriteByte(STC3115_REG_MODE,STC3115_VMODE);
		if(ret) {
			error("I2C error in %s (%d)\n", __func__, __LINE__);
			return;
		}

		/* init OCV curve */
		ret = WriteData((unsigned char *) ConfigData.OCVOffset, 
				STC3115_REG_OCVTAB, OCVTAB_SIZE);
		if(ret) {
			error("I2C error in %s (%d)\n", __func__, __LINE__);
			return;
		}
  
		/* set alm level if different from default */
		if(ConfigData.Alm_SOC !=0) {
			ret = WriteByte(STC3115_REG_ALARM_SOC,ConfigData.Alm_SOC*2); 
			if(ret) {
				error("I2C error in %s (%d)\n", __func__, __LINE__);
				return;
			}
		}
		if(ConfigData.Alm_Vbat !=0) {
			value = ((long)(ConfigData.Alm_Vbat << 9) / VoltageFactor); /* LSB=8*2.2mV */
			ret = WriteByte(STC3115_REG_ALARM_VOLTAGE, value);
			if(ret) {
				error("I2C error in %s (%d)\n", __func__, __LINE__);
				return;
			}
		}
    
		/* relaxation timer */
		if(ConfigData.Rsense !=0 ) {
			value = ((long)(ConfigData.RelaxCurrent << 9) / (CurrentFactor / ConfigData.Rsense));   /* LSB=8*5.88uV */

			ret = WriteByte(STC3115_REG_CURRENT_THRES,value); 
			if(ret) {
				error("I2C error in %s (%d)\n", __func__, __LINE__);
				return;
			}
		}
  
		/* set parameters if different from default, only if a restart is done (battery change) */
		if(RAMData.reg.CC_cnf !=0) {
			ret = WriteWord(STC3115_REG_CC_CNF,RAMData.reg.CC_cnf); 
			if(ret) {
				error("I2C error in %s (%d)\n", __func__, __LINE__);
				return;
			}
		}
		if(RAMData.reg.VM_cnf !=0) {
			ret = WriteWord(STC3115_REG_VM_CNF,RAMData.reg.VM_cnf); 
			if(ret) {
				error("I2C error in %s (%d)\n", __func__, __LINE__);
				return;
			}
		}
		ret = WriteByte(STC3115_REG_CTRL,0x03); /*   clear PORDET, BATFAIL, free ALM pin, reset conv counter */
		if(ret) {
			error("I2C error in %s (%d)\n", __func__, __LINE__);
			return;
		}
		
		ret = WriteByte(STC3115_REG_MODE, STC3115_GG_RUN | 
				(STC3115_VMODE * ConfigData.Vmode) | 
				(STC3115_ALM_ENA * ALM_EN));  /*   set GG_RUN=1, set mode, set alm enable */
		if(ret) {
			error("I2C error in %s (%d)\n", __func__, __LINE__);
			return;
		}

		return;
	}

	/**
	 * @brief calculate the RAM CRC
	 * @param None
	 * @retval CRC value
	 */
	int UpdateRamCRC(void)
	{
		int res;
		
		res = CalcRamCRC8(RAMData.db,RAM_SIZE-1);
		RAMData.db[RAM_SIZE-1] = res;   /* last byte holds the CRC */
		return(res);
	}

	/**
	 * @brief Restore STC3115 state
	 * @param None 
	 * @retval 0 if OK, -1 if error
	 */
	int Restore(void)
	{
		int res;
		
		/* check STC310x status */
		res = Status();
		if (res<0) return(res);
 
		/* set STC3115 parameters  */
		SetParam();  

		/* restore SOC from RAM data */
		res = WriteWord(STC3115_REG_SOC,RAMData.reg.HRSOC);

		return(res);
	}

	/**
	 * @brief Stop the STC3115 at application power down
	 * @param None
	 * @retval Ok if success, error code otherwise
	 */
	int Powerdown(void)
	{
		int res;
		
		/* write 0x01 into the REG_CTRL to release IO0 pin open, */
		res = WriteByte(STC3115_REG_CTRL, 0x01);
		if(res) return (res);
		
		/* write 0 into the REG_MODE register to put the STC3115 in standby mode */
		res = WriteByte(STC3115_REG_MODE, 0);
		if (res != OK) return (res);

		return(OK);
	}

	int ReadBatteryData(void);
	int AlarmSetVoTh(int);
	int AlarmSetSOCTh(int);
};

#endif /* __STC3115_H */
