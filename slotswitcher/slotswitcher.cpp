/*
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

#include <hardware/hardware.h>
#include <hardware/boot_control.h>

#define SLOT_A 0

extern const hw_module_t HAL_MODULE_INFO_SYM;

extern "C" {

int main() {
    const hw_module_t* hw_module = &HAL_MODULE_INFO_SYM;
    boot_control_module_t* module = reinterpret_cast<boot_control_module_t*>(const_cast<hw_module_t*>(hw_module));

    module->init(module);

    if (module->setActiveBootSlot(module, SLOT_A)) {
        return -2;
    }

    return 0;
}

}
