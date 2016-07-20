/*########################################################## Main program file ############################################################*/
#include <stdio.h>
#include <math.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <hd44780.h>
GPIO_InitTypeDef        GPIO_InitStructure;											     //structure used for setting up a GPIO port
RCC_ClocksTypeDef 			RCC_Clocks;													//structure used for setting up the SysTick Interrupt
uint8_t err=1;																		   //used for diagnostics, can be removed
void hd44780_Delay(unsigned long del);
#if HD44780_CONF_BUS == HD44780_FUNC_BUS_4BIT
void hd44780_wr_hi_nibble( unsigned char data )
{
	if ( data & 0x10 ) {
		GPIO_SetBits( GPIOA, HD44780_DATABIT4 );
	} else {
		GPIO_ResetBits( GPIOA, HD44780_DATABIT4  );
	}
	if ( data & 0x20 ) {
		GPIO_SetBits( GPIOA, HD44780_DATABIT5 );
	} else {
		GPIO_ResetBits( GPIOA, HD44780_DATABIT5 );
	}
	if ( data & 0x40 ) {
		GPIO_SetBits( GPIOB, HD44780_DATABIT6 );
	} else {
		GPIO_ResetBits( GPIOB, HD44780_DATABIT6 );
	}
	if ( data & 0x80 ) {
		GPIO_SetBits( GPIOB, HD44780_DATABIT7 );
	} else {
		GPIO_ResetBits( GPIOB, HD44780_DATABIT7 );
	}

    /* set the EN signal */
    hd44780_EN_On();

    /* wait */
   hd44780_EN_high_delay();

    /* reset the EN signal */
   hd44780_EN_Off();
}


void hd44780_wr_lo_nibble( unsigned char data )
{
	if ( data & 0x01 ) {
		GPIO_SetBits(GPIOA, HD44780_DATABIT4 );
	} else {
		GPIO_ResetBits(GPIOA, HD44780_DATABIT4 );
	}
	if ( data & 0x02 ) {
		GPIO_SetBits( GPIOA, HD44780_DATABIT5 );
	} else {
		GPIO_ResetBits( GPIOA, HD44780_DATABIT5  );
	}
	if ( data & 0x04 ) {
		GPIO_SetBits( GPIOB, HD44780_DATABIT6 );
	} else {
		GPIO_ResetBits(GPIOB, HD44780_DATABIT6 );
	}
	if ( data & 0x08 ) {
		GPIO_SetBits( GPIOB, HD44780_DATABIT7 );
	} else {
		GPIO_ResetBits(GPIOB, HD44780_DATABIT7  );
	}

    /* set the EN signal */
    hd44780_EN_On();

    /* wait */
    hd44780_EN_high_delay();

    /* reset the EN signal */
    hd44780_EN_Off();
}

/* 4bit bus version */
void hd44780_write( unsigned char data )
{
	/* send the data bits - high nibble first */
	hd44780_wr_hi_nibble( data );
	hd44780_wr_lo_nibble( data );
}
#endif /* HD44780_CONF_BUS == HD44780_FUNC_BUS_4BIT */


#if HD44780_CONF_BUS == HD44780_FUNC_BUS_8BIT

/* 8bit bus version */
void hd44780_write( unsigned char data )
{
	/* set the data bits */
	if ( data & 0x01 ) {
		GPIO_SetBits( HD44780_DATAPORT, HD44780_DATABIT0 );
	} else {
		GPIO_ResetBits( HD44780_DATAPORT, HD44780_DATABIT0 );
	}
	if ( data & 0x02 ) {
		GPIO_SetBits( HD44780_DATAPORT, HD44780_DATABIT1 );
	} else {
		GPIO_ResetBits( HD44780_DATAPORT, HD44780_DATABIT1 );
	}
	if ( data & 0x04 ) {
		GPIO_SetBits( HD44780_DATAPORT, HD44780_DATABIT2 );
	} else {
		GPIO_ResetBits( HD44780_DATAPORT, HD44780_DATABIT2 );
	}
	if ( data & 0x08 ) {
		GPIO_SetBits( HD44780_DATAPORT, HD44780_DATABIT3 );
	} else {
		GPIO_ResetBits( HD44780_DATAPORT, HD44780_DATABIT3 );
	}
	if ( data & 0x10 ) {
		GPIO_SetBits( HD44780_DATAPORT, HD44780_DATABIT4 );
	} else {
		GPIO_ResetBits( HD44780_DATAPORT, HD44780_DATABIT4 );
	}
	if ( data & 0x20 ) {
		GPIO_SetBits( HD44780_DATAPORT, HD44780_DATABIT5 );
	} else {
		GPIO_ResetBits( HD44780_DATAPORT, HD44780_DATABIT5 );
	}
	if ( data & 0x40 ) {
		GPIO_SetBits( HD44780_DATAPORT, HD44780_DATABIT6 );
	} else {
		GPIO_ResetBits( HD44780_DATAPORT, HD44780_DATABIT6 );
	}
	if ( data & 0x80 ) {
		GPIO_SetBits( HD44780_DATAPORT, HD44780_DATABIT7 );
	} else {
		GPIO_ResetBits( HD44780_DATAPORT, HD44780_DATABIT7 );
	}

    /* tell the lcd that we have a command to read in */
    hd44780_EN_On();

    /* wait long enough so that the lcd can see the command */
    hd44780_EN_high_delay();

    /* reset the ce line */
    hd44780_EN_Off();
    hd44780_init_end_delay();

}
#endif /* HD44780_CONF_BUS == HD44780_FUNC_BUS_8BIT */


