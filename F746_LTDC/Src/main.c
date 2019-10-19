/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "crc.h"
#include "dcmi.h"
#include "dma.h"
#include "dma2d.h"
#include "fatfs.h"
#include "i2c.h"
#include "ltdc.h"
#include "rng.h"
#include "sdmmc.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"
#include "app_x-cube-ai.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "MT48LC4M32B2.h"
#include <stdio.h>
#include "stm32746g_discovery_camera.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
//volatile uint32_t RGB565_480x272[65280] = {0x00000000};
#define LCD_FRAME_BUFFER          SDRAM_DEVICE_ADDR

FATFS SDFatFs; /* File system object for SD card logical drive */
FIL MyFile; /* File object */
extern char SDPath[4]; /* SD logical drive path */
uint8_t sect[4096];
uint32_t bytesread = 0;
uint8_t* bmp1;
uint8_t* dma2d_in1;
uint8_t* dma2d_in2;

ai_float out_data[AI_NETWORK_1_OUT_1_SIZE];
ai_u16 batch_size = 1;
ai_float pfData[AI_NETWORK_1_IN_1_SIZE];

const char Result[] = {'0','1','2','3','4','5','6','7','8','9'};
char* Res = "Recognized: ";

extern DCMI_HandleTypeDef hdcmi;
extern UART_HandleTypeDef huart1;

extern uint16_t X_SIZE;
extern uint16_t Y_SIZE;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

int GetMaxResult(ai_float* out_data, int length)
{
	int ind = 0;
	ai_float max = *out_data;
	for (int i=1;i<length;i++)
	{
		if (*(out_data+i)>max)
		{
			max = *(out_data+i);
			ind = i;
		}
	}
	return ind;
	if (max>0.98) return ind;
	else return -1;
}

ai_float ConvertRGB565_To_Float(uint16_t value)
{
	ai_float R = ((ai_float)(((value&0xF800)>>11)&0x001F))/31.0;
	ai_float G = ((ai_float)(((value&0x07E0)>>5)&0x003F))/63.0;
	ai_float B = ((ai_float)(value&0x001F))/31.0;
	return 1.0-(R+B+G)/3.0;
}

uint16_t ConvertFloat_To_RGB565(ai_float value)
{
	uint16_t R = (uint16_t)(value*31.0);
	uint16_t B = (uint16_t)(value*31.0);
	uint16_t G = (uint16_t)(value*63.0);
	return (R<<11)|(G<<5)|B;
}

/*const uint16_t X_OFF = 114;
const uint16_t Y_OFF = 9;
const uint16_t STRIDE = 9;
const uint16_t WIDTH = 480;*/

/*const uint16_t X_OFF = 170;
const uint16_t Y_OFF = 66;
const uint16_t STRIDE = 5;
const uint16_t WIDTH = 480;*/

const uint16_t X_OFF = 198;
const uint16_t Y_OFF = 94;
const uint16_t STRIDE = 3;
const uint16_t WIDTH = 480;

void TakeCropFrame(void)
{
	uint16_t X0 = X_OFF;
	uint16_t Y0 = Y_OFF;
	uint32_t ind0 = 0;
	ai_float sum = 0.0;
	for (int i=0;i<28;i++)
	{
		for (int j=0;j<28;j++)
		{
			X0 = X_OFF + STRIDE*i;
			Y0 = Y_OFF + STRIDE*j;
			ind0 = X0 + Y0 * WIDTH;
			sum = 0.0;
			for (int m=0;m<STRIDE;m++)
			{
				for (int k=0;k<STRIDE;k++)
				{
					sum += ConvertRGB565_To_Float(*((uint16_t*)dma2d_in1 + ind0 + m));
				}
				ind0 += WIDTH;
			}
			//pfData[(27-i)*28 + j] = (sum/(STRIDE*STRIDE) > 0.5) ? 1.0 : 0.0;
			pfData[j*28 + i] = (sum/(STRIDE*STRIDE) > 0.5) ? 1.0 : 0.0;
		}
	}
}

void PlaceCroppedFrame(void)
{
	for (int i=0;i<28;i++)
	{
		for (int j=0;j<28;j++)
		{
			*(dma2d_in2 + j*28 + i) = ConvertFloat_To_RGB565(pfData[j*28 + i]);
		}
	}
	for (int i=0;i<28;i++)
	{
		for (int j=0;j<28;j++)
		{
			*(dma2d_in1 + i + 480*j) = *(dma2d_in2 + i + 28*j);
		}
	}
}

