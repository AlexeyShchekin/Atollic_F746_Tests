/**
  ******************************************************************************
  * File Name          : LTDC.c
  * Description        : This file provides code for the configuration
  *                      of the LTDC instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "ltdc.h"

/* USER CODE BEGIN 0 */
uint16_t X_SIZE = 480;
uint16_t Y_SIZE = 272;
LCD_DrawPropTypeDef lcdprop;
const uint8_t *ch;

extern DMA2D_HandleTypeDef hdma2d;
/* USER CODE END 0 */

LTDC_HandleTypeDef hltdc;

/* LTDC init function */
void MX_LTDC_Init(void)
{
  LTDC_LayerCfgTypeDef pLayerCfg = {0};

  hltdc.Instance = LTDC;
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc.Init.HorizontalSync = 40;
  hltdc.Init.VerticalSync = 9;
  hltdc.Init.AccumulatedHBP = 53;
  hltdc.Init.AccumulatedVBP = 11;
  hltdc.Init.AccumulatedActiveW = 533;
  hltdc.Init.AccumulatedActiveH = 283;
  hltdc.Init.TotalWidth = 565;
  hltdc.Init.TotalHeigh = 285;
  hltdc.Init.Backcolor.Blue = 0;
  hltdc.Init.Backcolor.Green = 0;
  hltdc.Init.Backcolor.Red = 0;
  if (HAL_LTDC_Init(&hltdc) != HAL_OK)
  {
    Error_Handler();
  }
  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = 480;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = 272;
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
  pLayerCfg.Alpha = 255;
  pLayerCfg.Alpha0 = 0;
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  pLayerCfg.FBStartAdress = 0;
  pLayerCfg.ImageWidth = 480;
  pLayerCfg.ImageHeight = 272;
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_LTDC_MspInit(LTDC_HandleTypeDef* ltdcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(ltdcHandle->Instance==LTDC)
  {
  /* USER CODE BEGIN LTDC_MspInit 0 */

  /* USER CODE END LTDC_MspInit 0 */
    /* LTDC clock enable */
    __HAL_RCC_LTDC_CLK_ENABLE();
  
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOJ_CLK_ENABLE();
    __HAL_RCC_GPIOK_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();
    /**LTDC GPIO Configuration    
    PE4     ------> LTDC_B0
    PJ13     ------> LTDC_B1
    PK7     ------> LTDC_DE
    PK6     ------> LTDC_B7
    PK5     ------> LTDC_B6
    PG12     ------> LTDC_B4
    PJ14     ------> LTDC_B2
    PI10     ------> LTDC_HSYNC
    PK4     ------> LTDC_B5
    PJ15     ------> LTDC_B3
    PI9     ------> LTDC_VSYNC
    PK1     ------> LTDC_G6
    PK2     ------> LTDC_G7
    PI15     ------> LTDC_R0
    PJ11     ------> LTDC_G4
    PK0     ------> LTDC_G5
    PI14     ------> LTDC_CLK
    PJ8     ------> LTDC_G1
    PJ10     ------> LTDC_G3
    PJ7     ------> LTDC_G0
    PJ9     ------> LTDC_G2
    PJ6     ------> LTDC_R7
    PJ4     ------> LTDC_R5
    PJ5     ------> LTDC_R6
    PJ3     ------> LTDC_R4
    PJ2     ------> LTDC_R3
    PJ0     ------> LTDC_R1
    PJ1     ------> LTDC_R2 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_11 
                          |GPIO_PIN_8|GPIO_PIN_10|GPIO_PIN_7|GPIO_PIN_9 
                          |GPIO_PIN_6|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_3 
                          |GPIO_PIN_2|GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4 
                          |GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOK, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_LTDC;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_9|GPIO_PIN_15|GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /* USER CODE BEGIN LTDC_MspInit 1 */

  /* USER CODE END LTDC_MspInit 1 */
  }
}

void HAL_LTDC_MspDeInit(LTDC_HandleTypeDef* ltdcHandle)
{

  if(ltdcHandle->Instance==LTDC)
  {
  /* USER CODE BEGIN LTDC_MspDeInit 0 */

  /* USER CODE END LTDC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_LTDC_CLK_DISABLE();
  
    /**LTDC GPIO Configuration    
    PE4     ------> LTDC_B0
    PJ13     ------> LTDC_B1
    PK7     ------> LTDC_DE
    PK6     ------> LTDC_B7
    PK5     ------> LTDC_B6
    PG12     ------> LTDC_B4
    PJ14     ------> LTDC_B2
    PI10     ------> LTDC_HSYNC
    PK4     ------> LTDC_B5
    PJ15     ------> LTDC_B3
    PI9     ------> LTDC_VSYNC
    PK1     ------> LTDC_G6
    PK2     ------> LTDC_G7
    PI15     ------> LTDC_R0
    PJ11     ------> LTDC_G4
    PK0     ------> LTDC_G5
    PI14     ------> LTDC_CLK
    PJ8     ------> LTDC_G1
    PJ10     ------> LTDC_G3
    PJ7     ------> LTDC_G0
    PJ9     ------> LTDC_G2
    PJ6     ------> LTDC_R7
    PJ4     ------> LTDC_R5
    PJ5     ------> LTDC_R6
    PJ3     ------> LTDC_R4
    PJ2     ------> LTDC_R3
    PJ0     ------> LTDC_R1
    PJ1     ------> LTDC_R2 
    */
    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_4);

    HAL_GPIO_DeInit(GPIOJ, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_11 
                          |GPIO_PIN_8|GPIO_PIN_10|GPIO_PIN_7|GPIO_PIN_9 
                          |GPIO_PIN_6|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_3 
                          |GPIO_PIN_2|GPIO_PIN_0|GPIO_PIN_1);

    HAL_GPIO_DeInit(GPIOK, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4 
                          |GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_0);

    HAL_GPIO_DeInit(GPIOG, GPIO_PIN_12);

    HAL_GPIO_DeInit(GPIOI, GPIO_PIN_10|GPIO_PIN_9|GPIO_PIN_15|GPIO_PIN_14);

  /* USER CODE BEGIN LTDC_MspDeInit 1 */

  /* USER CODE END LTDC_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
void TFT_FillScreen(uint32_t color)
{
	hdma2d.Init.Mode = DMA2D_R2M;
	hdma2d.Init.OutputOffset = 0;
	if (HAL_DMA2D_Init(&hdma2d)==HAL_OK)
	{
		if (HAL_DMA2D_Start(&hdma2d, color, hltdc.LayerCfg[0].FBStartAdress,
				hltdc.LayerCfg[0].ImageWidth, hltdc.LayerCfg[0].ImageHeight) == HAL_OK)
		{
			HAL_DMA2D_PollForTransfer(&hdma2d, 10);
		}
	}
	/*uint32_t i;
	uint32_t n = hltdc.LayerCfg[0].ImageHeight * hltdc.LayerCfg[0].ImageWidth;
	for (i=0;i<n;i++)
	{
		// *(__IO uint16_t*)(hltdc.LayerCfg[0].FBStartAdress + (i*2)) = (uint16_t)color;
		*(__IO uint32_t*) (hltdc.LayerCfg[0].FBStartAdress + (i*4)) = color;
	}*/
}

void TFT_FillRectangle(uint16_t x1, uint16_t y1,
        uint16_t x2, uint16_t y2, uint32_t color)
{
	//uint32_t xpos, ypos;
	if(x1>x2) swap(x1,x2);
	if(y1>y2) swap(y1,y2);
	uint32_t addr = 0;
	addr = (hltdc.LayerCfg[0].FBStartAdress) + 4*(y1*hltdc.LayerCfg[0].ImageWidth + x1);
	hdma2d.Init.Mode = DMA2D_R2M;
	hdma2d.Init.OutputOffset = hltdc.LayerCfg[0].ImageWidth-(x2-x1);
	if(HAL_DMA2D_Init(&hdma2d) == HAL_OK)
	{
		if (HAL_DMA2D_Start(&hdma2d, color, addr, x2-x1, y2-y1) == HAL_OK)
		{
			HAL_DMA2D_PollForTransfer(&hdma2d, 10);
		}
	}
	/*for(ypos=y1;ypos<=y2;ypos++)
	{
		for(xpos=x1;xpos<=x2;xpos++)
		{
			// *(__IO uint16_t*) (hltdc.LayerCfg[0].FBStartAdress + (2*(ypos*hltdc.LayerCfg[0].ImageWidth + xpos))) = (uint16_t)color;
			*(__IO uint32_t*) (hltdc.LayerCfg[0].FBStartAdress + (4*(ypos*hltdc.LayerCfg[0].ImageWidth + xpos))) = color;
		}
	}*/
}

void TFT_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint32_t color)
{
   *(__IO uint16_t*) (hltdc.LayerCfg[0].FBStartAdress + (2*(Ypos*hltdc.LayerCfg[0].ImageWidth + Xpos))) = (uint16_t)color;
	//*(__IO uint32_t*) (hltdc.LayerCfg[0].FBStartAdress + (4*(Ypos*hltdc.LayerCfg[0].ImageWidth + Xpos))) = color;
}

