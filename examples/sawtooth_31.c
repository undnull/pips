/*
 * sawtooth_31.c - emits a 31.25 Hz sawtooth wave.
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
#include "pips.h"

static int callback(void *out, unsigned int samples, unsigned int channels, void *data)
{
    uint_least8_t *out_b = (uint_least8_t *)out;
    uint_least8_t *data_p = (uint_least8_t *)data;
    for(unsigned int i = 0; i < samples; i++)
        *out_b++ = (*data_p)++;
    return PIPS_TRUE;
}

// ./sawtooth_31 | aplay -f U8 -r 8000 -c 1
int main(void)
{
    uint_least8_t data = 0;

    // Here we do a little trick.
    // Since 8-bit integers are in range of [0; 255]
    // we can calculate the exact frequency that will
    // be produced: 8000 / 256 = 31.25 Hz
    pips_generator_info_t info;
    info.format = PIPS_U8;
    info.sample_rate = 8000;
    info.channels = 1;
    info.callback = callback;
    info.user_data = &data;
    info.write_fileno = STDOUT_FILENO;

    pips_generator_t generator;
    if(!PIPS_createGenerator(&generator, &info))
        return 1;

    PIPS_generate(&generator, 0);
    PIPS_destroyGenerator(&generator);
    return 1;
}
