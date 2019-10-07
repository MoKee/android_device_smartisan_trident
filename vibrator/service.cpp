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
#define LOG_TAG "android.hardware.vibrator@1.3-service.trident"

#include <android-base/logging.h>
#include <android/hardware/vibrator/1.3/IVibrator.h>
#include <hidl/HidlTransportSupport.h>

#include "Vibrator.h"

using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;

using android::hardware::vibrator::V1_3::IVibrator;
using android::hardware::vibrator::V1_3::implementation::Vibrator;

const static std::string kActivatePath = "/sys/class/leds/vibrator/activate";
const static std::string kDurationPath = "/sys/class/leds/vibrator/duration";
const static std::string kAmplitudePath = "/sys/class/haptic/player/amp";

int main() {
    std::ofstream activate(kActivatePath);
    if (!activate) {
        LOG(ERROR) << "Failed to open " << kActivatePath << ", error=" << errno
                   << " (" << strerror(errno) << ")";
        return -errno;
    }

    std::ofstream duration(kDurationPath);
    if (!duration) {
        LOG(ERROR) << "Failed to open " << kDurationPath << ", error=" << errno
                   << " (" << strerror(errno) << ")";
        return -errno;
    }

    std::ofstream amplitude(kAmplitudePath);
    if (!amplitude) {
        LOG(ERROR) << "Failed to open " << kAmplitudePath << ", error=" << errno
                   << " (" << strerror(errno) << ")";
        return -errno;
    }

    android::sp<IVibrator> vibrator = new Vibrator(
        std::move(activate), std::move(duration), std::move(amplitude));

    configureRpcThreadpool(1, true);

    android::status_t status = vibrator->registerAsService();

    if (status != android::OK) {
        LOG(ERROR) << "Cannot register Vibrator HAL service";
        return status;
    }

    LOG(INFO) << "Vibrator HAL Ready.";
    joinRpcThreadpool();

    // Under normal cases, execution will not reach this line.
    LOG(ERROR) << "Vibrator HAL failed to join thread pool.";

    return 1;
}
