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

#include "Vibrator.h"

#include <android-base/logging.h>

namespace android {
namespace hardware {
namespace vibrator {
namespace V1_3 {
namespace implementation {

Vibrator::Vibrator(std::ofstream&& activate,
                   std::ofstream&& duration)
    : mActivate(std::move(activate))
    , mDuration(std::move(duration))
    {
}

// Methods from ::android::hardware::vibrator::V1_0::IVibrator follow.

Return<Status> Vibrator::on(uint32_t timeoutMs) {
    mDuration << timeoutMs << std::endl;
    mActivate << 1 << std::endl;
    return Status::OK;
}

Return<Status> Vibrator::off() {
    mActivate << 0 << std::endl;
    return Status::OK;
}

Return<bool> Vibrator::supportsAmplitudeControl() {
    return false;
}

Return<Status> Vibrator::setAmplitude(__unused uint8_t amplitude) {
    return Status::UNSUPPORTED_OPERATION;
}

Return<void> Vibrator::perform(V1_0::Effect effect, EffectStrength strength, perform_cb _hidl_cb) {
    return perform<decltype(effect)>(effect, strength, _hidl_cb);
}

// Methods from ::android::hardware::vibrator::V1_1::IVibrator follow.

Return<void> Vibrator::perform_1_1(V1_1::Effect_1_1 effect, EffectStrength strength,
                                   perform_cb _hidl_cb) {
    return perform<decltype(effect)>(effect, strength, _hidl_cb);
}

// Methods from ::android::hardware::vibrator::V1_2::IVibrator follow.

Return<void> Vibrator::perform_1_2(V1_2::Effect effect, EffectStrength strength,
                                   perform_cb _hidl_cb) {
    return perform<decltype(effect)>(effect, strength, _hidl_cb);
}

// Methods from ::android::hardware::vibrator::V1_3::IVibrator follow.

Return<bool> Vibrator::supportsExternalControl() {
    return false;
}

Return<Status> Vibrator::setExternalControl(__unused bool enabled) {
    return Status::UNSUPPORTED_OPERATION;
}

Return<void> Vibrator::perform_1_3(Effect effect, EffectStrength strength, perform_cb _hidl_cb) {
    return perform<decltype(effect)>(effect, strength, _hidl_cb);
}

// Private methods follow.

Return<void> Vibrator::perform(Effect effect, EffectStrength strength, perform_cb _hidl_cb) {
    uint32_t ms;
    Status status = Status::OK;

    LOG(INFO) << "Perform: Effect " << effectToName(effect);

    ms = effectToMs(effect, &status);
    if (status != Status::OK) {
        _hidl_cb(status, 0);
        return Void();
    }

    mDuration << ms << std::endl;
    mActivate << 1 << std::endl;

    _hidl_cb(status, ms);

    return Void();
}

template <typename T>
Return<void> Vibrator::perform(T effect, EffectStrength strength, perform_cb _hidl_cb) {
    auto validRange = hidl_enum_range<T>();
    if (effect < *validRange.begin() || effect > *std::prev(validRange.end())) {
        _hidl_cb(Status::UNSUPPORTED_OPERATION, 0);
        return Void();
    }
    return perform(static_cast<Effect>(effect), strength, _hidl_cb);
}

const std::string Vibrator::effectToName(Effect effect) {
    return toString(effect);
}

uint32_t Vibrator::effectToMs(Effect effect, Status* status) {
    switch (effect) {
        case Effect::CLICK:
            return 10;
        case Effect::DOUBLE_CLICK:
            return 15;
        case Effect::TICK:
        case Effect::TEXTURE_TICK:
            return 5;
        case Effect::THUD:
            return 5;
        case Effect::POP:
            return 5;
        case Effect::HEAVY_CLICK:
            return 10;
        case Effect::RINGTONE_1:
            return 30000;
        case Effect::RINGTONE_2:
            return 30000;
        case Effect::RINGTONE_3:
            return 30000;
        case Effect::RINGTONE_4:
            return 30000;
        case Effect::RINGTONE_5:
            return 30000;
        case Effect::RINGTONE_6:
            return 30000;
        case Effect::RINGTONE_7:
            return 30000;
        case Effect::RINGTONE_8:
            return 30000;
        case Effect::RINGTONE_9:
            return 30000;
        case Effect::RINGTONE_10:
            return 30000;
        case Effect::RINGTONE_11:
            return 30000;
        case Effect::RINGTONE_12:
            return 30000;
        case Effect::RINGTONE_13:
            return 30000;
        case Effect::RINGTONE_14:
            return 30000;
        case Effect::RINGTONE_15:
            return 30000;
    }
    *status = Status::UNSUPPORTED_OPERATION;
    return 0;
}

}  // namespace implementation
}  // namespace V1_3
}  // namespace vibrator
}  // namespace hardware
}  // namespace android
