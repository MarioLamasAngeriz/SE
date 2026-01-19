// accel
#include <sys/types.h>
#define BOARD_TIMER_BASEADDR TPM0
#define BOARD_FIRST_TIMER_CHANNEL 5U
#define BOARD_SECOND_TIMER_CHANNEL 2U
/* Get source clock for TPM driver */
#define BOARD_TIMER_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)
#define TIMER_CLOCK_MODE 1U
/* I2C source clock */
#define I2C_BAUDRATE 100000U

#define I2C_RELEASE_SDA_PORT PORTE
#define I2C_RELEASE_SCL_PORT PORTE
#define I2C_RELEASE_SDA_GPIO GPIOE
#define I2C_RELEASE_SDA_PIN 25U
#define I2C_RELEASE_SCL_GPIO GPIOE
#define I2C_RELEASE_SCL_PIN 24U
#define I2C_RELEASE_BUS_COUNT 100U

// magnet
#define MAG3110_I2C_ADDRESS 0x0E
#define MAG3110_WHO_AM_I 0x07  
#define MAG3110_WHO_AM_I_VALUE 0xC4   

#define MAG3110_CTRL_REG1 0x10
#define MAG3110_CTRL_REG2 0x11

#define MAG3110_OUT_X_MSB 0x01   
#define MAG3110_OUT_X_LSB 0x02   
#define MAG3110_OUT_Y_MSB 0x03 
#define MAG3110_OUT_Y_LSB 0x04   
#define MAG3110_OUT_Z_MSB 0x05   
#define MAG3110_OUT_Z_LSB 0x06  

#define MAG3110_DR_STATUS 0x00 

// pwm
#define BOARD_TPM_BASEADDR TPM0
#define BOARD_FIRST_TPM_CHANNEL 2U
#define BOARD_SECOND_TPM_CHANNEL 5U
#define TPM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_PllFllSelClk)

#include "drivers/fsl_debug_console.h"
#include "includes/clock_config.h"
#include "drivers/fsl_common.h"
#include "drivers/fsl_gpio.h"
#include "drivers/fsl_port.h"
#include "includes/pin_mux.h"
#include "includes/MKL46Z4.h"
#include "includes/board.h"
#include <stdint.h>
#include <stdio.h>
#include <math.h>

// accel
#include "drivers/fsl_mma.h"

// pwm
#include "drivers/fsl_tpm.h"

// lcd
#include "includes/lcd.h"

mma_handle_t mmaHandle = {0};

volatile uint8_t dataScale = 0;

volatile int16_t xData = 0;
volatile int16_t yData = 0;
volatile int16_t zData = 0;

volatile int16_t xAngle = 0;
volatile int16_t yAngle = 0;
volatile int16_t zAngle = 0;

const uint8_t g_accel_address[] = {0x1CU, 0x1DU, 0x1EU, 0x1FU};

volatile int16_t xMagData = 0;
volatile int16_t yMagData = 0;
volatile int16_t zMagData = 0;

volatile int16_t headingDegrees = 0;

static void i2c_release_bus_delay(void) {
	uint32_t i = 0;
	for (i = 0; i < I2C_RELEASE_BUS_COUNT; i++) {
		__NOP();
	}
}

void BOARD_I2C_ReleaseBus(void) {
	uint8_t i = 0;
	gpio_pin_config_t pin_config;
	port_pin_config_t i2c_pin_config = {0};

	/* Config pin mux as gpio */ 
	i2c_pin_config.pullSelect = kPORT_PullUp;
	i2c_pin_config.mux = kPORT_MuxAsGpio;

	pin_config.pinDirection = kGPIO_DigitalOutput;
	pin_config.outputLogic = 1U;
    	CLOCK_EnableClock(kCLOCK_PortE);
    	PORT_SetPinConfig(I2C_RELEASE_SCL_PORT, I2C_RELEASE_SCL_PIN, &i2c_pin_config);
    	PORT_SetPinConfig(I2C_RELEASE_SDA_PORT, I2C_RELEASE_SDA_PIN, &i2c_pin_config);

   	GPIO_PinInit(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, &pin_config);
   	GPIO_PinInit(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, &pin_config);

    	/* Drive SDA low first to simulate a start */
    	GPIO_PinWrite(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 0U);
    	i2c_release_bus_delay();

    	/* Send 9 pulses on SCL and keep SDA high */
    	for (i = 0; i < 9; i++) {
		GPIO_PinWrite(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 0U);
        	i2c_release_bus_delay();

        	GPIO_PinWrite(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 1U);
        	i2c_release_bus_delay();

        	GPIO_PinWrite(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 1U);
        	i2c_release_bus_delay();
        	i2c_release_bus_delay();
    	}

    	/* Send stop */
    	GPIO_PinWrite(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 0U);
    	i2c_release_bus_delay();

    	GPIO_PinWrite(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 0U);
    	i2c_release_bus_delay();

    	GPIO_PinWrite(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 1U);
    	i2c_release_bus_delay();

   	GPIO_PinWrite(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 1U);
    	i2c_release_bus_delay();
}

