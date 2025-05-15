#include "main.h"
#include "Timer/Timer.h"
#include "GPIO/GPIO.h"
#include "Console/Console.h"

volatile uint32_t timer_tick = 0;


Timer_Config Delay_Timer;


typedef enum Flag_States_Modes{

	Undefined = 0,
	On = 1,
	Off = 2,

}States_of_Flags;

volatile States_of_Flags G_Flag = Undefined;
volatile States_of_Flags W_Flag = Undefined;
volatile States_of_Flags Delay_Flag = Undefined;


volatile States_of_Flags Speed_Tap1_Flag = Undefined;
volatile States_of_Flags Speed_Tap2_Flag = Undefined;
volatile States_of_Flags Heater_Flag_1,Heater_Flag_2  = Undefined;

const uint8_t heater1_on_timer_delay = 15;
volatile uint8_t heater1_delay_on_timer_tick = 0;

const uint8_t heater2_on_timer_delay = 20;
volatile uint8_t heater2_delay_on_timer_tick = 0;

const uint8_t heater1_off_timer_delay = 15;
volatile uint8_t heater1_delay_off_timer_tick = 0;

const uint8_t heater2_off_timer_delay = 20;
volatile uint8_t heater2_delay_off_timer_tick = 0;


const uint8_t fan1_on_delay = 15;
volatile uint8_t fan1_delay_on_timer_tick = 0;

const uint8_t fan1_off_delay = 15;
volatile uint8_t fan1_delay_off_timer_tick = 0;

const uint8_t fan2_on_delay = 10;
volatile uint8_t fan2_delay_on_timer_tick = 0;

const uint8_t fan2_off_delay = 90;
volatile uint8_t fan2_delay_off_timer_tick = 0;


void H1_L1(bool on_off)
{
	if(on_off){
		GPIO_Pin_High(GPIOA, 4);
		Heater_Flag_1 = On;
	}
	else{
		GPIO_Pin_Low(GPIOA, 4);
		Heater_Flag_1 = Off;
	}

}

void H1_L2(bool on_off)
{
	if(on_off){
		GPIO_Pin_High(GPIOA, 5);
		Heater_Flag_1 = On;
	}
	else{
		GPIO_Pin_Low(GPIOA, 5);
		Heater_Flag_1 = Off;
	}

}

void H2_L1(bool on_off)
{
	if(on_off){
		GPIO_Pin_High(GPIOA, 6);
		Heater_Flag_1 = On;
	}
	else{
		GPIO_Pin_Low(GPIOA, 6);
		Heater_Flag_1 = Off;
	}

}

void H2_L2(bool on_off)
{
	if(on_off){
		GPIO_Pin_High(GPIOA, 7);
		Heater_Flag_2 = On;
	}
	else{
		GPIO_Pin_Low(GPIOA, 7);
		Heater_Flag_2 = Off;
	}

}

void Speed_Tap_1(bool on_off)
{
	if(on_off){
		GPIO_Pin_High(GPIOA, 8);
		Speed_Tap1_Flag = On;
	}
	else {
		GPIO_Pin_Low(GPIOA, 8);
		Speed_Tap1_Flag = Off;
	}

}

void Speed_Tap_2(bool on_off)
{
	if(on_off){
		GPIO_Pin_High(GPIOA, 9);
		Speed_Tap2_Flag = On;
	}
	else {
		GPIO_Pin_Low(GPIOA, 9);
		Speed_Tap2_Flag = Off;
	}

}


void Delay_Timer_ISR()
{
	timer_tick += 1;

	if(G_Flag == On)
	{
		fan1_delay_on_timer_tick += 1;
		if(fan1_delay_on_timer_tick >= fan1_on_delay)
		{
			Speed_Tap_1(1);
			G_Flag = Undefined;
			fan1_delay_on_timer_tick = 0;
		}
	}

	if(G_Flag == Off)
	{
		fan1_delay_off_timer_tick += 1;
		if(fan1_delay_off_timer_tick >= fan1_off_delay)
		{
			Speed_Tap_1(0);
			G_Flag = Undefined;
			fan1_delay_off_timer_tick = 0;
		}
	}



	if(W_Flag == On)
	{
		static uint8_t W_Flag_Count = 0;
		Speed_Tap_1(0);
		fan2_delay_on_timer_tick += 1;
		if(fan2_delay_on_timer_tick >= fan2_on_delay)
		{
			Speed_Tap_2(1);
			W_Flag_Count += fan2_delay_on_timer_tick;
			fan2_delay_on_timer_tick = 0;

		}

		heater1_delay_on_timer_tick += 1;
		if((heater1_delay_on_timer_tick >= heater1_on_timer_delay) && (W_Flag_Count >=  fan2_delay_on_timer_tick))
		{
			H1_L1(1);
			H1_L2(1);
			W_Flag_Count += heater1_delay_on_timer_tick;
			heater1_delay_on_timer_tick = 0;
		}

		heater2_delay_on_timer_tick += 1;
		if((heater2_delay_on_timer_tick >= heater2_on_timer_delay) && (W_Flag_Count >=  (fan2_delay_on_timer_tick+heater1_delay_on_timer_tick)))
		{
			H2_L1(1);
			H2_L2(1);
			W_Flag_Count += heater2_delay_on_timer_tick;
			heater2_delay_on_timer_tick = 0;
		}

		if(W_Flag_Count >= (fan2_on_delay + heater1_on_timer_delay + heater2_on_timer_delay))
		{
			W_Flag = Undefined;
			W_Flag_Count = 0;
		}
	}

	if(W_Flag == Off)
	{

	}

}