void TFT_DrawLine(uint16_t x1, uint16_t y1,
        uint16_t x2, uint16_t y2, uint32_t color)
{
	int steep = abs(y2-y1)>abs(x2-x1);
	if (steep)
	{
		swap(x1,y1);
		swap(x2,y2);
	}
	if(x1>x2)
	{
		swap(x1,x2);
		swap(y1,y2);
	}
	int dx,dy;
	dx=x2-x1;
	dy=abs(y2-y1);
	int err=dx/2;
	int ystep;
	if(y1<y2) ystep = 1;
	else  ystep = -1;
	for (;x1<=x2;x1++)
	{
		if (steep) TFT_DrawPixel(y1,x1,color);
		else TFT_DrawPixel(x1,y1,color);
		err-=dy;
		if (err<0)
		{
			y1 += ystep;
			err+=dx;
		}
	}
}

void TFT_FontsIni(void)
{
 lcdprop.BackColor=LCD_COLOR_BLACK;
 lcdprop.TextColor=LCD_COLOR_GREEN;
 lcdprop.pFont=&Font16;
}

void TFT_SetFont(sFONT *fonts)
{
 lcdprop.pFont=fonts;
}

void TFT_SetTextColor(uint32_t color)
{
 lcdprop.TextColor=color;
}

