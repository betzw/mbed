/**
 ******************************************************************************
 * @file    stc3115.cpp
 * @author  AST / EST
 * @version V0.0.1
 * @date    08-October-2014
 * @brief   Implementation file for the STC3115 driver class
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

/* Includes ------------------------------------------------------------------*/
#include "mbed.h"
#include "stc3115.h"
#include "../../x_nucleo_ikc01a1_targets.h"

#if (RSENSE == 0) || (RINT == 0)
#error "RSENSE & RINT must not be zero!"
#endif

/* Methods -------------------------------------------------------------------*/
/** Constructor
 * @param i2c device I2C used to communicate on battery expansion board
 */
STC3115::STC3115(DevI2C &i2c) : GasGauge(), alm(GG_PIN_ALM),
	dev_i2c(i2c), RAMData(), ConfigData(), BatteryData()
{
	int ret;
	char OCVOffset[] = OCV_OFFSET_TAB;

	/* Fill configuration structure parameters */
	ConfigData.Vmode = VMODE;
	ConfigData.Alm_SOC = ALM_SOC;
	ConfigData.Alm_Vbat = ALM_VBAT;
	ConfigData.CC_cnf = (CAPACITY * RSENSE * 250 + 6194) / 12389;
	ConfigData.VM_cnf = (CAPACITY * RINT * 50 + 24444) / 48889;
	ConfigData.Cnom = CAPACITY;
	ConfigData.Rsense = RSENSE;
	ConfigData.RelaxCurrent = CAPACITY / 20;
	for(int i = 0; i < OCVTAB_SIZE; i++) {
		ConfigData.OCVOffset[i] = (char)OCVOffset[i];
	}
    
	/* Battery presence status init*/
	BatteryData.Presence = 1;

	/* check RAM data validity */
	ret = ReadRamData();
	if(ret) error("Initialiaztion failed (%s, %d)\n", __func__, __LINE__);

	if((RAMData.reg.TstWord != RAM_TSTWORD) || (CalcRamCRC8(RAMData.db,RAM_SIZE)!=0)) {
		/* RAM invalid */
		InitRamData();
		ret = Startup();  /* return -1 if I2C error or STC3115 not present */
	} else {
		/* check STC3115 status */
		if((ReadByte(STC3115_REG_CTRL) & (STC3115_BATFAIL | STC3115_PORDET)) != 0 ) {
			ret = Startup();  /* return -1 if I2C error or STC3115 not present */
		} else {
			ret = Restore();  /* recover from last SOC */
		}
	}
	if(ret) error("Initialiaztion failed (%s, %d)\n", __func__, __LINE__);

	//Update RAM status
	RAMData.reg.STC3115_Status = STC3115_INIT;
	UpdateRamCRC();
	ret = WriteRamData();
	if(ret) error("Initialization failed (%s, %d)\n", __func__, __LINE__);
}

/**
 * @brief Periodic Gas Gauge task, to be called e.g. every 5 sec.
 * @param None
 * @retval 1 if data available, 0 if no data, -1 if error
 * @note Affects global STC310x data and gas gauge variables
 */
