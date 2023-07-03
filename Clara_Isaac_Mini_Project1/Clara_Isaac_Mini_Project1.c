/*
 ***************************
 * Name        :  Mini_Project.c
 * Author      :  Clara Issa Ishac
 * Description :  Vehicle control system project
 * Date        :  27/8/2022
 ***************************
*/

#include <stdio.h>
#define WITH_ENGINE_TEMP_CONTROLLER 1

/* creating enum for the things which have ON and OFF state
   like AC and Engine temp controller */
typedef enum{
	OFF, ON
} ON_OFF;

// creating a structure for all the things related to the vehicle.
typedef struct
{
	ON_OFF state; // vehicle state (ON or OFF)
	short room_temp;
	unsigned short speed;
	ON_OFF ac_state;
#if WITH_ENGINE_TEMP_CONTROLLER
	short engine_temp;
	ON_OFF engine_temp_state; // engine temp controller state
#endif
}Vehicle;

 // Functions Prototypes
void vehicle_state_func(Vehicle *vec);
void ON_state(Vehicle *vec);
void set_traffic_color(Vehicle *vec);
void set_room_temp(Vehicle *vec);
#if WITH_ENGINE_TEMP_CONTROLLER
void set_engine_temp(Vehicle *vec);
#endif
void check_speed(ON_OFF *state, short *temp, const unsigned short *speed);
void print_data(const Vehicle *vec);

int main(void)
{
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
#if WITH_ENGINE_TEMP_CONTROLLER
	Vehicle vec1 = {OFF, 35, 0, OFF, 90, OFF};
#else
	Vehicle vec1 = {OFF, 35, 0, OFF};
#endif
	/* pass by reference because we will change
	   in the structure members themselves.
	   All functions are pass by reference too for the same reason.*/
	vehicle_state_func(&vec1);
	return 0;
}

// function to choose the vehicle's state or quit the system
void vehicle_state_func(Vehicle *vec)
{
	/* create a variable which will contain the user's choice.
	 * unsigned char because we don't need more than the values
	 * of ASCII alphabets as we don't need the negative values. */
	unsigned char choice;
	/* the code after while will execute if the vehicle is on OFF
	 * state or the user enters another letter than a or b or c
	 * with is not in the list of options. */
	while(choice != 'a' && choice != 'c')
	{
		printf("a. Turn on the vehicle engine\n");
		printf("b. Turn off the vehicle engine\n");
		printf("c. Quit the system\n");
		scanf(" %c", &choice);
		// if the user enters a letter which is not in the options
		if (choice != 'a' && choice != 'b' && choice != 'c'){
				printf("Please enter 'a' or 'b' or 'c'\n");
		}
		// print that the vehicle is on OFF state then loop again
		else if(choice == 'b')
		{
			printf("Turn off the vehicle engine\n\n");
			vec->state = OFF; // Turn OFF the vehicle
		}
	}
	/* if the user chose quit then the function will end
	   and return to main which will end too.
	   Or we can use exit() function but in this case here we
	   don't need to. */
	if(choice == 'c')
	{
		printf("Quit the system\n");
		return;
	}
	/* the last option is ON state, print that the vehicle
	 * is on ON state then go to display sensors set menu. */
	else
	{
		printf("Turn on the vehicle engine\n\n");
		vec->state = ON; // Turn ON the vehicle
		ON_state(vec); // function to display sensors set menu.
	}
}

/* function to display sensors set menu and do something
   depend on user's choice. */
void ON_state(Vehicle *vec)
{
	/* create a variable which will contain the user's choice.
	 * unsigned char because we don't need more than the values
	 * of ASCII alphabets as we don't need the negative values. */
	unsigned char choice;
	/* the loop will continue to execute until the user enters
	 * one the choice a or b or c or sometimes d. */
	do{
		printf("a. Turn off the engine\n");
		printf("b. Set the traffic light color.\n");
		printf("c. Set the room temperature (Temperature Sensor)\n");
#if WITH_ENGINE_TEMP_CONTROLLER
		printf("d. Set the engine temperature (Engine Temperature Sensor)\n");
		scanf(" %c", &choice);
		/* I couldn't put the d choice in the while's condition
		 * dut to the #if directive hence I put it as if
		 * statement alone */
		if (choice == 'd')
			break;
#else
		scanf(" %c", &choice);
#endif
	}while(choice != 'a' && choice != 'b' && choice != 'c');
	// depend on user's choice the program will do something
	switch(choice){
	case 'a':
		// turn OFF the engine
		vec->state = OFF;
		vehicle_state_func(vec);
		break;
	case 'b':
		set_traffic_color(vec);
		break;
	case 'c':
		set_room_temp(vec);
		break;
#if WITH_ENGINE_TEMP_CONTROLLER
	case 'd':
		set_engine_temp(vec);
		break;
#endif
	}
}

