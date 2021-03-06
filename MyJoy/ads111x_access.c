#include "stm32f1xx_hal.h"
#include "ads111x_access.h"


extern I2C_HandleTypeDef hi2c1;

volatile int ReceiveComplete = 0;
volatile int TransmitComplete = 0;

unsigned char ads111x_adr = ADS1115_REMOTE_ADR;

void ads111x_select(unsigned char adr)
{
	ads111x_adr = adr;
}


unsigned int ads111x_read()
{

	int ret;
	unsigned short Data = 0;

	ReceiveComplete = 0;
	ret = HAL_I2C_Master_Receive(&hi2c1, ads111x_adr, (unsigned char*)&Data, 2, 100);
	if (ret != HAL_OK)
	{
		return -1;
	}

	  //while( ReceiveComplete == 0);
  
	unsigned short Data2 = ((Data & 0xFF) << 8) + (Data >> 8);
	return Data2;

}

#if 0
unsigned short ads111x_read_IT()
{
	int ret;
	unsigned short Data;

	ReceiveComplete = 0;
	ret = HAL_I2C_Master_Receive_IT(&hi2c1, ads111x_adr, (unsigned char*)&Data, 2);
	if (ret != HAL_OK)
	{
		return ret;
	}

	while (ReceiveComplete == 0)
		;
  
	unsigned short Data2 = ((Data & 0xFF) << 8) + (Data >> 8);
	return Data2;

}
#endif
HAL_StatusTypeDef ads111x_write_pointer(unsigned char RegisterAddress)
{
	unsigned char Address = RegisterAddress;
	TransmitComplete = 0;

	HAL_StatusTypeDef ret = HAL_I2C_Master_Transmit(&hi2c1, ads111x_adr, &Address, 1, 100);
	if (ret != HAL_OK)
	{
	  // generation d'un stop alors
		return ret;
	}

	  //while( TransmitComplete == 0);
	return ret;
}
#if 0
int ads111x_write_pointer_IT(unsigned char RegisterAddress)
{
	unsigned char Address = RegisterAddress;
	TransmitComplete = 0;

	int ret = HAL_I2C_Master_Transmit_IT(&hi2c1, ads111x_adr, &Address, 1);
	if (ret != HAL_OK)
	{
	  // generation d'un stop alors
		return ret;
	}

	while (TransmitComplete == 0)
		;
	return ret;
}

// ne sert que pour les configurations, il faut 
void ads111x_write(unsigned short Data)
{
	int ret;
	TransmitComplete = 0;

	unsigned char data[3];
  
	data[0] = 1;
	data[2] = Data & 0xFF;
	data[1] = Data >> 8;
   
	ret = HAL_I2C_Master_Transmit_IT(&hi2c1, ads111x_adr, (unsigned char*)&data, 3);
	if (ret != HAL_OK)
	{
		Printf("Pas R�ponse de l'ads\n");
		return ;
	}

	while (TransmitComplete == 0)
		;
}
#endif

// ne sert que pour les configurations, il faut 
HAL_StatusTypeDef ads111x_write_rr(unsigned short Data, unsigned char reg)
{
	int ret;
	TransmitComplete = 0;

	unsigned char data[3];
  
	data[0] = reg;
	data[1] = Data >> 8;
	data[2] = Data & 0xFF;
   
	ret = HAL_I2C_Master_Transmit(&hi2c1, ads111x_adr, (unsigned char*)&data, 3, 100);
	//if (ret != HAL_OK)
	return ret;

		  //while( TransmitComplete == 0);
}

#if 0
// ne sert que pour les configurations, il faut 
void ads111x_write_rr_IT(unsigned short Data, unsigned char reg)
{
	int ret;
	TransmitComplete = 0;

	unsigned char data[3];
  
	data[0] = reg;
	data[2] = Data & 0xFF;
	data[1] = Data >> 8;
   
	ret = HAL_I2C_Master_Transmit_IT(&hi2c1, ads111x_adr, (unsigned char*)&data, 3);
	if (ret != HAL_OK)
		return ;

	while (TransmitComplete == 0)
		;
}


void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	ReceiveComplete = 1;
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	TransmitComplete = 1;
}
#endif