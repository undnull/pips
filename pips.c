/*
 * pips.h
 * Copyright (c) 2021 Kirill GPRB
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "pips.h"

static size_t PIPS_calcBufferSize(unsigned int format, unsigned int sample_rate, unsigned int channels)
{
    size_t size = 1;
    switch(format) {
        case PIPS_U8:
            size = sizeof(uint_least8_t);
            break;
        
        case PIPS_S16_LE:
        case PIPS_S16_BE:
            size = sizeof(uint_least16_t);
            break;
        
        case PIPS_S32_LE:
        case PIPS_S32_BE:
        case PIPS_F32_LE:
        case PIPS_F32_BE:
            size = sizeof(uint_least32_t);
            break;
    }
    return size * sample_rate * channels;
}

int PIPS_createGenerator(pips_generator_t *generator, const pips_generator_info_t *info)
{
    if(generator && info && info->callback) {
        memcpy(&generator->info, info, sizeof(pips_generator_info_t));
        generator->buffer_size = PIPS_calcBufferSize(generator->info.format, generator->info.sample_rate, generator->info.channels);
        generator->buffer = malloc(generator->buffer_size);
        return generator->buffer ? PIPS_TRUE : PIPS_FALSE;
    }

    return PIPS_FALSE;
}

void PIPS_destroyGenerator(pips_generator_t *generator)
{
    if(generator && generator->buffer) {
        free(generator->buffer);
        memset(generator, 0, sizeof(pips_generator_t));
    }
}

void PIPS_generate(pips_generator_t *generator, size_t seconds)
{
    int finite = !!seconds;
    int running = PIPS_TRUE;
    do {
        memset(generator->buffer, 0, generator->buffer_size);
        running = generator->info.callback(generator->buffer, generator->info.sample_rate, generator->info.channels, generator->info.user_data);
        if(finite)
            running = running && seconds--;
        write(generator->info.write_fileno, generator->buffer, generator->buffer_size);
    } while(running == PIPS_TRUE);
}
