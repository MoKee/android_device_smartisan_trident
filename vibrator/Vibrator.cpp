/*
 * Copyright (C) 2019 The Android Open Source Project
 * Copyright (C) 2019 The MoKee Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "VibratorService"

#include <errno.h>
#include <fcntl.h>
#include <malloc.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <log/log.h>

#include "Vibrator.h"

#define TIMEOUT_STR_LEN         20

namespace android {
namespace hardware {
namespace vibrator {
namespace V1_0 {
namespace implementation {

static const char LED_DEVICE[] = "/sys/class/leds/vibrator";

static int write_value(const char *file, const char *value)
{
    int to_write, written, ret, fd;

    fd = TEMP_FAILURE_RETRY(open(file, O_WRONLY));
    if (fd < 0) {
        return -errno;
    }

    to_write = strlen(value) + 1;
    written = TEMP_FAILURE_RETRY(write(fd, value, to_write));
    if (written == -1) {
        ret = -errno;
    } else if (written != to_write) {
        /* even though EAGAIN is an errno value that could be set
           by write() in some cases, none of them apply here.  So, this return
           value can be clearly identified when debugging and suggests the
           caller that it may try to call vibrator_on() again */
        ret = -EAGAIN;
    } else {
        ret = 0;
    }

    errno = 0;
    close(fd);

    return ret;
}

static int write_led_file(const char *file, const char *value)
{
    char file_str[50];

    snprintf(file_str, sizeof(file_str), "%s/%s", LED_DEVICE, file);
    return write_value(file_str, value);
}

static int vibra_led_on(unsigned int timeout_ms)
{
    int ret;
    char value[TIMEOUT_STR_LEN]; /* large enough for millions of years */

    ret = write_led_file("state", "1");
    if (ret)
        return ret;

    snprintf(value, sizeof(value), "%u\n", timeout_ms);
    ret = write_led_file("duration", value);
    if (ret)
        return ret;

    return write_led_file("activate", "1");
}

static int vibra_led_off()
{
    return write_led_file("activate", "0");
}

Vibrator::Vibrator() {}

// Methods from ::android::hardware::vibrator::V1_0::IVibrator follow.

Return<Status> Vibrator::on(uint32_t timeoutMs) {
    int32_t ret = vibra_led_on(timeoutMs);
    if (ret != 0) {
        ALOGE("on command failed : %s", strerror(-ret));
        return Status::UNKNOWN_ERROR;
    }
    return Status::OK;
}

Return<Status> Vibrator::off() {
    int32_t ret = vibra_led_off();
    if (ret != 0) {
        ALOGE("off command failed : %s", strerror(-ret));
        return Status::UNKNOWN_ERROR;
    }
    return Status::OK;
}

Return<bool> Vibrator::supportsAmplitudeControl() {
    return false;
}

Return<Status> Vibrator::setAmplitude(uint8_t) {
    return Status::UNSUPPORTED_OPERATION;
}

Return<void> Vibrator::perform(V1_0::Effect, EffectStrength, perform_cb _hidl_cb) {
    _hidl_cb(Status::UNSUPPORTED_OPERATION, 0);
    return Void();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace vibrator
}  // namespace hardware
}  // namespace android