void TFT_SetBackColor(uint32_t color)
{
 lcdprop.BackColor=color;
}

void TFT_DrawChar(uint16_t x, uint16_t y, const uint8_t c)
{
	uint32_t i = 0, j = 0;
	uint16_t height, width;
	uint8_t offset;
	uint8_t *pchar;
	uint32_t line;

	ch = &lcdprop.pFont->table[(c-' ') * lcdprop.pFont->Height * ((lcdprop.pFont->Width + 7) / 8)];
	height = lcdprop.pFont->Height;
	width = lcdprop.pFont->Width;
	offset = 8 *((width + 7)/8) - width ;

	for(i = 0; i < height; i++)
	{
		pchar = ((uint8_t *)ch + (width + 7)/8 * i);
		switch((width + 7)/8)
		{
		  case 1:
		    line = pchar[0];
		    break;
		  case 2:
		    line = (pchar[0]<< 8) | pchar[1];
		    break;
		  case 3:
		  default:
		    line = (pchar[0]<< 16) | (pchar[1]<< 8) | pchar[2];
		    break;
		}
		for (j = 0; j < width; j++)
		{
		  if(line & (1 << (width- j + offset- 1)))
		  {
		    TFT_DrawPixel((x + j), y, lcdprop.TextColor);
		  }
		  else
		  {
		    TFT_DrawPixel((x + j), y, lcdprop.BackColor);
		  }
		}
		y++;
	}
}

void TFT_DisplayString(uint16_t Xpos, uint16_t Ypos, uint8_t *Text, Text_AlignModeTypdef Mode)
{
	uint16_t ref_column = 1, i = 0;
	uint32_t size = 0, xsize = 0;
	uint8_t *ptr = Text;
	while (*ptr++) size ++ ;
	xsize = (X_SIZE/lcdprop.pFont->Width);

	switch (Mode)
	{
		case CENTER_MODE:
		{
			ref_column = Xpos + ((xsize - size) * lcdprop.pFont->Width) / 2;
			break;
		}
		case LEFT_MODE:
		{
			ref_column = Xpos;
			break;
		}
		case RIGHT_MODE:
		{
			ref_column = - Xpos + ((xsize - size) * lcdprop.pFont->Width);
			break;
		}
		default:
		{
			ref_column = Xpos;
			break;
		}
	}

	if ((ref_column < 1) || (ref_column >= 0x8000))
	{
		ref_column = 1;
	}
	while ((*Text != 0) & (((X_SIZE - (i*lcdprop.pFont->Width)) & 0xFFFF) >= lcdprop.pFont->Width))
	{
	    TFT_DrawChar(ref_column, Ypos, *Text);
	    ref_column += lcdprop.pFont->Width;
	    Text++;
	    i++;
	}
}