uint32_t OpenBMP(uint8_t *ptr, const char* fname)
{
	uint32_t ind = 0, sz = 0, i1 = 0, ind1 = 0;
	static uint32_t bmp_addr;
	if(f_open(&MyFile, fname, FA_READ) != FR_OK)
	{
		TFT_FillScreen(0xFF00FF00);
	}
	else
	{
		if (f_read (&MyFile, sect, 30, (UINT *)bytesread) != FR_OK)
		{
			Error_Handler();
		}
		else
		{
			bmp_addr = (uint32_t)sect;
			sz = *(uint16_t *) (bmp_addr + 2);
			sz |= (*(uint16_t *) (bmp_addr + 4)) << 16;
			/* Get bitmap data address offset */
			ind = *(uint16_t *) (bmp_addr + 10);
			ind |= (*(uint16_t *) (bmp_addr + 12)) << 16;
			f_close (&MyFile);
			f_open (&MyFile, fname, FA_READ);
			ind=0;
			do
			{
				if (sz < 4096)
				{
					i1 = sz;
				}
				else
				{
					i1 = 4096;
				}
				sz -= i1;
				f_lseek(&MyFile,ind1);
				f_read (&MyFile, sect, i1, (UINT *)&bytesread);
				memcpy((void*)(bmp1+ind1), (void*)sect, i1);
				ind1+=i1;
			}
			while (sz > 0);
			f_close (&MyFile);
		}
		ind1=0;
	}
	return 0;
}

