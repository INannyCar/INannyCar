/*
 * Obj : tizen pratice and design motorfuntion for senior project
 * Creation date : 2020 . 5 . 27
 * Orignal Writer : S2h
 * Modified date : 2020. 5. 27
 */

#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <peripheral_io.h>
#include "log.h"

#define SERVO_MOTOR_CHANNER 12

static peripheral_pwm_h g_pwm_h;

void resource_close_pwm_motor(void)
{
	_D("pwm_motor_end...");
	if(g_pwm_h){
		peripheral_pwm_close(g_pwm_h);
		g_pwm_h = NULL;
	}
}

int resource_set_pwm_motor_value(double duty_cycle_ms)
{
	int ret = 0;

	if(!g_pwm_h){
		ret = peripheral_pwm_open(0, SERVO_MOTOR_CHANNER, &g_pwm_h);
		if (ret != PERIPHERAL_ERROR_NONE) {
			_E("failed to open servo motor with ch : %s", get_error_message(ret));
			return -1;
		}
	}

	ret = peripheral_pwm_set_period(g_pwm_h, 20 * 1000 * 1000);
	if (ret != PERIPHERAL_ERROR_NONE) {
		_E("failed to set pwm motor");
		return -1;
	}

	ret = peripheral_pwm_set_duty_cycle(g_pwm_h, duty_cycle_ms * 1000 * 1000);
	if (ret != PERIPHERAL_ERROR_NONE) {
		_E("failed to duty pwm motor");
		return -1;
	}

	ret = peripheral_pwm_set_polarity(g_pwm_h, PERIPHERAL_PWM_POLARITY_ACTIVE_HIGH);
	if (ret != PERIPHERAL_ERROR_NONE) {
		_E("failed to polarity pwm motor");
		return -1;
	}

	ret = peripheral_pwm_set_enabled(g_pwm_h, true);
	if (ret != PERIPHERAL_ERROR_NONE) {
		_E("failed to enable pwm motor");
		return -1;
	}

	return 0;
}