void set_traffic_color(Vehicle *vec)
{
	// creating variable to take the user's choice
	unsigned char light;
	printf("Enter traffic color: \n");
	scanf(" %c", &light);
	/* while will execute until the user's enters a suitable
	   choise: 'g' stands for green or 'o' stands for orange
	   or 'r' stands for red. */
	while(light != 'g' && light != 'G' && light != 'o' && light != 'O' && light != 'r' && light != 'R')
	{
		printf("Enter traffic light from green (g)(G) or orange (o)(O) or red (r)(R)\n");
		scanf(" %c", &light);
	}
	/* depend on user's choice the speed of the vehicle will change*/
	switch(light){
	case 'g':
	case 'G':
		vec->speed = 100;
		break;
	case 'o':
	case 'O':
		vec->speed = 30;
		break;
	case 'r':
	case 'R':
		vec->speed = 0;
		break;
	}
	/* check speed function to check if the speed is 30 Km/Hr,
	 * if yes do something. It takes the speed as argument
	 * to check on it, and takes the temp and the state to change
	 * in them. */
	check_speed(&(vec->ac_state), &(vec->room_temp), &(vec->speed));
#if WITH_ENGINE_TEMP_CONTROLLER
	check_speed(&(vec->engine_temp_state), &(vec->engine_temp), &(vec->speed));
#endif
	/* after finishing go print the data of the vehicle
	   then print the menu again. */
	print_data(vec);
	ON_state(vec);
}

void set_room_temp(Vehicle *vec)
{
	// take from the user the room temperature
	printf("Enter Room Temperature: \n");
	scanf(" %hi", &(vec->room_temp));
	/* if the room temp is less than 10 or greater than 30
	   then turn ON the AC and set the room temp to 20. */
	if (vec->room_temp < 10 || vec->room_temp > 30)
	{
		vec->ac_state = ON;
		vec->room_temp = 20;
	}
	// otherwise turn OFF the AC.
	else vec->ac_state = OFF;

	//check if the speed is 30 Km/Hr
	check_speed(&(vec->ac_state), &(vec->room_temp), &(vec->speed));
	/* after finishing go print the data of the vehicle
	   then print the menu again. */
	print_data(vec);
	ON_state(vec);
}

#if WITH_ENGINE_TEMP_CONTROLLER
void set_engine_temp(Vehicle *vec)
{
	// take from the user the engine temperature
	printf("Enter Engine Temperature: \n");
	scanf(" %hi", &(vec->engine_temp));
	/* if the engine temp is less than 100 or greater than 150
	   then turn ON the engine temp controller*/
	if (vec->engine_temp < 100 || vec->engine_temp > 150)
	{
		vec->engine_temp_state = ON;
		vec->engine_temp = 125;
	}
	// otherwise turn OFF the engine temp controller
	else vec->engine_temp_state = OFF;

	//check if the speed is 30 Km/Hr
	check_speed(&(vec->engine_temp_state), &(vec->engine_temp), &(vec->speed));
	/* after finishing go print the data of the vehicle
	   then print the menu again. */
	print_data(vec);
	ON_state(vec);
}
#endif

// function to check if the speed is 30, if yes do something
void check_speed(ON_OFF *state, short *temp, const unsigned short *speed)
{
	//if the speed is equal to 30, some changes will happen
	if (*speed == 30)
	{
		if (*state == OFF)
			*state = ON;
		*temp = (*temp) * (5.0 / 4) + 1;
	}
}

// function to print the vehicle's data
void print_data(const Vehicle *vec)
{
	vec->state ? printf("Engine is ON\n") : printf("Engine is OFF\n");
	vec->ac_state ? printf("AC is ON\n") : printf("AC is OFF\n");
	printf("Vehicle Speed: %hu Km/Hr\n", vec->speed);
	printf("Room Temperature: %hi C\n", vec->room_temp);
#if WITH_ENGINE_TEMP_CONTROLLER
	vec->engine_temp_state ? printf("Engine Temp Controller is ON\n") : printf("Engine Temp Controller is OFF\n");
	printf("Engine Temperature is %hi C\n", vec->engine_temp);
#endif
	printf("\n");
}
