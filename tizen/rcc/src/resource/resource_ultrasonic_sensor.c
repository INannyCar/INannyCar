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

#include <stdlib.h>
#include <unistd.h>
#include <peripheral_io.h>
#include <sys/time.h>
#include <gio/gio.h>

#include "log.h"
#include "resource_internal.h"

static resource_read_s *resource_read_info = NULL;
static unsigned long long triggered_time = 0;

void resource_close_ultrasonic_sensor_trig(int trig_pin_num)
{
	_I("Ultrasonic sensor's trig is finishing...");

	if (resource_get_info(trig_pin_num)->opened) {
		peripheral_gpio_close(resource_get_info(trig_pin_num)->sensor_h);
		resource_get_info(trig_pin_num)->opened = 0;
	}
}

void resource_close_ultrasonic_sensor_echo(int echo_pin_num)
{
	int ret = PERIPHERAL_ERROR_NONE;

	_I("Ultrasonic sensor's echo is finishing...");

	if (resource_get_info(echo_pin_num)->opened) {
		ret = peripheral_gpio_unset_interrupted_cb(resource_get_info(echo_pin_num)->sensor_h);
		if (ret != PERIPHERAL_ERROR_NONE) {
			_E("It fails to unset the interrupted callback");
		}

		peripheral_gpio_close(resource_get_info(echo_pin_num)->sensor_h);
		resource_get_info(echo_pin_num)->opened = 0;
	}

	free(resource_read_info);
	resource_read_info = NULL;
}

static unsigned long long _get_timestamp(void)
{
	struct timespec t;
	clock_gettime(CLOCK_REALTIME, &t);
	return ((unsigned long long)(t.tv_sec)*1000000000LL + t.tv_nsec) / 1000;
}

static void _resource_read_ultrasonic_sensor_cb(peripheral_gpio_h gpio, peripheral_error_e error, void *user_data)
{
	//_D("userdata %d", &user_data);
	float dist = 0;
	static uint32_t value = 0;
	static unsigned long long triggered_time = 0;
	static unsigned long long returned_time = 0;
	resource_read_s *resource_read_info = user_data;
	int ret = PERIPHERAL_ERROR_NONE;

	ret_if(!resource_read_info);
	//_D("error1");
	ret_if(!resource_read_info->cb);
	//_D("error2");

	ret = peripheral_gpio_read(gpio, &value);
	//_D("error3 %d",ret);
	ret_if(ret != PERIPHERAL_ERROR_NONE);

	//_D("_resource_read_ultrasonic_sensor_cb222");
	//_D("value %d", value);
	if (value) {
		triggered_time = _get_timestamp();
	} else if (value == 0) {
		returned_time = _get_timestamp();
	}

	if (triggered_time > 0 && value == 0) {
		dist = returned_time - triggered_time;
		if (dist < 150 || dist > 25000) {
			dist = -1;
		} else {
			dist = (dist * 34300) / 2000000;
		}
		resource_read_info->cb(dist, resource_read_info->data);
		triggered_time = 0l;
	}
}

int resource_read_ultrasonic_sensor(int trig_pin_num, int echo_pin_num, resource_read_cb cb, void *data)
{
	int ret = 0;

	triggered_time = 0;

	if (!resource_get_info(trig_pin_num)->opened) {
		_I("Ultrasonic sensor's trig is initializing...%d",trig_pin_num);

		ret = peripheral_gpio_open(trig_pin_num, &resource_get_info(trig_pin_num)->sensor_h);
		retv_if(ret != PERIPHERAL_ERROR_NONE, -1);

		resource_get_info(trig_pin_num)->opened = 1;

		ret = peripheral_gpio_set_direction(resource_get_info(trig_pin_num)->sensor_h, PERIPHERAL_GPIO_DIRECTION_OUT_INITIALLY_LOW);
		goto_if(ret != PERIPHERAL_ERROR_NONE, ERROR);

		resource_get_info(trig_pin_num)->close = resource_close_ultrasonic_sensor_trig;
	}

	if (!resource_get_info(echo_pin_num)->opened) {
		_I("Ultrasonic sensor's echo is initializing...%d",echo_pin_num);

		resource_read_info = calloc(1, sizeof(resource_read_s));
		goto_if(!resource_read_info, ERROR);

		resource_read_info->cb = cb;
		resource_read_info->data = data;
		resource_read_info->pin_num = echo_pin_num;

		ret = peripheral_gpio_open(echo_pin_num, &resource_get_info(echo_pin_num)->sensor_h);
		//_I("open error");
		goto_if(ret != PERIPHERAL_ERROR_NONE, ERROR);

		resource_get_info(echo_pin_num)->opened = 1;

		ret = peripheral_gpio_set_direction(resource_get_info(echo_pin_num)->sensor_h, PERIPHERAL_GPIO_DIRECTION_IN);
		//_I("deirection error");
		goto_if(ret != PERIPHERAL_ERROR_NONE, ERROR);

		ret = peripheral_gpio_set_edge_mode(resource_get_info(echo_pin_num)->sensor_h, PERIPHERAL_GPIO_EDGE_BOTH);
		/* WARNING : Setting edge mode returns an error at the first time. */
		//goto_if(ret != PERIPHERAL_ERROR_NONE, ERROR);

		ret = peripheral_gpio_set_interrupted_cb(resource_get_info(echo_pin_num)->sensor_h, _resource_read_ultrasonic_sensor_cb, resource_read_info);
		//_I("interrupted error");
		//_D("%d",resource_get_info(echo_pin_num)->sensor_h);
		//_D("%d",_resource_read_ultrasonic_sensor_cb);
		//_D("%d", resource_read_info);
		goto_if(ret != PERIPHERAL_ERROR_NONE, ERROR);

		resource_get_info(echo_pin_num)->close = resource_close_ultrasonic_sensor_echo;
	}

	ret = peripheral_gpio_write(resource_get_info(trig_pin_num)->sensor_h, 0);
	//_D("gpio_write_1 %d",ret);
	goto_if(ret != PERIPHERAL_ERROR_NONE, ERROR);

	usleep(20000);

	ret = peripheral_gpio_write(resource_get_info(trig_pin_num)->sensor_h, 1);
	goto_if(ret != PERIPHERAL_ERROR_NONE, ERROR);
	//_D("gpio_write_2 %d",ret);

	usleep(20000);

	ret = peripheral_gpio_write(resource_get_info(trig_pin_num)->sensor_h, 0);
	goto_if(ret != PERIPHERAL_ERROR_NONE, ERROR);
	//_D("gpio_write_3 %d", ret);

	return 0;

ERROR:
	resource_close_ultrasonic_sensor_trig(trig_pin_num);
	resource_close_ultrasonic_sensor_echo(echo_pin_num);
	_I("error!!!!!!!!!!!!!!!");
	_D("error!!!!!!!!!!!!!!!");

	return -1;
}