void TFT_DrawBitmap(uint32_t Xpos, uint32_t Ypos, uint8_t *pbmp)
{
	uint32_t index = 0, width = 0, height = 0, bit_pixel = 0;
	uint32_t address;

	index = *(__IO uint16_t *) (pbmp + 10);
	index |= (*(__IO uint16_t *) (pbmp + 12)) << 16;
	/* Read bitmap width */
	width = *(uint16_t *) (pbmp + 18);
	width |= (*(uint16_t *) (pbmp + 20)) << 16;
	/* Read bitmap height */
	height = *(uint16_t *) (pbmp + 22);
	height |= (*(uint16_t *) (pbmp + 24)) << 16;
	/* Read bit/pixel */
	bit_pixel = *(uint16_t *) (pbmp + 28);

	address = hltdc.LayerCfg[0].FBStartAdress + (((X_SIZE*Ypos) + Xpos)*4);
	pbmp += (index + (width * (height - 1) * (bit_pixel/8)));
	if ((bit_pixel/8) == 4)
	{
		TFT_FillScreen(0xFFFF0000);
	}
	else if ((bit_pixel/8) == 2)
	{
		TFT_FillScreen(0xFF00FF00);
	}
	else
	{
		for(index=0; index < height; index++)
		{
			hdma2d.Init.Mode = DMA2D_M2M_PFC;
			//hdma2d.Init.ColorMode = DMA2D_ARGB8888;
			hdma2d.Init.ColorMode = DMA2D_RGB565;
			hdma2d.Init.OutputOffset = 0;
			hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
			hdma2d.LayerCfg[1].InputAlpha = 0xFF;
			hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB888;
			hdma2d.LayerCfg[1].InputOffset = 0;
			if(HAL_DMA2D_Init(&hdma2d) == HAL_OK)
			{
				if(HAL_DMA2D_ConfigLayer(&hdma2d, 1) == HAL_OK)
				{
					if (HAL_DMA2D_Start(&hdma2d, (uint32_t) pbmp, address, width, 1) == HAL_OK)
					{
						HAL_DMA2D_PollForTransfer(&hdma2d, 10);
					}
				}
			}
			/*for(ix=0; ix < width; ix++)
			{
				*(uint32_t *) (address+(ix*4)) = convert24to32(*(uint32_t *)(pbmp+(ix*3)));
			}*/
			address+= (X_SIZE*4);
			pbmp -= width*(bit_pixel/8);
		}
	}
	hdma2d.Init.Mode = DMA2D_M2M_BLEND;
	//hdma2d.Init.ColorMode = DMA2D_OUTPUT_ARGB8888;
	hdma2d.Init.ColorMode = DMA2D_OUTPUT_RGB565;
	hdma2d.Init.OutputOffset = 0;
	hdma2d.LayerCfg[1].InputOffset = 0;
	//hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
	hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB565;
	hdma2d.LayerCfg[1].AlphaMode = DMA2D_REPLACE_ALPHA;
	hdma2d.LayerCfg[1].InputAlpha = 0;
	hdma2d.LayerCfg[0].InputOffset = 0;
	//hdma2d.LayerCfg[0].InputColorMode = DMA2D_INPUT_ARGB8888;
	hdma2d.LayerCfg[0].InputColorMode = DMA2D_INPUT_RGB565;
	hdma2d.LayerCfg[0].AlphaMode = DMA2D_REPLACE_ALPHA;
	hdma2d.LayerCfg[0].InputAlpha = 0;
	if(HAL_DMA2D_Init(&hdma2d) == HAL_OK)
	{
		HAL_DMA2D_ConfigLayer(&hdma2d, 0);
		HAL_DMA2D_ConfigLayer(&hdma2d, 1);
	}

	bit_pixel = 0;
}