void hd44780_wr_cmd( unsigned char cmd )
{
	hd44780_RS_Off();
	hd44780_write( cmd );
}


void hd44780_wr_data( unsigned char data )
{
	hd44780_RS_On();
	hd44780_write( data );
}



void hd44780_Delay(unsigned long del)
{
	unsigned long i=0;

	while(i<del)
	{
		i++;

	}
}



void HD44780_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Configure the peripheral clocks for the HD44780 data and control lines */
	RCC_APB2PeriphClockCmd(HD44780_RCC_AHBPeriph, ENABLE);

	/* Configure the HD44780 Data lines (DB7 - DB4) as outputs*/
	GPIO_InitStructure.GPIO_Pin = HD44780_DATABIT5 | HD44780_DATABIT4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = HD44780_DATABIT7 | HD44780_DATABIT6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);


	/* Configure the HD44780 Control lines (RS, RW, EN) as outputs*/
	GPIO_InitStructure.GPIO_Pin = HD44780_RS_BIT | HD44780_RW_BIT;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = HD44780_EN_BIT;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* clear control bits */
	hd44780_EN_Off();
	hd44780_RS_Off();
	hd44780_RW_Off();

	/* wait initial delay for LCD to settle */
  /* reset procedure - 3 function calls resets the device */
  hd44780_init_delay();
  hd44780_wr_hi_nibble( HD44780_CMD_RESET );
  hd44780_init_delay2();
  hd44780_wr_hi_nibble( HD44780_CMD_RESET );
  hd44780_init_delay3();
  hd44780_wr_hi_nibble( HD44780_CMD_RESET );

	#if HD44780_CONF_BUS == HD44780_FUNC_BUS_4BIT
    /* 4bit interface */
  hd44780_wr_hi_nibble( HD44780_CMD_FUNCTION );
  #endif /* HD44780_CONF_BUS == HD44780_FUNC_BUS_4BIT */

  /* sets the configured values - can be set again only after reset */
  hd44780_function( HD44780_CONF_BUS, HD44780_CONF_LINES, HD44780_CONF_FONT );

  /* turn the display on with no cursor or blinking */
	hd44780_display( HD44780_DISP_ON, HD44780_DISP_CURS_OFF, HD44780_DISP_BLINK_OFF );

  /* clear the display */
  hd44780_clear();

  /* addr increment, shift cursor */
	hd44780_entry( HD44780_ENTRY_ADDR_INC, HD44780_ENTRY_SHIFT_CURS );

}



void HD44780_PutChar(unsigned char c)
{
	hd44780_wr_data(c & 0xff);
}



void HD44780_GotoXY(unsigned char x, unsigned char y)
{
  unsigned char copy_y=0;

  if(x > (HD44780_DISP_LENGTH-1))
		x = 0;

	if(y > (HD44780_DISP_ROWS-1))
		y = 0;

	switch(y)
  {
		case 0:  copy_y = 0x80; break;
		case 1:  copy_y = 0xc0; break;
		case 2:  copy_y = 0x94; break;
		case 3:  copy_y = 0xd4; break;
  }
	hd44780_wr_cmd(x + copy_y);
}


void HD44780_PutStr(char *str)
{
	__IO unsigned int i=0;

	do
	{
		HD44780_PutChar(str[i]);
		i++;
	}while(str[i]!='\0');
}




void HD44780_ClrScr(void)
{
	hd44780_wr_cmd(HD44780_CMD_CLEAR);
}

//--------------------------------------------END OF LIBRARY----------------------------------------*/

// Unused global variables that have to be included to ensure correct compiling */
// ###### DO NOT CHANGE ######
// ===============================================================================
__IO uint32_t TimingDelay = 0;																	//used with the Delay function
__IO uint8_t DataReady = 0;
__IO uint32_t USBConnectTimeOut = 100;
__IO uint32_t UserButtonPressed = 0;
__IO uint8_t PrevXferComplete = 1;
// ===============================================================================



int main(void)
{
	/* Set the SysTick Interrupt to occur every 1ms) */
	RCC_GetClocksFreq(&RCC_Clocks);
	if (SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000))
		while(1);		//will end up in this infinite loop if there was an error with Systick_Config





















	/*
 * -----------NOTE: YOU NEED TO STEP INTO (USE DEBUG NOT RUN) THE HD44780_Init AND HD44780_PutStr FUNCTIONS FOR THEM TO WORK PROPERLY.------0
 *------------ OTHERWISE DISPLAY WILL NOT BE PROPERLY INITIALISED---------------------------------------------------------------------------0
*/






















	HD44780_Init();
    HD44780_PutStr("Sample 1st row");
    HD44780_GotoXY(0,1);
    HD44780_PutStr("Sample 2nd row");
    HD44780_ClrScr();


while(1)
{ 						/*infinite loop, just terminate debugging session to kill the program*/  }



}







// Function to insert a timing delay of nTime
// ###### DO NOT CHANGE ######
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

// Function to Decrement the TimingDelay variable.
// ###### DO NOT CHANGE ######
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}

// Unused functions that have to be included to ensure correct compiling
// ###### DO NOT CHANGE ######
// =======================================================================
uint32_t L3GD20_TIMEOUT_UserCallback(void)
{
  return 0;
}

uint32_t LSM303DLHC_TIMEOUT_UserCallback(void)
{
  return 0;
}
// =======================================================================

/*############################################################################################# EOF #######################################*/

