/*
 * Copyright 2014 - 2017 Real Logic Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AERON_AERONMD_H
#define AERON_AERONMD_H

#include <stdbool.h>
#include <stdint.h>
#include "util/aeron_bitutil.h"

#define AERON_MAX_PATH (256)
#define AERON_CNC_FILE "cnc.dat"
#define AERON_CNC_VERSION (5)

#pragma pack(push)
#pragma pack(4)
typedef struct aeron_cnc_metadata_stct
{
    int32_t cnc_version;
    int32_t to_driver_buffer_length;
    int32_t to_clients_buffer_length;
    int32_t counter_metadata_buffer_length;
    int32_t counter_values_buffer_length;
    int64_t client_liveness_timeout;
    int32_t error_log_buffer_length;
}
aeron_cnc_metadata_t;
#pragma pack(pop)

#define AERON_CNC_VERSION_AND_META_DATA_LENGTH (AERON_ALIGN(sizeof(aeron_cnc_metadata_t), AERON_CACHE_LINE_LENGTH * 2))

typedef int64_t (*aeron_clock_func_t)();

typedef enum aeron_threading_mode_enum
{
    AERON_THREADING_MODE_DEDICATED,
    AERON_THREADING_MODE_SHARED_NETWORK,
    AERON_THREADING_MODE_SHARED
}
aeron_threading_mode_t;

typedef struct aeron_driver_context_stct
{
    char *aeron_dir;                        /* aeron.dir */
    aeron_threading_mode_t threading_mode;  /* aeron.threading.mode */
    bool dirs_delete_on_start;              /* aeron.dir.delete.on.start */
}
aeron_driver_context_t;

typedef struct aeron_driver_stct
{
    aeron_driver_context_t *context;
}
aeron_driver_t;

/* load settings from Java properties file (https://en.wikipedia.org/wiki/.properties) and set env vars */
int aeron_driver_load_properties_file(const char *filename);

/* init defaults and from env vars */
int aeron_driver_context_init(aeron_driver_context_t **context);
int aeron_driver_context_close(aeron_driver_context_t *context);

/* init driver from context */
int aeron_driver_init(aeron_driver_t **driver, aeron_driver_context_t *context);
int aeron_driver_start(aeron_driver_t *driver);
int aeron_driver_close(aeron_driver_t *driver);

int aeron_dir_delete(const char *dirname);

typedef void (*aeron_log_func_t)(const char *);
bool aeron_is_driver_active(const char *dirname, int64_t timeout, int64_t now, aeron_log_func_t log_func);

inline uint8_t *aeron_cnc_to_driver_buff(aeron_cnc_metadata_t *metadata)
{
    return (uint8_t *)metadata + AERON_CNC_VERSION_AND_META_DATA_LENGTH;
}

#endif //AERON_AERONMD_H
