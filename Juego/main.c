/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ili9341.h"
#include "Bitmaps.h"
#include "fatfs_sd.h"
#include "string.h"
#include "stdio.h"
#include "ff.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#define BUFFER_SIZE 512

int _gettimeofday(struct timeval *tv, void *tz)
{
    if (tv) {
        tv->tv_sec = 0;
        tv->tv_usec = 0;
    }
    return 0;
}

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
SPI_HandleTypeDef hspi1;
FATFS fs;
FATFS *pfs;
FIL fil;
FRESULT fres;
DWORD fre_clust;
uint32_t totalSpace, freeSpace;
char buffer[100];
uint8_t buf[1];
uint8_t rxbuff;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart5;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
extern uint8_t fondo[];
extern FATFS fs;   // File system object
extern FIL fil;    // File object

char mRd[100];

uint16_t lastx = 0;
uint16_t lasty = 0;

//uint8_t buffy[10];
uint8_t temp;
uint8_t temp2;

uint8_t indx = 0;

int pos1 = 100;
int pos2 = 100;

int po1 = 10;
int po2 = 265;
uint32_t tamano_archivo;

int i;
int animacion;

unsigned char data[100]; // Buffer para almacenar los datos en unsigned char

volatile uint8_t animar_golpe = 0;
volatile uint8_t animar_golpe2 = 0;

int dx = 2;
int dy = 2;
int tam = 16;
int varx = 0;
int vary = 0;

int cont1 = 0;
int cont2 = 0;

uint8_t parte = 0;
uint8_t selection = 1;
uint8_t pointer = 1;

int mapa = 0;
char texto[10];