void BSP_CAMERA_FrameEventCallback(void)
{
	//DMA2D_LayersAlphaReconfig(128,127);
	//HAL_DMA2D_BlendingStart_IT(&hdma2d, (uint32_t) dma2d_in1,(uint32_t) dma2d_in2,
	//							LCD_FRAME_BUFFER, 480, 272);

	TakeCropFrame();
	PlaceCroppedFrame();

	hdma2d.Init.Mode = DMA2D_M2M_PFC;
	hdma2d.Init.ColorMode = DMA2D_ARGB8888;
	hdma2d.Init.OutputOffset = 0;
	hdma2d.LayerCfg[1].AlphaMode = DMA2D_REPLACE_ALPHA;
	hdma2d.LayerCfg[1].InputAlpha = 0xFF;
	hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB565;
	hdma2d.LayerCfg[1].InputOffset = 0;
	if(HAL_DMA2D_Init(&hdma2d) == HAL_OK)
	{
		if(HAL_DMA2D_ConfigLayer(&hdma2d, 1) == HAL_OK)
		{
			//HAL_DMA2D_Start_IT(&hdma2d, (uint32_t)dma2d_in1, (uint32_t)LCD_FRAME_BUFFER, 480, 272);
			if (HAL_DMA2D_Start(&hdma2d, (uint32_t)dma2d_in1, (uint32_t)LCD_FRAME_BUFFER, 480, 272) == HAL_OK)
			{
				HAL_DMA2D_PollForTransfer(&hdma2d, 1);
			}
		}
	}

	MX_X_CUBE_AI_Process((const ai_float *)pfData, out_data, batch_size);

	TFT_SetFont(&Font24);
	TFT_SetTextColor(LCD_COLOR_GREEN);
	TFT_SetBackColor(LCD_COLOR_BLACK);
	int res = GetMaxResult(out_data, 10);
	if (res!=-1)
	{
		TFT_DisplayString(30,240,(uint8_t*)Res,LEFT_MODE);
		TFT_DrawChar(230,240,Result[res]);
	}

	TFT_DrawLine(X_OFF,Y_OFF,X_OFF+28*STRIDE,Y_OFF,LCD_COLOR_ORANGE);
	TFT_DrawLine(X_OFF,Y_OFF+28*STRIDE,X_OFF+28*STRIDE,Y_OFF+28*STRIDE,LCD_COLOR_ORANGE);
	TFT_DrawLine(X_OFF,Y_OFF,X_OFF,Y_OFF+28*STRIDE,LCD_COLOR_ORANGE);
	TFT_DrawLine(X_OFF+28*STRIDE,Y_OFF,X_OFF+28*STRIDE,Y_OFF+28*STRIDE,LCD_COLOR_ORANGE);
	HAL_UART_Transmit(&huart1,(uint8_t*)"Frame/n", 6, 1000);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	uint16_t i,j;
	char image_name[15];

	bmp1 = (uint8_t *)0xC00FF000;
	dma2d_in1 = (uint8_t *)0xC017E800;
	dma2d_in2 = (uint8_t *)0xC01FE000;

  /* USER CODE END 1 */
  
  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_LTDC_Init();
  MX_RNG_Init();
  MX_FMC_Init();
  MX_SDMMC1_SD_Init();
  MX_FATFS_Init();
  MX_DCMI_Init();
  MX_DMA2D_Init();
  MX_I2C1_Init();
  MX_CRC_Init();
  MX_Core_Init();
  /* USER CODE BEGIN 2 */
  MT48LC4M32B2_init(&hsdram1);
  //HAL_LTDC_SetAddress(&hltdc, (uint32_t)&RGB565_480x272[0], 0);
  HAL_LTDC_SetAddress(&hltdc,LCD_FRAME_BUFFER,0);

  MX_X_CUBE_AI_Init();

  TFT_FillScreen(0xFF0000FF);
  HAL_Delay(1000);
  TFT_FillScreen(0xFF000000);

  if(f_mount(&SDFatFs, (TCHAR const*)SDPath, 0) != FR_OK)
	{
		TFT_FillScreen(0xFFFF0000);
		Error_Handler();
	}
  OpenBMP((uint8_t *)bmp1,"test01.bmp");
  TFT_DrawBitmapToMem(0,0,(uint8_t *)bmp1,(uint8_t *)dma2d_in2);

  if (BSP_CAMERA_Init(CAMERA_R480x272) != CAMERA_OK)
  {
	  TFT_FillScreen(0xFFFF00FF);
	  Error_Handler();
  }

  uint32_t Im_size = 240*272; //size=480*272*2/4
  //HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_CONTINUOUS , (uint32_t)LCD_FRAME_BUFFER, Im_size);
  HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_CONTINUOUS , (uint32_t)dma2d_in1, Im_size);

  while(1)
  {
	  HAL_Delay(75000);
	  HAL_DCMI_Stop(&hdcmi);
	  break;
  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

  MX_Core_Process();
    /* USER CODE BEGIN 3 */
	 TFT_FillScreen(LCD_COLOR_BLACK);
	 TFT_SetFont(&Font24);
	 TFT_SetTextColor(LCD_COLOR_YELLOW);
	 TFT_SetBackColor(LCD_COLOR_BLUE);
	 TFT_DrawChar(10, 10, (uint8_t)'S');
	 TFT_DrawChar(27, 10, (uint8_t)'t');
	 TFT_DrawChar(44, 10, (uint8_t)'m');
	 TFT_DrawChar(61, 10, (uint8_t)'3');
	 TFT_DrawChar(78, 10, (uint8_t)'2');
	 TFT_SetTextColor(LCD_COLOR_GREEN);
	 TFT_SetBackColor(LCD_COLOR_RED);
	 TFT_SetFont(&Font20);
	 TFT_DrawChar(10, 34, (uint8_t)'S');
	 TFT_DrawChar(24, 34, (uint8_t)'t');
	 TFT_DrawChar(38, 34, (uint8_t)'m');
	 TFT_DrawChar(52, 34, (uint8_t)'3');
	 TFT_DrawChar(66, 34, (uint8_t)'2');
	 TFT_SetTextColor(LCD_COLOR_BLUE);
	 TFT_SetBackColor(LCD_COLOR_YELLOW);
	 TFT_SetFont(&Font16);
	 TFT_DrawChar(10, 54, (uint8_t)'S');
	 TFT_DrawChar(21, 54, (uint8_t)'t');
	 TFT_DrawChar(32, 54, (uint8_t)'m');
	 TFT_DrawChar(43, 54, (uint8_t)'3');
	 TFT_DrawChar(54, 54, (uint8_t)'2');
	 TFT_SetFont(&Font12);
	 TFT_SetTextColor(LCD_COLOR_CYAN);
	 TFT_SetBackColor(LCD_COLOR_BLACK);
	 TFT_DrawChar(10, 70, (uint8_t)'S');
	 TFT_DrawChar(17, 70, (uint8_t)'t');
	 TFT_DrawChar(24, 70, (uint8_t)'m');
	 TFT_DrawChar(31, 70, (uint8_t)'3');
	 TFT_DrawChar(38, 70, (uint8_t)'2');
	 TFT_SetFont(&Font8);
	 TFT_SetTextColor(LCD_COLOR_RED);
	 TFT_SetBackColor(LCD_COLOR_GREEN);
	 TFT_DrawChar(10, 82, (uint8_t)'S');
	 TFT_DrawChar(15, 82, (uint8_t)'t');
	 TFT_DrawChar(20, 82, (uint8_t)'m');
	 TFT_DrawChar(25, 82, (uint8_t)'3');
	 TFT_DrawChar(30, 82, (uint8_t)'2');
	 TFT_SetFont(&Font24);
	 TFT_SetTextColor(LCD_COLOR_CYAN);
	 TFT_SetBackColor(LCD_COLOR_BLACK);
	 TFT_DisplayString(14, 100, (uint8_t *)"STM32 Left 24", LEFT_MODE);
	 TFT_SetFont(&Font20);
	 TFT_SetTextColor(LCD_COLOR_RED);
	 TFT_DisplayString(14, 130, (uint8_t *)"STM32 Center 20", CENTER_MODE);
	 TFT_SetFont(&Font16);
	 TFT_SetTextColor(LCD_COLOR_MAGENTA);
	 TFT_DisplayString(14, 160, (uint8_t *)"STM32 Right 16", RIGHT_MODE);
	 HAL_Delay(5000);

	  for(j=1;j<=6;j++)
	  {
	      sprintf(image_name,"test%02d.bmp", j%2==0 ? 2 : 1);
	      OpenBMP((uint8_t *)bmp1,image_name);
	      if(j%2!=0)
	    	  TFT_DrawBitmapToMem(0,0,(uint8_t *)bmp1,(uint8_t *)dma2d_in1);
	      else
	    	  TFT_DrawBitmapToMem(0,0,(uint8_t *)bmp1,(uint8_t *)dma2d_in2);

	      for(i=0;i<=255;i++)
			{
				DMA2D_LayersAlphaReconfig(i,255-i);
				if(j%2!=0)
					HAL_DMA2D_BlendingStart_IT(&hdma2d, (uint32_t) dma2d_in1,(uint32_t) dma2d_in2,
							LCD_FRAME_BUFFER, 480, 272);
				else
					HAL_DMA2D_BlendingStart_IT(&hdma2d, (uint32_t) dma2d_in2,(uint32_t) dma2d_in1,
							LCD_FRAME_BUFFER, 480, 272);
				HAL_Delay(10);
			}
			HAL_Delay(1000);
	  }

	for (i=0;i<50;i++)
	{
		TFT_FillScreen(HAL_RNG_GetRandomNumber(&hrng)|0xFF000000);
		HAL_Delay(200);
	}
	TFT_FillScreen(0xFF000000);
	HAL_Delay(1000);

	for(i=0;i<1000;i++)
	{
		TFT_FillRectangle(HAL_RNG_GetRandomNumber(&hrng)%480,
		HAL_RNG_GetRandomNumber(&hrng)%272,
		HAL_RNG_GetRandomNumber(&hrng)%480,
		HAL_RNG_GetRandomNumber(&hrng)%272,
		HAL_RNG_GetRandomNumber(&hrng)|0xFF000000);
		HAL_Delay(10);
	}
	TFT_FillScreen(0xFF000000);
	HAL_Delay(1000);
/*
	  for(i=0;i<10000;i++)
		{
		    for(j=0;j<100;j++)
		     {
		       TFT_DrawPixel(HAL_RNG_GetRandomNumber(&hrng)%480,
		            HAL_RNG_GetRandomNumber(&hrng)%272,0);
		     }
			TFT_DrawPixel(HAL_RNG_GetRandomNumber(&hrng)%480,
			HAL_RNG_GetRandomNumber(&hrng)%272,
			HAL_RNG_GetRandomNumber(&hrng)|0xFF000000);
			HAL_Delay(1);
		}
	  	TFT_FillScreen(0xFF000000);
	  	HAL_Delay(1000);

		for(i=0;i<1000;i++)
		{
			TFT_DrawLine(HAL_RNG_GetRandomNumber(&hrng)%480,
			HAL_RNG_GetRandomNumber(&hrng)%272,
			HAL_RNG_GetRandomNumber(&hrng)%480,
			HAL_RNG_GetRandomNumber(&hrng)%272,
			HAL_RNG_GetRandomNumber(&hrng)|0xFF000000);
			HAL_Delay(10);
		}

		TFT_FillScreen(0xFF000000);
		HAL_Delay(1000);*/
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode 
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC|RCC_PERIPHCLK_USART1
                              |RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_SDMMC1
                              |RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 192;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 3;
  PeriphClkInitStruct.PLLSAI.PLLSAIQ = 2;
  PeriphClkInitStruct.PLLSAI.PLLSAIP = RCC_PLLSAIP_DIV4;
  PeriphClkInitStruct.PLLSAIDivQ = 1;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_4;
  PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLLSAIP;
  PeriphClkInitStruct.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_CLK48;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();
  /** Initializes and configures the Region and the memory to be protected 
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0xC0000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4MB;
  MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