void G_Call_ISR(void)
{
	if((GPIOA -> IDR & GPIO_IDR_ID0)) G_Flag = 1;
	if(!(GPIOA -> IDR & GPIO_IDR_ID0)) G_Flag = 0;
}


void W_Call_ISR(void)
{
	if((GPIOA -> IDR & GPIO_IDR_ID1)) W_Flag = 1;
	if(!(GPIOA -> IDR & GPIO_IDR_ID1)) W_Flag = 0;
}



int main(void)
{
	MCU_Clock_Setup();


	GPIO_Pin_Init(GPIOA, 0,
			GPIO_Configuration.Mode.Input,
			GPIO_Configuration.Output_Type.None,
			GPIO_Configuration.Speed.None,
			GPIO_Configuration.Pull.None,
			GPIO_Configuration.Alternate_Functions.None);

	GPIO_Interrupt_Setup(GPIOA,0, GPIO_Configuration.Interrupt_Edge.RISING_FALLING_EDGE, 0, G_Call_ISR);

	GPIO_Pin_Init(GPIOA, 1,
			GPIO_Configuration.Mode.Input,
			GPIO_Configuration.Output_Type.None,
			GPIO_Configuration.Speed.None,
			GPIO_Configuration.Pull.None,
			GPIO_Configuration.Alternate_Functions.None);

	GPIO_Interrupt_Setup(GPIOA,1, GPIO_Configuration.Interrupt_Edge.RISING_FALLING_EDGE, 0, W_Call_ISR);


	GPIO_Pin_Init(GPIOA, 4,
			GPIO_Configuration.Mode.General_Purpose_Output,
			GPIO_Configuration.Output_Type.Push_Pull,
			GPIO_Configuration.Speed.Very_High_Speed,
			GPIO_Configuration.Pull.Pull_Down,
			GPIO_Configuration.Alternate_Functions.None);

	GPIO_Pin_Init(GPIOA, 5,
			GPIO_Configuration.Mode.General_Purpose_Output,
			GPIO_Configuration.Output_Type.Push_Pull,
			GPIO_Configuration.Speed.Very_High_Speed,
			GPIO_Configuration.Pull.Pull_Down,
			GPIO_Configuration.Alternate_Functions.None);

	GPIO_Pin_Init(GPIOA, 6,
			GPIO_Configuration.Mode.General_Purpose_Output,
			GPIO_Configuration.Output_Type.Push_Pull,
			GPIO_Configuration.Speed.Very_High_Speed,
			GPIO_Configuration.Pull.Pull_Down,
			GPIO_Configuration.Alternate_Functions.None);

	GPIO_Pin_Init(GPIOA, 7,
			GPIO_Configuration.Mode.General_Purpose_Output,
			GPIO_Configuration.Output_Type.Push_Pull,
			GPIO_Configuration.Speed.Very_High_Speed,
			GPIO_Configuration.Pull.Pull_Down,
			GPIO_Configuration.Alternate_Functions.None);

	GPIO_Pin_Init(GPIOA, 8,
			GPIO_Configuration.Mode.General_Purpose_Output,
			GPIO_Configuration.Output_Type.Push_Pull,
			GPIO_Configuration.Speed.Very_High_Speed,
			GPIO_Configuration.Pull.Pull_Down,
			GPIO_Configuration.Alternate_Functions.None);

	GPIO_Pin_Init(GPIOA, 9,
			GPIO_Configuration.Mode.General_Purpose_Output,
			GPIO_Configuration.Output_Type.Push_Pull,
			GPIO_Configuration.Speed.Very_High_Speed,
			GPIO_Configuration.Pull.Pull_Down,
			GPIO_Configuration.Alternate_Functions.None);

	Delay_Flag = 0;
	Delay_Timer.Port = TIM6;
	Delay_Timer.Mode = Timer_Configurations.Mode.Update;
	Delay_Timer.Clock_Source = Timer_Configurations.Clock_Source.Internal;
	Delay_Timer.DMA_Enable = false;
	Delay_Timer.Direction = Timer_Configurations.Direction.Upcounting;
	Delay_Timer.Interrupt_Request = Timer_Configurations.Interrupt_Request.Update_Interrupt;
	Delay_Timer.ISR_Routines.Update_ISR = Delay_Timer_ISR;
	Delay_Timer.Prescaler = (uint16_t)(8400)-1;
	Delay_Timer.Autoreload_Value = 10000-1;
	Timer_Init(&Delay_Timer);
	Timer_Trigger(&Delay_Timer);



	for(;;)
	{

	}
}