void setup_accel(void) {

	mma_config_t config = {0}; 
	status_t result; 

    	uint8_t array_addr_size = 0;
	uint8_t sensorRange = 0;
   	uint8_t i = 0;
 
	BOARD_Accel_I2C_Init();
    	config.I2C_SendFunc = BOARD_Accel_I2C_Send;
    	config.I2C_ReceiveFunc = BOARD_Accel_I2C_Receive;

    	array_addr_size = sizeof(g_accel_address) / sizeof(g_accel_address[0]);
    
	for (i = 0; i < array_addr_size; i++) {
		config.slaveAddress = g_accel_address[i];
        	result = MMA_Init(&mmaHandle, &config);
		if (result == kStatus_Success)
			break;
	}

	if (result != kStatus_Success) {
		PRINTF("\r\nFallo al inicializar el sensor\r\n");
		return;
	}
    
	if (MMA_ReadReg(&mmaHandle, kMMA8451_XYZ_DATA_CFG, &sensorRange) != kStatus_Success) {
		PRINTF("\r\nFallo obteniendo el rango del sensor\r\n");
		return;
	}
    
	if (sensorRange == 0x00) {
		dataScale = 2U;
	} else if (sensorRange == 0x01) {
		dataScale = 4U;
	} else if (sensorRange == 0x10) {
		dataScale = 8U;
	}
}

void get_accel_data(void) {

	mma_data_t sensorData = {0};

        if (MMA_ReadSensorData(&mmaHandle, &sensorData) != kStatus_Success) {
		PRINTF("\r\nFallo obteniendo datos de sensor\r\n");
		return;
	}

	/* Get the X and Y data from the sensor data structure in 14 bit left format data*/ 
	xData = (int16_t)((uint16_t)((uint16_t)sensorData.accelXMSB << 8) | (uint16_t)sensorData.accelXLSB) / 4U;
	yData = (int16_t)((uint16_t)((uint16_t)sensorData.accelYMSB << 8) | (uint16_t)sensorData.accelYLSB) / 4U;
	zData = (int16_t)((uint16_t)((uint16_t)sensorData.accelZMSB << 8) | (uint16_t)sensorData.accelZLSB) / 4U;

	/* Convert raw data to angle (normalize to 0-90 degrees). No negative angles. */
	xAngle = (int16_t)floor((double)xData * (double)dataScale * 90 / 8192);
	yAngle = (int16_t)floor((double)yData * (double)dataScale * 90 / 8192);
	zAngle = (int16_t)floor((double)zData * (double)dataScale * 90 / 8192);

	if (xAngle < 0) xAngle *= -1;
	if (yAngle < 0) yAngle *= -1;
	if (zAngle < 0) zAngle *= -1;

	/* Print out the angle data. */ 
	PRINTF("ACC -> X = %2d Y = %2d Z = %2d\r\n", xAngle, yAngle, zAngle);
}

void setup_pwm(void) {

	tpm_config_t tpmInfo;
	tpm_chnl_pwm_signal_param_t tpmParam[2];
    
	tpmParam[0].chnlNumber = (tpm_chnl_t)BOARD_FIRST_TPM_CHANNEL;	
	tpmParam[0].level = kTPM_LowTrue ;
	tpmParam[0].dutyCyclePercent = 0U;

	tpmParam[1].chnlNumber = (tpm_chnl_t)BOARD_SECOND_TPM_CHANNEL;
	tpmParam[1].level = kTPM_LowTrue ;
	tpmParam[1].dutyCyclePercent = 0U;

    	CLOCK_SetTpmClock(1U);

    	TPM_GetDefaultConfig(&tpmInfo);
    	TPM_Init(BOARD_TPM_BASEADDR, &tpmInfo);

    	TPM_SetupPwm(BOARD_TPM_BASEADDR, tpmParam, 2U, kTPM_EdgeAlignedPwm, 24000U, TPM_SOURCE_CLOCK);
    	TPM_StartTimer(BOARD_TPM_BASEADDR, kTPM_SystemClock);

}

