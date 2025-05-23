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
volatile States_of_Flags Heater_Flag_1 = Undefined, Heater_Flag_2 = Undefined;

volatile uint16_t W_Flag_Count_ON = 0;
volatile uint16_t W_Flag_Count_OFF = 0;

const uint16_t fan1_on_delay = 5;
volatile uint16_t fan1_delay_on_timer_tick = 0;

const uint16_t fan2_on_delay = 5;
volatile uint16_t fan2_delay_on_timer_tick = 0;

const uint16_t heater1_on_timer_delay = 15;
volatile uint16_t heater1_delay_on_timer_tick = 0;

const uint16_t heater2_on_timer_delay = 17;
volatile uint16_t heater2_delay_on_timer_tick = 0;




const uint16_t fan1_off_delay = 15;
volatile uint16_t fan1_delay_off_timer_tick = 0;

const uint16_t fan2_off_delay = 15;
volatile uint16_t fan2_delay_off_timer_tick = 0;

const uint16_t heater1_off_timer_delay = 20;
volatile uint16_t heater1_delay_off_timer_tick = 0;

const uint16_t heater2_off_timer_delay = 15;
volatile uint16_t heater2_delay_off_timer_tick = 0;


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
		if((Speed_Tap2_Flag == Undefined) || (Speed_Tap2_Flag == Off))
		{
			fan1_delay_on_timer_tick += 1;
			if(fan1_delay_on_timer_tick >= fan1_on_delay)
			{
				Speed_Tap_1(true);
				G_Flag = Undefined;
				fan1_delay_on_timer_tick = 0;
			}
		}

	}

	if(G_Flag == Off)
	{
		fan1_delay_off_timer_tick += 1;
		if(fan1_delay_off_timer_tick >= fan1_off_delay)
		{
			Speed_Tap_1(false);
			G_Flag = Undefined;
			fan1_delay_off_timer_tick = 0;
		}
	}



	if(W_Flag == On)
	{

		Speed_Tap_1(false);
		fan1_delay_off_timer_tick = 0;
		fan1_delay_on_timer_tick = 0;
		fan2_delay_on_timer_tick += 1;
		if(fan2_delay_on_timer_tick >= fan2_on_delay)
		{
			Speed_Tap_2(true);
		}

		heater1_delay_on_timer_tick += 1;
		if(heater1_delay_on_timer_tick >= heater1_on_timer_delay)
		{
			H1_L1(true);
			H1_L2(true);
		}

		heater2_delay_on_timer_tick += 1;
		if(heater2_delay_on_timer_tick >= heater2_on_timer_delay)
		{
			H2_L1(true);
			H2_L2(true);
		}

		W_Flag_Count_ON++;
		if (W_Flag_Count_ON >= (fan2_on_delay + heater1_on_timer_delay + heater2_on_timer_delay))
		{
			W_Flag = Undefined;
			W_Flag_Count_ON = 0;
			fan2_delay_on_timer_tick = 0;
			heater1_delay_on_timer_tick = 0;
			heater2_delay_on_timer_tick = 0;
		}
	}

	if(W_Flag == Off)
	{


			H2_L1(false);
			H2_L2(false);

			H1_L1(false);
			H1_L2(false);


		fan2_delay_off_timer_tick += 1;
		if((fan2_delay_off_timer_tick >= fan2_off_delay))
		{
			Speed_Tap_2(false);
		}

		W_Flag_Count_OFF++;

		if (W_Flag_Count_OFF >= fan2_off_delay)
		{
			W_Flag = Undefined;
			W_Flag_Count_OFF = 0;
			fan2_delay_off_timer_tick = 0;
		}
	}

}





void G_Call_ISR(void)
{
	bool pin = (GPIOA->IDR & GPIO_IDR_ID0);

	if(pin == true){
		G_Flag = On;
	}
	else
	{
		G_Flag = Off;
	}
}


void W_Call_ISR(void)
{
	bool pin = (GPIOA->IDR & GPIO_IDR_ID1);

	if(pin == true){
		W_Flag = On;
	}
	else
	{
		W_Flag = Off;
	}
}



int main(void)
{
	MCU_Clock_Setup();


	GPIO_Pin_Init(GPIOA, 0,
			GPIO_Configuration.Mode.Input,
			GPIO_Configuration.Output_Type.None,
			GPIO_Configuration.Speed.None,
			GPIO_Configuration.Pull.Pull_Up,
			GPIO_Configuration.Alternate_Functions.None);

	GPIO_Interrupt_Setup(GPIOA,0, GPIO_Configuration.Interrupt_Edge.RISING_FALLING_EDGE, 0, G_Call_ISR);

	GPIO_Pin_Init(GPIOA, 1,
			GPIO_Configuration.Mode.Input,
			GPIO_Configuration.Output_Type.None,
			GPIO_Configuration.Speed.None,
			GPIO_Configuration.Pull.Pull_Up,
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

	bool pin = (GPIOA->IDR & GPIO_IDR_ID0);

	if(pin == true){
		G_Flag = On;
	}
	else
	{
		G_Flag = Off;
	}

	pin = (GPIOA->IDR & GPIO_IDR_ID1);

	if(pin == true){
		W_Flag = On;
	}
	else
	{
		W_Flag = Off;
	}


	for(;;)
	{

	}
}