int STC3115::Task(void)
{
	int res;

	/* ----------------------------- System state verification ---------------------------- */
	/*Read STC3115 status registers */
	res=Status();
	if(res<0) return(res);  /* return if I2C error or STC3115 not responding */
	BatteryData.status = res;

	/* check STC3115 RAM status (battery has not been changed) */
	res = ReadRamData();
	if(res<0) return(res);  /* return if I2C error or STC3115 not responding */

	if((RAMData.reg.TstWord != RAM_TSTWORD) || (CalcRamCRC8(RAMData.db,RAM_SIZE) != 0)) {
		/* if RAM non ok, reset it and set init state */
		InitRamData();
		RAMData.reg.STC3115_Status = STC3115_INIT;
	}

	/* check battery presence status */
	if((BatteryData.status & (STC3115_BATFAIL<<8)) != 0) {
		/* Battery disconnection has been detected */

		/* BATD pin level is over 1.61 or Vcc is below 2.7V */
		BatteryData.Presence = 0;
		
		/* HW and SW state machine reset */
		Reset();

		return (-1);
	} 

	/* check STC3115 running mode */
	if ((BatteryData.status & STC3115_GG_RUN) == 0) {
		if(RAMData.reg.STC3115_Status != STC3115_INIT) {
			/* if RUNNING state, restore STC3115 */
			res = Restore();
			if(res) return(res);
		} else {
			/* if INIT state, initialize STC3115 */
			res = Startup();
			if(res) return(res);
		}
		
		RAMData.reg.STC3115_Status = STC3115_INIT;
	}

	/* --------------------------------- Read battery data ------------------------------- */
	res = ReadBatteryData();
	if(res!=0) return(-1); /* abort in case of I2C failure */

	/* ------------------------------- battery data report ------------------------------- */
	/* check INIT state */
	if(RAMData.reg.STC3115_Status == STC3115_INIT) {
		/* INIT state, wait for current & temperature value available: */
		if (BatteryData.ConvCounter>VCOUNT) {
			RAMData.reg.STC3115_Status = STC3115_RUNNING;

			/*Battery is connected*/
			BatteryData.Presence = 1;
		}
	}

	if (RAMData.reg.STC3115_Status != STC3115_RUNNING) { /* not running : data partially available*/
		BatteryData.ChargeValue = ConfigData.Cnom * BatteryData.SOC / MAX_SOC;
		BatteryData.Current=0;
		BatteryData.Temperature=250;
		BatteryData.RemTime = -1;
	} else { /*STC3115 running */
		/* ---------- process SW algorithms -------- */
			
		/* early empty compensation */
		if (BatteryData.Voltage < APP_CUTOFF_VOLTAGE)
			BatteryData.SOC = 0;
		else if (BatteryData.Voltage < (APP_CUTOFF_VOLTAGE+200))
			BatteryData.SOC = BatteryData.SOC * (BatteryData.Voltage - APP_CUTOFF_VOLTAGE) / 200;

		/* Battery charge value calculation */
		BatteryData.ChargeValue = ConfigData.Cnom * BatteryData.SOC / MAX_SOC;
		if ((BatteryData.status & STC3115_VMODE) == 0) { /* mixed mode only */
			/* Lately fully compensation */
			if ((BatteryData.status & STC3115_VMODE) == 0) { /*running in mixed mode*/
				if(BatteryData.Current > APP_EOC_CURRENT && BatteryData.SOC > 990) {
					res = WriteWord(STC3115_REG_SOC,50688);   /* 99% */
					if(res) return(res); 
					BatteryData.SOC = 990;
				}
			}
				
			/* Remaining time calculation */
			if(BatteryData.Current < 0) {
				BatteryData.RemTime = (BatteryData.RemTime * 4 + BatteryData.ChargeValue / BatteryData.Current * 60 ) / 5;
				if( BatteryData.RemTime  < 0)
					BatteryData.RemTime = -1; /* means no estimated time available */
			} else
				BatteryData.RemTime = -1; /* means no estimated time available */
		} else { /* voltage mode only */
			BatteryData.Current=0;
			BatteryData.RemTime = -1;
		}
		
		// SOC min/max clamping
		if(BatteryData.SOC>1000) BatteryData.SOC = MAX_SOC;
		if(BatteryData.SOC<0) BatteryData.SOC = 0;
	}
	
	/* save SOC */
	RAMData.reg.HRSOC = BatteryData.HRSOC;
	RAMData.reg.SOC = (BatteryData.SOC+5)/10;
	UpdateRamCRC();
	res = WriteRamData();
	if(res) return(res);  /* return if I2C error or STC3115 not responding */

	if (RAMData.reg.STC3115_Status==STC3115_RUNNING)
		return(1);
	else
		return(0);  /* only SOC, OCV and voltage are valid */
}

/**
 * @brief Utility function to read the battery data from STC3115 to be called every 5s or so
 * @param None
 * @retval error status (OK, !OK)
 */
