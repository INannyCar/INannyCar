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

#ifndef __RCC_RESOURCE_LED_H__
#define __RCC_RESOURCE_LED_H__

/**
 * @brief Writes the value of gpio connected to the led.
 * @param[in] pin_num The number of the gpio pin connected to the led
 * @param[in] write_value The value for led (zero or non-zero)
 * @return 0 on success, otherwise a negative error value
 */
extern int resource_write_led(int pin_num, int write_value);

#endif /* __RCC_RESOURCE_LED_H__ */
