/*
 * Copyright (c) 2017 Samsung Electronics Co., Ltd.
 *
 * Contact: Jin Yoon <jinny.yoon@samsung.com>
 *          Geunsun Lee <gs86.lee@samsung.com>
 *          Eunyoung Lee <ey928.lee@samsung.com>
 *          Junkyu Han <junkyu.han@samsung.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __RCC_RESOURCE_PWM_MOTOR_H__
#define __RCC_RESOURCE_PWM_MOTOR_H__
void resource_close_pwm_motor(void);
int resource_set_pwm_motor_value(double duty_cycle_ms);
#endif

#include <unistd.h>
#include <glib.h>
#include <Ecore.h>
#include <tizen.h>
#include <service_app.h>
#include <stdio.h>


#include "log.h"
#include "resource.h"
#include "connectivity.h"
#include "controller.h"
#include "controller_util.h"
#include "webutil.h"

typedef struct app_data_s {
	Ecore_Timer *getter_timer;
	connectivity_resource_s *resource_info;
} app_data;

#define CONNECTIVITY_KEY "opened"
/* 			value			*/
static double target_object = 0.0f;

/*		 Timer_Duration		*/
#define DISTANCE_DURATION (0.20f)
#define TEMPERATURE_DURATION (1.0f)
#define ILLUMINANCE_DURATION (60.0f)
#define BREAK_DURATION (10.0f)

/*		Sensor_Pin_Number		*/
#define FRONT_US_TRIG 21
#define FRONT_US_ECHO 20
#define BACK_US_TRIG 19
#define BCAK_US_ECHO 26
#define TILT_PIN 25

/* UART */
int port = 0 ;
peripheral_uart_h uart_h;
int state = -1; //Break state
float distance_value[10] = {};
int distance_count = 0;
int count = 0;
int major_count = 0;
int minor_count = 0;
int break_on = 0;

/*						timer 						*/
static Ecore_Timer *handle_break_timer = NULL;
static Ecore_Timer *back_distance_timer = NULL;

/*--------------------- Ultrasonic func -----------------------*/
void __back_distance_cb(double dist, void *data)
{
	_D("back distance : %f ",dist);

	if(distance_count >= 0 && distance_count < 4){
		distance_value[distance_count] = dist;
		distance_count += 1;
		if (distance_count == 4){
			distance_count = -1;
		}
		//_D("%d",distance_count);
	}
	if(distance_count < 0){
		distance_count = 0;
		for(int i = 0; i <4; i++){
			if(distance_value[i] < 30 && distance_value[i] > 0){
				major_count += 1;
			}
			else if(distance_value[i] > 50 && distance_value[i] <150){
				minor_count += 1;
			}
		}
		//_D("state print");
	}
	if(minor_count >= 3){
		//_D("M : %d",major_count);
		//_D("m : %d",minor_count);
		major_count = 0;
		minor_count = 0;
		break_on = 1;
	}
	else if(major_count >=1){
		//_D("M : %d",major_count);
		//_D("m : %d",minor_count);
		major_count = 0;
		minor_count = 0;
		break_on = 0;
	}
	else{
		major_count = 0;
		minor_count = 0;
	}

	if (state == -1 && dist > 50 && dist < 150) { // (state == -1 && break_on == 1)
		state = 1;
		_D("A");
		uint8_t massage[] = "BREAK:ON";
		uint32_t length = sizeof(massage) / sizeof(massage[0]);
		peripheral_uart_write(uart_h, massage, length);
	}
	if (state == 1 && dist > 0 && dist < 30) { // (state == 1 && break_on == 0)
		state = -1;
		_D("B");
		uint8_t massage[] = "BREAK:OFF";
		uint32_t length = sizeof(massage) / sizeof(massage[0]);
		peripheral_uart_write(uart_h, massage, length);
	}
	/*if (state == 1 && break_on == 1){
		uint8_t massage[] = "C\n";
		_D("C");
		uint32_t length = sizeof(massage) / sizeof(massage[0]);
		peripheral_uart_write(uart_h, massage, length);
		uint8_t massage1[] = " ";
		peripheral_uart_write(uart_h, massage1, length);
	}*/
}
Eina_Bool _back_distance_timer_cb(void *data)
{
	int ret = 0;

	ret = resource_read_ultrasonic_sensor(BACK_US_TRIG, BCAK_US_ECHO, __back_distance_cb, NULL);
	retv_if(ret < 0, ECORE_CALLBACK_CANCEL);

	return ECORE_CALLBACK_RENEW;
}

static bool service_app_create(void *data)
{
	_D("create");

	peripheral_uart_open(port, &uart_h);
	peripheral_uart_set_baud_rate(uart_h, PERIPHERAL_UART_BAUD_RATE_9600);
	peripheral_uart_set_byte_size(uart_h, PERIPHERAL_UART_BYTE_SIZE_8BIT);
	peripheral_uart_set_parity(uart_h, PERIPHERAL_UART_PARITY_NONE);
	peripheral_uart_set_stop_bits(uart_h, PERIPHERAL_UART_STOP_BITS_1BIT);
	peripheral_uart_set_flow_control(uart_h, PERIPHERAL_UART_SOFTWARE_FLOW_CONTROL_NONE,
		                                     PERIPHERAL_UART_HARDWARE_FLOW_CONTROL_NONE);

	return true;
}
static void service_app_terminate(void *data)
{
	app_data *ad = (app_data *)data;

	_D("terminate");
	/*		 timer close 		*/
	//if (back_distance_timer)
	ecore_timer_del(back_distance_timer);
	//if (uart_h != NULL)
	peripheral_uart_close(uart_h);
	free(ad);

}
static void service_app_control(app_control_h app_control, void *data)
{
	_D("control");
	/*			timer add			*/
	back_distance_timer = ecore_timer_add(DISTANCE_DURATION, _back_distance_timer_cb, NULL);
	//handle_break_timer = ecore_timer_add(BREAK_DURATION, handle_set_motor_cb, NULL);
	//service_app_exit();

}

static void service_app_lang_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LANGUAGE_CHANGED*/
}

static void service_app_region_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_REGION_FORMAT_CHANGED*/
}

static void service_app_low_battery(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_BATTERY*/
}

static void service_app_low_memory(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_MEMORY*/
}

int main(int argc, char* argv[])
{
	app_data *ad = NULL;
	int ret = 0;
	service_app_lifecycle_callback_s event_callback;
	app_event_handler_h handlers[5] = {NULL, };

	ad = calloc(1, sizeof(app_data));
	retv_if(!ad, -1);

	event_callback.create = service_app_create;
	event_callback.terminate = service_app_terminate;
	event_callback.app_control = service_app_control;

	service_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY], APP_EVENT_LOW_BATTERY, service_app_low_battery, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY], APP_EVENT_LOW_MEMORY, service_app_low_memory, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, service_app_lang_changed, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED], APP_EVENT_REGION_FORMAT_CHANGED, service_app_region_changed, &ad);

	ret = service_app_main(argc, argv, &event_callback, ad);

	return ret;
}