int a = 0;
int m = 1;
uint8_t nrandom;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_UART5_Init(void);
/* USER CODE BEGIN PFP */
void menu(void);
void jugador1(int pos);
void jugador2(int pos);
void saque(int r);
void ballselect(uint8_t ball);
void finish(uint8_t j1, uint8_t j2);
void pointerselect(uint8_t btn);
void sdtry(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

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
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_UART5_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */
  LCD_Init();
  srand(time(NULL));

  LCD_Clear(0x00);
  FillRect(0, 0, 319, 239, 0x228B);
  //menu();
  //HAL_Delay(1000);


  /*LCD_Bitmap(0, 0, 320, 240, fondo);
  V_line(160, 25, 206, 0xFFFF);
  V_line(161, 25, 206, 0xFFFF);
  nrandom = rand() % 4;
  saque(nrandom);

  varx = lastx;
  vary = lasty;*/

  HAL_UART_Receive_IT(&huart3, &temp, 1);
  HAL_UART_Receive_IT(&huart5, &temp2, 1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
		switch(parte){
		case 0:
			while (m < 2) {
				HAL_GPIO_TogglePin(Wii_GPIO_Port, Wii_Pin);
				FillRect(0, 0, 319, 239, 0x228B);
				cont1 = 0;
				cont2 = 0;
				menu();
				m++;
			}
			break;
		case 1:

			itoa(cont1, texto, 10);
			LCD_Print(texto, 128, 4, 2, 0xFFFF, 0x228B);
			LCD_Print(":", 153, 4, 2, 0xFFFF, 0x228B);
			itoa(cont2, texto, 10);
			LCD_Print(texto, 178, 4, 2, 0xFFFF, 0x228B);


			if (cont1 == 0 && cont2 == 0){

				while (a < 1) {
					HAL_GPIO_TogglePin(Musica_GPIO_Port, Musica_Pin);
					LCD_Bitmap(0, 0, 320, 240, fondo);
					V_line(160, 25, 206, 0xFFFF);
					V_line(161, 25, 206, 0xFFFF);
					nrandom = rand() % 4;
					saque(nrandom);
					mapa = 0;

					pos1 = 100;
					po1 = 10;
					pos2 = 100;
					po2 = 265;

					varx = lastx;
					vary = lasty;
					a++;
				}

				//Jugadores con animación junta
				jugador1(pos1);
				jugador2(pos2);

				// Restaurar fondo dependiendo de la dirección
				if (dx > 0 && dy > 0) { // abajo-derecha
				    V_line(lastx, lasty, tam, 0x228B);
				    V_line(lastx+1, lasty+1, tam, 0x228B);
				    V_line(lastx-1, lasty-1, tam, 0x228B);
				    H_line(lastx, lasty, tam, 0x228B);
				    H_line(lastx+1, lasty+1, tam, 0x228B);
				    H_line(lastx-1, lasty-1, tam, 0x228B);
				} else if (dx > 0 && dy < 0) { // arriba-derecha
				    V_line(lastx, lasty, tam, 0x228B);
				    V_line(lastx+1, lasty-1, tam, 0x228B);
				    V_line(lastx-1, lasty+1, tam, 0x228B);
				    H_line(lastx, lasty + tam, tam, 0x228B);
				    H_line(lastx+1, lasty+tam-1, tam, 0x228B);
				    H_line(lastx-1, lasty+tam+1, tam, 0x228B);
				} else if (dx < 0 && dy < 0) { // arriba-izquierda
				    V_line(lastx + tam, lasty, tam, 0x228B);
				    V_line(lastx + tam-1, lasty-1, tam, 0x228B);
				    V_line(lastx + tam+1, lasty+1, tam, 0x228B);
				    H_line(lastx, lasty + tam, tam, 0x228B);
				    H_line(lastx-1, lasty + tam-1, tam, 0x228B);
				    H_line(lastx+1, lasty + tam+1, tam, 0x228B);
				} else if (dx < 0 && dy > 0) { // abajo-izquierda
				    V_line(lastx + tam, lasty, tam, 0x228B);
				    V_line(lastx + tam-1, lasty+1, tam, 0x228B);
				    V_line(lastx + tam+1, lasty-1, tam, 0x228B);
				    H_line(lastx, lasty, tam, 0x228B);
				    H_line(lastx-1, lasty+1, tam, 0x228B);
				    H_line(lastx+1, lasty-1, tam, 0x228B);
				}


				// Actualizar posición
				varx += dx;
				vary += dy;


				// Verificar colisión con los bordes y rebotar
				if (vary <= 26 || vary >= (229 - tam)) {
				    dy = -dy;
				}

				//Verificar colisiones con  jugadores
								if ((((po1 + 44) >= varx && (po1 + 38) <= varx) && (vary >= (pos1 - 16) && vary <= (pos1 + 45)))
								&& animar_golpe) {
								    dx = 2;
								}
								if (((po2 >= (varx + 16) && varx <= (po2+22)) && (vary >= (pos2 - 16) && vary <= (pos2 + 45)))
									&& animar_golpe2) {
									dx = -2;
				}

				// Dibujar la pelota en nueva posición
				LCD_Sprite(varx, vary, tam, tam, pelota, 5, selection-1, 0, 0);

				if (varx >= 160 || varx <= (160 - tam)){
					V_line(160, 25, 206, 0xFFFF);
				}

				if (varx >= 161 || varx <= (161 - tam)){
					V_line(161, 25, 206, 0xFFFF);
				}


				// Guardar posición actual
				lastx = varx;
				lasty = vary;

				//Verificar puntos
				if (varx <= 11) {
					cont2++;
					if ((cont1 == 1) || (cont2 == 1)){
						a = 0;
					}
				}else if (varx >= (310 - tam)){
					cont1++;
					if ((cont1 == 1) || (cont2 == 1)){
						a = 0;
					}
				}

				HAL_Delay(4);
			} else if((cont1 == 1 && cont2 == 0) || (cont1 == 0 && cont2 == 1) || (cont1 == 1 && cont2 == 1)){

				//HAL_Delay(4);

				//Jugadores con animación junta
				jugador1(pos1);
				jugador2(pos2);

				while (a < 1) {
					__HAL_UART_DISABLE_IT(&huart5, UART_IT_RXNE);
					__HAL_UART_DISABLE_IT(&huart3, UART_IT_RXNE);

					HAL_GPIO_TogglePin(Musica_GPIO_Port, Musica_Pin);

					FillRect(po1, pos1, 45, 45, 0x228B);
					FillRect(po2, pos2, 45, 45, 0x228B);

					LCD_Bitmap(150, 113, 20, 20, pared);
					LCD_Bitmap(150, 133, 20, 20, pared);
					FillRect(lastx, lasty, tam, tam, 0x228B);

					mapa = 1;

					pos1 = 100;
					po1 = 10;
					pos2 = 100;
					po2 = 265;

					nrandom = rand() % 4;
					saque(nrandom);

					varx = lastx;
					vary = lasty;
					__HAL_UART_ENABLE_IT(&huart5, UART_IT_RXNE);
					__HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);
					a++;
				}

				// Restaurar fondo dependiendo de la dirección
				if (dx > 0 && dy > 0) { // abajo-derecha
				    V_line(lastx, lasty, tam, 0x228B);
				    V_line(lastx+1, lasty+1, tam, 0x228B);
				    V_line(lastx-1, lasty-1, tam, 0x228B);
				    H_line(lastx, lasty, tam, 0x228B);
				    H_line(lastx+1, lasty+1, tam, 0x228B);
				    H_line(lastx-1, lasty-1, tam, 0x228B);
				} else if (dx > 0 && dy < 0) { // arriba-derecha
				    V_line(lastx, lasty, tam, 0x228B);
				    V_line(lastx+1, lasty-1, tam, 0x228B);
				    V_line(lastx-1, lasty+1, tam, 0x228B);
				    H_line(lastx, lasty + tam, tam, 0x228B);
				    H_line(lastx+1, lasty+tam-1, tam, 0x228B);
				    H_line(lastx-1, lasty+tam+1, tam, 0x228B);
				} else if (dx < 0 && dy < 0) { // arriba-izquierda
				    V_line(lastx + tam, lasty, tam, 0x228B);
				    V_line(lastx + tam-1, lasty-1, tam, 0x228B);
				    V_line(lastx + tam+1, lasty+1, tam, 0x228B);
				    H_line(lastx, lasty + tam, tam, 0x228B);
				    H_line(lastx-1, lasty + tam-1, tam, 0x228B);
				    H_line(lastx+1, lasty + tam+1, tam, 0x228B);
				} else if (dx < 0 && dy > 0) { // abajo-izquierda
				    V_line(lastx + tam, lasty, tam, 0x228B);
				    V_line(lastx + tam-1, lasty+1, tam, 0x228B);
				    V_line(lastx + tam+1, lasty-1, tam, 0x228B);
				    H_line(lastx, lasty, tam, 0x228B);
				    H_line(lastx-1, lasty+1, tam, 0x228B);
				    H_line(lastx+1, lasty-1, tam, 0x228B);
				}


				// Actualizar posición
				varx += dx;
				vary += dy;


				// Verificar colisión con los bordes y rebotar
				if (vary <= 28 || vary >= (229 - tam)) {
				    dy = -dy;
				}

				//Verificar coliciones con bloques
				if (((varx == 172 && dx == -2) || (varx+tam == 148 && dx == 2)) && (vary+tam >= 112 && vary <= 154)) {
					dx = -dx;
				}

				if (((vary+tam == 112 && dy == 2) || (vary == 154 && dy == -2)) && (varx+tam >= 150 && varx <= 170)) {
					dy = -dy;
				}

				//Verificar colisiones con  jugadores
								if ((((po1 + 44) >= varx && (po1 + 38) <= varx) && (vary >= (pos1 - 16) && vary <= (pos1 + 45)))
								&& animar_golpe) {
								    dx = 2;
								}
								if (((po2 >= (varx + 16) && varx <= (po2+22)) && (vary >= (pos2 - 16) && vary <= (pos2 + 45)))
									&& animar_golpe2) {
									dx = -2;
								}

				// Dibujar la pelota en nueva posición
				LCD_Sprite(varx, vary, tam, tam, pelota, 5, selection-1, 0, 0);

				if (varx >= 160 || varx <= (160 - tam)){
					V_line(160, 25, 87, 0xFFFF);
					V_line(161, 25, 87, 0xFFFF);
					V_line(160, 153, 78, 0xFFFF);
					V_line(161, 153, 78, 0xFFFF);
				}

				if (varx >= 161 || varx <= (161 - tam)){
					V_line(160, 25, 87, 0xFFFF);
					V_line(161, 25, 87, 0xFFFF);
					V_line(160, 153, 78, 0xFFFF);
					V_line(161, 153, 78, 0xFFFF);
				}

				// Guardar posición actual
				lastx = varx;
				lasty = vary;

				//Verificar puntos
				if (varx <= 11) {
					cont2++;
					if ((cont1 == 1 && cont2 == 1) || (cont1 == 2) || (cont2 == 2)){
						a = 0;
					}
				}else if (varx >= (310 - tam)){
					cont1++;
					if ((cont1 == 1 && cont2 == 1) || (cont1 == 2) || (cont2 == 2)){
						a = 0;
					}
				}

				HAL_Delay(2);
			} else if((cont1 == 2 && cont2 == 0) || (cont1 == 0 && cont2 == 2) || (cont1 == 2 && cont2 == 1) || (cont1 == 2 && cont2 == 2) || (cont1 == 1 && cont2 == 2)){

				//Jugadores con animación junta
				jugador1(pos1);
				jugador2(pos2);

				while (a < 1) {
					__HAL_UART_DISABLE_IT(&huart5, UART_IT_RXNE);
					__HAL_UART_DISABLE_IT(&huart3, UART_IT_RXNE);

					HAL_GPIO_TogglePin(Musica_GPIO_Port, Musica_Pin);

					FillRect(po1, pos1, 45, 45, 0x228B);
					FillRect(po2, pos2, 45, 45, 0x228B);

					LCD_Bitmap(150, 113, 20, 20, pared);
					LCD_Bitmap(150, 133, 20, 20, pared);

					LCD_Bitmap(110, 26, 20, 20, pared);
					LCD_Bitmap(110, 46, 20, 20, pared);

					LCD_Bitmap(190, 228-tam, 20, 20, pared);
					LCD_Bitmap(190, 228-tam-20, 20, 20, pared);

					FillRect(lastx, lasty, tam, tam, 0x228B);

					mapa = 2;

					pos1 = 100;
					po1 = 10;
					pos2 = 100;
					po2 = 265;

					nrandom = rand() % 4;
					saque(nrandom);

					varx = lastx;
					vary = lasty;
					__HAL_UART_ENABLE_IT(&huart5, UART_IT_RXNE);
					__HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);
					a++;
				}

				// Restaurar fondo dependiendo de la dirección
				if (dx > 0 && dy > 0) { // abajo-derecha
				    V_line(lastx, lasty, tam, 0x228B);
				    V_line(lastx+1, lasty+1, tam, 0x228B);
				    V_line(lastx-1, lasty-1, tam, 0x228B);
				    H_line(lastx, lasty, tam, 0x228B);
				    H_line(lastx+1, lasty+1, tam, 0x228B);
				    H_line(lastx-1, lasty-1, tam, 0x228B);
				} else if (dx > 0 && dy < 0) { // arriba-derecha
				    V_line(lastx, lasty, tam, 0x228B);
				    V_line(lastx+1, lasty-1, tam, 0x228B);
				    V_line(lastx-1, lasty+1, tam, 0x228B);
				    H_line(lastx, lasty + tam, tam, 0x228B);
				    H_line(lastx+1, lasty+tam-1, tam, 0x228B);
				    H_line(lastx-1, lasty+tam+1, tam, 0x228B);
				} else if (dx < 0 && dy < 0) { // arriba-izquierda
				    V_line(lastx + tam, lasty, tam, 0x228B);
				    V_line(lastx + tam-1, lasty-1, tam, 0x228B);
				    V_line(lastx + tam+1, lasty+1, tam, 0x228B);
				    H_line(lastx, lasty + tam, tam, 0x228B);
				    H_line(lastx-1, lasty + tam-1, tam, 0x228B);
				    H_line(lastx+1, lasty + tam+1, tam, 0x228B);
				} else if (dx < 0 && dy > 0) { // abajo-izquierda
				    V_line(lastx + tam, lasty, tam, 0x228B);
				    V_line(lastx + tam-1, lasty+1, tam, 0x228B);
				    V_line(lastx + tam+1, lasty-1, tam, 0x228B);
				    H_line(lastx, lasty, tam, 0x228B);
				    H_line(lastx-1, lasty+1, tam, 0x228B);
				    H_line(lastx+1, lasty-1, tam, 0x228B);
				}


				// Actualizar posición
				varx += dx;
				vary += dy;


				// Verificar colisión con los bordes y rebotar
				if (vary <= 28 || vary >= (229 - tam)) {
				    dy = -dy;
				}

				//Verificar coliciones con bloques en x
				if (((varx == 172 && dx == -2) || (varx+tam == 148 && dx == 2)) && (vary+tam >= 112 && vary <= 154)) {
					dx = -dx;
				}

				if (((varx == 212 && dx == -2) || (varx+tam == 188 && dx == 2)) && (vary+tam >= 228-tam-20)) {
					dx = -dx;
				}

				if (((varx == 132 && dx == -2) || (varx+tam == 108 && dx == 2)) && (vary <= 66)) {
					dx = -dx;
				}

				//Verificar coliciones con bloques en y

				if (((vary+tam == 112 && dy == 2) || (vary == 154 && dy == -2)) && (varx+tam >= 150 && varx <= 170)) {
					dy = -dy;
				}

				if ((vary+tam == 228-20-tam && dy == 2) && (varx+tam >= 190 && varx <= 210)) {
					dy = -dy;
				}

				if ((vary == 66 && dy == -2) && (varx+tam >= 110 && varx <= 130)) {
					dy = -dy;
				}

				//Verificar colisiones con  jugadores
								if ((((po1 + 44) >= varx && (po1 + 38) <= varx) && (vary >= (pos1 - 16) && vary <= (pos1 + 45)))
								&& animar_golpe) {
								    dx = 2;
								}
								if (((po2 >= (varx + 16) && varx <= (po2+22)) && (vary >= (pos2 - 16) && vary <= (pos2 + 45)))
									&& animar_golpe2) {
									dx = -2;
								}

				// Dibujar la pelota en nueva posición
				LCD_Sprite(varx, vary, tam, tam, pelota, 5, selection-1, 0, 0);

				if (varx >= 160 || varx <= (160 - tam)){
					V_line(160, 25, 87, 0xFFFF);
					V_line(161, 25, 87, 0xFFFF);
					V_line(160, 153, 78, 0xFFFF);
					V_line(161, 153, 78, 0xFFFF);
				}

				if (varx >= 161 || varx <= (161 - tam)){
					V_line(160, 25, 87, 0xFFFF);
					V_line(161, 25, 87, 0xFFFF);
					V_line(160, 153, 78, 0xFFFF);
					V_line(161, 153, 78, 0xFFFF);
				}

				// Guardar posición actual
				lastx = varx;
				lasty = vary;


				//Verificar puntos
				if (varx <= 11) {
					cont2++;
					if ((cont1 == 2 && cont2 == 2) || (cont1 == 3) || (cont2 == 3)){
						a = 0;
					}
				}else if (varx >= (310 - tam)){
					cont1++;
					if ((cont1 == 2 && cont2 == 2) || (cont1 == 3) || (cont2 == 3)){
						a = 0;
					}
				}

				if ((cont1 == 3) || (cont2 == 3)){
					parte = 2;
					m = 0;
				}
			}
			break;
		case 2:
			while (m < 1) {
				HAL_GPIO_TogglePin(Wii_GPIO_Port, Wii_Pin);
				FillRect(0, 0, 319, 239, 0x228B);
				finish(cont1, cont2);
				m = 1;
			}
			break;
		}

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 80;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief UART5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART5_Init(void)
{

  /* USER CODE BEGIN UART5_Init 0 */

  /* USER CODE END UART5_Init 0 */

  /* USER CODE BEGIN UART5_Init 1 */

  /* USER CODE END UART5_Init 1 */
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 115200;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART5_Init 2 */

  /* USER CODE END UART5_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LCD_RST_Pin|Wii_Pin|Musica_Pin|LCD_D1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LCD_RD_Pin|LCD_WR_Pin|LCD_RS_Pin|LCD_D7_Pin
                          |LCD_D0_Pin|LCD_D2_Pin|Golpe_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LCD_CS_Pin|LCD_D6_Pin|LCD_D3_Pin|LCD_D5_Pin
                          |LCD_D4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SD_SS_GPIO_Port, SD_SS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : LCD_RST_Pin LCD_D1_Pin */
  GPIO_InitStruct.Pin = LCD_RST_Pin|LCD_D1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : Wii_Pin Musica_Pin */
  GPIO_InitStruct.Pin = Wii_Pin|Musica_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_RD_Pin LCD_WR_Pin LCD_RS_Pin LCD_D7_Pin
                           LCD_D0_Pin LCD_D2_Pin */
  GPIO_InitStruct.Pin = LCD_RD_Pin|LCD_WR_Pin|LCD_RS_Pin|LCD_D7_Pin
                          |LCD_D0_Pin|LCD_D2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_CS_Pin LCD_D6_Pin LCD_D3_Pin LCD_D5_Pin
                           LCD_D4_Pin */
  GPIO_InitStruct.Pin = LCD_CS_Pin|LCD_D6_Pin|LCD_D3_Pin|LCD_D5_Pin
                          |LCD_D4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : Golpe_Pin */
  GPIO_InitStruct.Pin = Golpe_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Golpe_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SD_SS_Pin */
  GPIO_InitStruct.Pin = SD_SS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(SD_SS_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void menu(void){
	FillRect(0, 0, 319, 239, 0x228B);
	LCD_Bitmap(30, 20, 90, 90, rojo);
	LCD_Bitmap(200, 20, 90, 90, morado);
	LCD_Bitmap(100, 20, 120, 60, titulo);
	LCD_Bitmap(60, 130, 200, 40, mball);

	LCD_Print("player 1", 130, 95, 1, 0xFFFF, 0x228B);
	sdtry();
	LCD_Print(mRd, 100, 110, 1, 0xFFFF, 0x228B);
	Rect(60, 130, 40, 40, 0xFFFF);

	LCD_Sprite(110, 180, 100, 40, btnst, 3, 0, 0, 0);
}

void sdtry(void){
	  //Montamos
	  fres = f_mount(&fs, "/",0);
	  //Abrimos
	  fres = f_open(&fil, "press.txt", FA_READ);

	  // Leemos archivo
	  while (f_gets(buffer, sizeof(buffer), &fil)){
		  sprintf(mRd, "%s", buffer);
	  }
	  // Cerramos archivo
	  fres = f_close(&fil);

	  //Desmontamos
	  f_mount(NULL, "", 1);
}

void finish(uint8_t j1, uint8_t j2){
	FillRect(0, 0, 319, 239, 0x228B);
	if(cont1 == 3){
		LCD_Sprite(30, 50, 260, 48, player, 2, 0, 0, 0);
	} else if (cont2 == 3){
		LCD_Sprite(30, 50, 260, 48, player, 2, 1, 0, 0);
	}

	itoa(cont1, texto, 10);
	LCD_Print(texto, 100, 118, 2, 0xFFFF, 0x228B);
	LCD_Print(":", 153, 118, 2, 0xFFFF, 0x228B);
	itoa(cont2, texto, 10);
	LCD_Print(texto, 206, 118, 2, 0xFFFF, 0x228B);

	pointer = 1;

	LCD_Sprite(45, 150, 100, 40, goback, 3, 1, 0, 0);
	LCD_Sprite(175, 150, 100, 40, again, 3, 0, 0, 0);
}


void saque(int r){
	switch(r){
		case 0:
			lastx = 250;
			lasty = 30;

			dx = -2;
			dy = 2;
			break;
		case 1:
			lastx = 70;
			lasty = 30;

			dx = 2;
			dy = 2;
			break;
		case 2:
			lastx = 70;
			lasty = 210;

			dx = 2;
			dy = -2;
			break;
		case 3:
			lastx = 250;
			lasty = 210;

			dx = -2;
			dy = -2;
			break;
		default:
			break;
	}
}

void jugador1(int pos){
	 // Variables de animación
	  static uint32_t last_anim_tick = 0;
	  static uint32_t anim_interval = 100; // ms entre frames
	  static uint8_t anim_frame = 0;
	  static uint8_t anim_max = 4; // frames usados

	  static uint8_t golpe_frame = 3;
	  static uint8_t golpe_max = 7;
	  static uint32_t last_golpe_tick = 0;
	  static uint32_t golpe_interval = 50;
	  static uint8_t golpe_en_curso = 0;
	uint32_t now = HAL_GetTick();

	// Animación normal del jugador (idle/movimiento)
	if (!golpe_en_curso && (now - last_anim_tick >= anim_interval)) {
		anim_frame = (anim_frame + 1) % anim_max;
		LCD_Sprite(po1, pos1, 45, 45, ju1, 7, anim_frame, 0, 0);
		last_anim_tick = now;
	}

	// Si se presionó GOLPEAR (se activó la bandera)
	if (animar_golpe && !golpe_en_curso) {
		golpe_en_curso = 1;
		golpe_frame = 3; // Empieza en 3
		last_golpe_tick = now;
		animar_golpe = 0;
	}

	// Animación de golpe
	if (golpe_en_curso && (now - last_golpe_tick >= golpe_interval)) {
		LCD_Sprite(po1, pos1, 45, 45, ju1, 7, golpe_frame, 0, 0);
		golpe_frame++;

		if (golpe_frame >= golpe_max) {
			golpe_en_curso = 0;  // Terminar la animación de golpe
		} else {
			last_golpe_tick = now;
		}
		}
}

void pointerselect(uint8_t btn){
	switch(btn){
	case 1:
		LCD_Sprite(45, 150, 100, 40, goback, 3, 1, 0, 0);
		LCD_Sprite(175, 150, 100, 40, again, 3, 0, 0, 0);
		break;
	case 2:
		LCD_Sprite(45, 150, 100, 40, goback, 3, 0, 0, 0);
		LCD_Sprite(175, 150, 100, 40, again, 3, 1, 0, 0);
		break;
	default:
		break;
	}
}

void ballselect(uint8_t ball){
	switch(ball){
	case 1:
		Rect(100, 130, 40, 40, 0x228B);
		Rect(140, 130, 40, 40, 0x228B);
		Rect(180, 130, 40, 40, 0x228B);
		Rect(220, 130, 40, 40, 0x228B);
		Rect(60, 130, 40, 40, 0xFFFF);
		break;
	case 2:
		Rect(140, 130, 40, 40, 0x228B);
		Rect(180, 130, 40, 40, 0x228B);
		Rect(220, 130, 40, 40, 0x228B);
		Rect(60, 130, 40, 40, 0x228B);
		Rect(100, 130, 40, 40, 0xFFFF);
		break;
	case 3:
		Rect(180, 130, 40, 40, 0x228B);
		Rect(220, 130, 40, 40, 0x228B);
		Rect(60, 130, 40, 40, 0x228B);
		Rect(100, 130, 40, 40, 0x228B);
		Rect(140, 130, 40, 40, 0xFFFF);
		break;
	case 4:
		Rect(220, 130, 40, 40, 0x228B);
		Rect(60, 130, 40, 40, 0x228B);
		Rect(100, 130, 40, 40, 0x228B);
		Rect(140, 130, 40, 40, 0x228B);
		Rect(180, 130, 40, 40, 0xFFFF);
		break;
	case 5:
		Rect(60, 130, 40, 40, 0x228B);
		Rect(100, 130, 40, 40, 0x228B);
		Rect(140, 130, 40, 40, 0x228B);
		Rect(180, 130, 40, 40, 0x228B);
		Rect(220, 130, 40, 40, 0xFFFF);
		break;
	}
}

void jugador2(int pos){
	// Variables de animación
	static uint32_t last_anim_tick2 = 0;
	static uint32_t anim_interval2 = 100; // ms entre frames
	static uint8_t anim_frame2 = 0;
	static uint8_t anim_max2 = 4; // depende de los frames

	static uint8_t golpe_frame2 = 3;
	static uint8_t golpe_max2 = 7;
	static uint32_t last_golpe_tick2 = 0;
	static uint32_t golpe_interval2 = 50;
	static uint8_t golpe_en_curso2 = 0;

	uint32_t now = HAL_GetTick();

	// Animación normal del jugador 2 (idle/movimiento)
	if (!golpe_en_curso2 && (now - last_anim_tick2 >= anim_interval2)) {
	    anim_frame2 = (anim_frame2 + 1) % anim_max2;
	    LCD_Sprite(po2, pos2, 45, 45, ju2, 7, anim_frame2, 1, 0);
	    last_anim_tick2 = now;
	}

	// Si se presionó GOLPEAR (se activó la bandera) para jugador 2
	if (animar_golpe2 && !golpe_en_curso2) {
	    golpe_en_curso2 = 1;
	    golpe_frame2 = 3; // Empieza en 3
	    last_golpe_tick2 = now;
	    animar_golpe2 = 0;
	}

	// Animación de golpe del jugador 2
	if (golpe_en_curso2 && (now - last_golpe_tick2 >= golpe_interval2)) {
	    LCD_Sprite(po2, pos2, 45, 45, ju2, 7, golpe_frame2, 1, 0);
	    golpe_frame2++;

	    if (golpe_frame2 >= golpe_max2) {
	        golpe_en_curso2 = 0;  // Terminar la animación de golpe
	    } else {
	        last_golpe_tick2 = now;
	    }
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){

	if (huart->Instance == USART3) {
	        // Comprobar si el dato recibido es una letra
	        switch (temp) {
	            case 'A':  // Arriba
	                if (((pos1 > 28 && mapa == 0)
	                	| (mapa == 2 && ((po1 >= 150-45  && pos1 > 153 && pos1 < 185) | (po1 <= 108-45 && pos1 < 68 && pos1 > 28) | (po1 <= 150-45 && pos1 > 68)))
	                	| (mapa == 1 && ((po1 >= 150-45  && pos1 > 153 && pos1 < 185) | (po1 >= 150-45 && pos1 < 113-45 && pos1 > 28) | (po1 <= 150-45 && pos1 > 28))))
						&& parte == 1) {
	                    pos1 -= 3;  // Mover hacia arriba
	                    LCD_Sprite(po1, pos1, 45, 45, ju1, 7, animacion, 0, 0);
	                    V_line(89, pos1, 45, 0x228B);
	                }
	                break;
	            case 'B':  // Abajo
	                if (((pos1 < 185 && mapa == 0)
	                	| (mapa == 2 && ((po1 >= 150-45 && pos1 > 153 && pos1 < 185) | (po1 <= 150-45 && pos1 < 185)))
	                	| (mapa == 1 && ((po1 >= 150-45 && pos1 > 153 && pos1 < 185) | (po1 >= 150-45 && pos1 < 113-45 && pos1 > 28) | (po1 <= 150-45 && pos1 < 185))))
	                	&& parte == 1) {
	                    pos1 += 3;  // Mover hacia abajo
	                    LCD_Sprite(po1, pos1, 45, 45, ju1, 7, animacion, 0, 0);
	                    V_line(89, pos1, 45, 0x228B);
	                }
	                break;
	            case 'N':  // Golpear
	            	if(parte == 1){
						HAL_GPIO_TogglePin(Golpe_GPIO_Port, Golpe_Pin);
	            		animar_golpe = 1;
	            	}
	            	if (parte == 0){
	            		LCD_Sprite(110, 180, 100, 40, btnst, 3, 1, 0, 0);
	            		LCD_Sprite(110, 180, 100, 40, btnst, 3, 2, 0, 0);
	            		parte = 1;
	            		//a = 0;
	            	}
	            	if (parte == 2){
	            		if (pointer == 1){
	            			LCD_Sprite(45, 150, 100, 40, goback, 3, 2, 0, 0);
		            		parte = 0;
		            		m = 1;
	            		} else if (pointer == 2){
	            			LCD_Sprite(175, 150, 100, 40, again, 3, 2, 0, 0);
		            		parte = 1;
		            		m = 0;
	            		}
	    				cont1 = 0;
	    				cont2 = 0;
	            		//a = 0;
	            	}
	            	break;
	            case 'D':
	            	if (((po1 < 160-45 && mapa == 0)
	            		| (mapa == 2 && ((po1 < 150-45 && pos1 >= 66 && pos1 <= 153) | (po1 < 108-45 && pos1 <= 68) | (po1 < 160-45 && pos1 >= 153)))
						| (mapa == 1 && ((po1 < 150-45 && pos1 >= 113-45 && pos1 <= 153) | (po1 < 160-45 && pos1 <= 113-45) | (po1 < 160-45 && pos1 >= 153))))
						&& parte == 1) {
	            		po1 += 3;  // Mover hacia derecha
	            		LCD_Sprite(po1, pos1, 45, 45, ju1, 7, animacion, 0, 0);
	            		//V_line(89, pos1, 45, 0x228B);
	            	}
	            	if (parte == 0){
	            		selection++;
	            		if (selection > 5){
	            			selection = 1;
	            		}
	            		ballselect(selection);
	            	}
	            	if (parte == 2){
	            		pointer++;
	            		if (pointer > 2){
	            			pointer = 1;
	            		}
	            		pointerselect(pointer);
	            	}
	            	break;
	            case 'I':
	            	if (po1 > 10 && parte == 1) {
	            	    po1 -= 3;  // Mover hacia derecha
	            	    LCD_Sprite(po1, pos1, 45, 45, ju1, 7, animacion, 0, 0);
	            	    //V_line(89, pos1, 45, 0x228B);
	            	}
	            	if (parte == 0){
	            		selection--;
	            		if (selection < 1){
	            			selection = 5;
	            		}
	            		ballselect(selection);
	            	}
	            	if (parte == 2){
	            		pointer--;
	            		if (pointer < 1){
	            			pointer = 2;
	            		}
	            		pointerselect(pointer);
	            	}
	            	break;
	            // Puedes agregar más casos para otras direcciones o comandos
	            default:
	                break;
	        }
	        char msg[20];
	        sprintf(msg, "Recibido: %c\n", temp);
	        //HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 10);
	        // Preparar para recibir el siguiente byte
	        HAL_UART_Receive_IT(&huart3, &temp, 1);
		}

	if (huart->Instance == UART5) {
				// Comprobar si el dato recibido es una letra
		        switch (temp2) {
		            case 'A':  // Arriba
		                if (((pos2 > 28 && mapa == 0)
			                	| (mapa == 2 && ((po2 <= 150+20 && pos2 > 28 && pos2 < 113-45) | (po2 >= 150+20 && pos2 > 28)))
			                	| (mapa == 1 && ((po2 <= 150+20 && pos2 > 153 && pos2 < 185) | (po2 <= 150-45 && pos2 < 113-45 && pos2 > 28) | (po2 >= 150-45 && pos1 > 28))))
		                	&& parte == 1){
		                    pos2 -= 3;  // Mover hacia arriba
		                    LCD_Sprite(po2, pos2, 45, 45, ju2, 7, animacion, 1, 0);
		                    V_line(89, pos2, 45, 0x228B);
		                }
		                break;
		            case 'B':  // Abajo
		                if (((pos2 < 185 && mapa == 0)
			                	| (mapa == 2 && ((po2 <= 150+20  && pos2 > 28 && pos2 < 113-45) | (po2 >= 212 && pos2 < 185 && pos2 > 192-45) | (po2 >= 150+20 && pos2 <= 192-45)))
			                	| (mapa == 1 && ((po2 <= 150+20  && pos2 > 153 && pos2 < 185) | (po2 <= 150+20 && pos2 < 113-45 && pos2 > 28) | (po2 >= 150+20 && pos2 < 185))))
		                	&& parte == 1){
		                    pos2 += 3;  // Mover hacia abajo
		                    LCD_Sprite(po2, pos2, 45, 45, ju2, 7, animacion, 1, 0);
		                    V_line(89, pos2, 45, 0x228B);
		                }
		                break;
		            case 'D':
		            	if ((po2 < 265) && parte == 1) {
		            	    po2 += 3;  // Mover hacia derecha
		            	    LCD_Sprite(po2, pos2, 45, 45, ju2, 7, animacion, 1, 0);
		            	    //V_line(89, pos1, 45, 0x228B);
		            	}
		            	break;
		            case 'I':
		            	if (((po2 > 162 && mapa == 0)
		            		| (mapa == 2 && ((po2 > 150-45 && pos2 <= 192-45 && pos2 >= 28) | (po2 > 212 && pos2 >= 192-45) | (po2 > 160 && pos2 >= 113-45)))
							| (mapa == 1 && ((po2 > 150+20 && pos2 >= 113-45 && pos2 <= 153) | (po2 > 160 && pos2 <= 113-45) | (po2 > 160 && pos2 >= 153))))
		            		&& parte == 1){
		            	    po2 -= 3;  // Mover hacia derecha
		            	    LCD_Sprite(po2, pos2, 45, 45, ju2, 7, animacion, 1, 0);
		            	    //V_line(89, pos1, 45, 0x228B);
		            	}
		            	break;
		            case 'N':  // Golpear
		            	if(parte == 1){
		            	HAL_GPIO_TogglePin(Golpe_GPIO_Port, Golpe_Pin);
		            	animar_golpe2 = 1;
		            	}
		            	break;
		            // Puedes agregar más casos para otras direcciones o comandos
		            default:
		                break;
		        }
		        char msg[20];
		        sprintf(msg, "Recibido: %c\n", temp2);
		        //HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 10);
		        // Preparar para recibir el siguiente byte
		        HAL_UART_Receive_IT(&huart5, &temp2, 1);

	}
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