void TFT_DrawBitmapToMem(uint32_t Xpos, uint32_t Ypos, uint8_t *pbmp, uint8_t *pdst)
{
	uint32_t index = 0, width = 0, height = 0, bit_pixel = 0;

	index = *(__IO uint16_t *) (pbmp + 10);
	index |= (*(__IO uint16_t *) (pbmp + 12)) << 16;
	/* Read bitmap width */
	width = *(uint16_t *) (pbmp + 18);
	width |= (*(uint16_t *) (pbmp + 20)) << 16;
	/* Read bitmap height */
	height = *(uint16_t *) (pbmp + 22);
	height |= (*(uint16_t *) (pbmp + 24)) << 16;
	/* Read bit/pixel */
	bit_pixel = *(uint16_t *) (pbmp + 28);

	pbmp += (index + (width * (height - 1) * (bit_pixel/8)));
	if ((bit_pixel/8) == 4)
	{
		TFT_FillScreen(0xFFFF0000);
	}
	else if ((bit_pixel/8) == 2)
	{
		TFT_FillScreen(0xFF00FF00);
	}
	else
	{
		for(index=0; index < height; index++)
		{
			hdma2d.Init.Mode = DMA2D_M2M_PFC;
			//hdma2d.Init.ColorMode = DMA2D_ARGB8888;
			hdma2d.Init.ColorMode = DMA2D_RGB565;
			hdma2d.Init.OutputOffset = 0;
			hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
			hdma2d.LayerCfg[1].InputAlpha = 0xFF;
			hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB888;
			hdma2d.LayerCfg[1].InputOffset = 0;
			if(HAL_DMA2D_Init(&hdma2d) == HAL_OK)
			{
				if(HAL_DMA2D_ConfigLayer(&hdma2d, 1) == HAL_OK)
				{
					if (HAL_DMA2D_Start(&hdma2d, (uint32_t) pbmp, (uint32_t)pdst, width, 1) == HAL_OK)
					{
						HAL_DMA2D_PollForTransfer(&hdma2d, 10);
					}
				}
			}
			/*for(ix=0; ix < width; ix++)
			{
				*(uint32_t *) (address+(ix*4)) = convert24to32(*(uint32_t *)(pbmp+(ix*3)));
			}*/
			pdst+= (X_SIZE*4);
			pbmp -= width*(bit_pixel/8);
		}
	}
	hdma2d.Init.Mode = DMA2D_M2M_BLEND;
	//hdma2d.Init.ColorMode = DMA2D_OUTPUT_ARGB8888;
	hdma2d.Init.ColorMode = DMA2D_OUTPUT_RGB565;
	hdma2d.Init.OutputOffset = 0;
	hdma2d.LayerCfg[1].InputOffset = 0;
	//hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
	hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB565;
	hdma2d.LayerCfg[1].AlphaMode = DMA2D_REPLACE_ALPHA;
	hdma2d.LayerCfg[1].InputAlpha = 0;
	hdma2d.LayerCfg[0].InputOffset = 0;
	//hdma2d.LayerCfg[0].InputColorMode = DMA2D_INPUT_ARGB8888;
	hdma2d.LayerCfg[0].InputColorMode = DMA2D_INPUT_RGB565;
	hdma2d.LayerCfg[0].AlphaMode = DMA2D_REPLACE_ALPHA;
	hdma2d.LayerCfg[0].InputAlpha = 0;
	if(HAL_DMA2D_Init(&hdma2d) == HAL_OK)
	{
		HAL_DMA2D_ConfigLayer(&hdma2d, 0);
		HAL_DMA2D_ConfigLayer(&hdma2d, 1);
	}

	bit_pixel = 0;
}

void DMA2D_LayersAlphaReconfig(uint32_t alpha1, uint32_t alpha2)
{
	hdma2d.LayerCfg[1].InputAlpha = alpha1;
	hdma2d.LayerCfg[0].InputAlpha = alpha2;
	HAL_DMA2D_ConfigLayer(&hdma2d, 1);
	HAL_DMA2D_ConfigLayer(&hdma2d, 0);
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