void change_leds(uint8_t rojo, uint8_t verde) {
	TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_FIRST_TPM_CHANNEL, kTPM_EdgeAlignedPwm, rojo);		// rojo, valores de 0 a 100 unsigned
	TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_SECOND_TPM_CHANNEL, kTPM_EdgeAlignedPwm, verde); 		// verde, valores de 0 a 100 unsigned
}

void update_lcd(void) {

	lcd_set((headingDegrees / 1000) % 10, 1);
	lcd_set((headingDegrees / 100) % 10, 2);
	lcd_set((headingDegrees / 10) % 10, 3);
	lcd_set(headingDegrees % 10, 4);

}

void setup_magnet(void) {
	uint8_t who_am_i = 0;
	uint8_t reg_val = 0;

	BOARD_Accel_I2C_Receive(MAG3110_I2C_ADDRESS, MAG3110_WHO_AM_I, 1, &who_am_i, 1);
    
	if (who_am_i != 0xC4) {
		PRINTF("Error: Magnetómetro no encontrado (ID: 0x%02X)\r\n", who_am_i);
		return;
	}

	reg_val = 0x80; 
	BOARD_Accel_I2C_Send(MAG3110_I2C_ADDRESS, MAG3110_CTRL_REG2, 1, reg_val);

	reg_val = 0x19;
	BOARD_Accel_I2C_Send(MAG3110_I2C_ADDRESS, MAG3110_CTRL_REG1, 1, reg_val);
}

void get_magnet_data(void) {
	uint8_t raw_data[6];
    
	if (BOARD_Accel_I2C_Receive(MAG3110_I2C_ADDRESS, MAG3110_OUT_X_MSB, 1, raw_data, 6) == kStatus_Success) {
        
		xMagData = (int16_t)((raw_data[0] << 8) | raw_data[1]);
		yMagData = (int16_t)((raw_data[2] << 8) | raw_data[3]);
		zMagData = (int16_t)((raw_data[4] << 8) | raw_data[5]);
		PRINTF("MAG -> X = %d Y = %d Z = %d\r\n", xMagData, yMagData, zMagData);

	} else {
		PRINTF("Error al leer datos del Magnetómetro\r\n");
	}
}

void get_tilt_compensated_headign(void) {

	float offsetX = 2507.0f;
	float offsetY = 1674.0f;
	float offsetZ = 2800.0f;

	float accX = (float)xData;
	float accY = (float)yData;
	float accZ = (float)zData;

	float roll = atan2(accY, accZ);
	float pitch = atan2(-accX, sqrt(accY * accY + accZ * accZ));

	float magX = (float)xMagData - offsetX;
	float magY = (float)yMagData - offsetY;
	float magZ = (float)zMagData - offsetZ;

	float xh = magX * cos(pitch) + magZ * sin(pitch);
	float yh = magX * sin(roll) * sin(pitch) + magY * cos(roll) - magZ * sin(roll) * cos(pitch);

	float degrees = atan2(yh, xh) * (180.0 / M_PI);

	if (degrees < 0) 
		degrees += 360.0;

	headingDegrees = (int16_t)degrees;
}

void leds_orientados(int16_t grados) {
	uint8_t intensidadRojo = 0;
	uint8_t intensidadVerde = 0;

	float distanciaAlSur = fabs((float)grados - 180.0f);
	
	intensidadVerde = (uint8_t)((distanciaAlSur / 180.0f) * 100.0f);
	intensidadRojo = 100 - intensidadVerde;
	
	change_leds(intensidadRojo, intensidadVerde);
}

int main(void) {

    	BOARD_InitPins();
	BOARD_BootClockRUN();
	BOARD_I2C_ReleaseBus();
	BOARD_I2C_ConfigurePins();
	BOARD_InitDebugConsole();
	
	lcd_ini();
	setup_pwm();
	setup_accel();
	setup_magnet();

	while (1) {
		get_accel_data();
		get_magnet_data();

		get_tilt_compensated_headign();

		update_lcd();
		leds_orientados(headingDegrees);
		for (volatile int i = 0; i < 500000; i++);
	}
}