int STC3115::ReadBatteryData(void)
{
	unsigned char data[16];
	int res;
	int value;

	/* read STC3115 registers 0 to 14 */
	res = ReadData(data, 0, 15);

	if(res<0) return(!OK);  /* read failed */

	/* fill the battery status data */
	/* SOC */
	value = data[3];
	value = (value<<8) + data[2];
	BatteryData.HRSOC = value;     /* result in 1/512% */
	BatteryData.SOC = (value * 10 + 256) / 512; /* result in 0.1% */

	/* conversion counter */
	value=data[5];
	value = (value<<8) + data[4];
	BatteryData.ConvCounter = value;

	/* current */
	value=data[7];
	value = (value<<8) + data[6];
	value &= 0x3fff;   /* mask unused bits */
	if (value>=0x2000) value = value - 0x4000;  /* convert to signed value */
	BatteryData.Current = conv(value, CurrentFactor/RSENSE);  /* result in mA */

	/* voltage */
	value=data[9];
	value = (value<<8) + data[8];
	value &= 0x0fff; /* mask unused bits */
	if (value>=0x0800) value -= 0x1000;  /* convert to signed value */
	value = conv(value,VoltageFactor);  /* result in mV */
	BatteryData.Voltage = value;  /* result in mV */

	/* temperature */
	value=data[10];
	if (value>=0x80) value -= 0x100;  /* convert to signed value */
	BatteryData.Temperature = value*10;  /* result in 0.1°C */

	/* OCV */
	value=data[14];
	value = (value<<8) + data[13];
	value &= 0x3fff; /* mask unused bits */
	if (value>=0x02000) value -= 0x4000;  /* convert to signed value */
	value = conv(value,VoltageFactor);
	value = (value+2) / 4;  /* divide by 4 with rounding */
	BatteryData.OCV = value;  /* result in mV */

	//  res=STC3115_Read(1, STC3115_REG_RELAX_COUNT, data);
	//  if (res<0) return(res);  /* read failed */
	//  BatteryData.RelaxTimer = data[0];

	return(OK);
}

/**
 * @brief Reset the Gas Gauge system
 * @param None
 * @retval None
 * @note  calls 'error()' in case of I2C error
 */
void STC3115::Reset(void)
{
	int ret;

	/* reset RAM */
	RAMData.reg.TstWord=0;
	RAMData.reg.STC3115_Status = 0;
	ret = WriteRamData();
	if(ret) error("Reset failed (%s, %d)\n", __func__, __LINE__);

	/* reset STC3115 */
	ret = WriteByte(STC3115_REG_CTRL, STC3115_PORDET);  /* set soft POR */
	if(ret) error("Reset failed (%s, %d)\n", __func__, __LINE__);
}

/**
 * @brief Stop the Gas Gauge system
 * @param None
 * @retval 0 is ok, -1 if I2C error
 */
int STC3115::Stop(void)
{
	int res;

	/* Save context in RAM */
	res = ReadRamData();
	if(res) return (-1);
	RAMData.reg.STC3115_Status= STC3115_POWERDN;

	/* update the crc */
	UpdateRamCRC();
	res = WriteRamData();
	if(res) return (-1);

	/* STC3115 Power down */
	res = Powerdown();
	if (res!=0) return (-1);  /* error */

	return(0);
}

/**
 * @brief Set the voltage alarm threshold
 * @param VoltThresh Voltage alarm threshold
 * @retval error status (OK, !OK)
 */
int STC3115::AlarmSetVoTh(int VoltThresh)
{
	int res;
	int value;

	ConfigData.Alm_Vbat = VoltThresh;

	value= ((long)(ConfigData.Alm_Vbat << 9) / VoltageFactor); /* LSB=8*2.2mV */
	res = WriteByte(STC3115_REG_ALARM_VOLTAGE, value);
	if (res != OK) return (!OK);

	return (OK);
}

/**
 * @brief Sets Alarm State of Charge threshold
 * @param SoC alarm threshold
 * @retval error status (OK, !OK)
 */
int STC3115::AlarmSetSOCTh(int SOCThresh)
{
	int res;

	ConfigData.Alm_SOC = SOCThresh;
	res = WriteByte(STC3115_REG_ALARM_SOC, ConfigData.Alm_SOC*2);
	if (res != OK) return (!OK);

	return (OK);
}
