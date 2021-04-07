/*
 * tetris.c - plays Tetris theme. Bazinga!
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
#include <math.h>
#include "pips.h"

#define MX_TAU 6.283185f

typedef struct beep_t {
    float frequency;
    float ms;
} beep_t;

// https://gist.github.com/erineccleston/0d65a78e9df88d68a13a421d2a298f10
static const beep_t beeps[] = {
    { 1320, 500 },
    { 990,  250 },
    { 1056, 250 },
    { 1188, 250 },
    { 1320, 125 },
    { 1188, 125 },
    { 1056, 250 },
    { 990,  250 },
    { 880,  500 },
    { 0,    10  },
    { 880,  250 },
    { 1056, 250 },
    { 1320, 500 },
    { 1188, 250 },
    { 1056, 250 },
    { 990,  750 },
    { 1056, 250 },
    { 1188, 500 },
    { 1320, 500 },
    { 1056, 500 },
    { 880,  500 },
    { 0,    10  },
    { 880,  500 },
    { 0,    250 },
    { 1188, 500 },
    { 1408, 250 },
    { 1760, 500 },
    { 1584, 250 },
    { 1408, 250 },
    { 1320, 750 },
    { 1056, 250 },
    { 1320, 500 },
    { 1188, 250 },
    { 1056, 250 },
    { 990,  500 },
    { 0,    10  },
    { 990,  250 },
    { 1056, 250 },
    { 1188, 500 },
    { 1320, 500 },
    { 1056, 500 },
    { 880,  500 },
    { 0,    10  },
    { 880,  500 },
    { 0,    500 }
};

#define NUM_BEEPS (sizeof(beeps)/sizeof(beeps[0]))

typedef struct callback_data_t {
    size_t cur_beep;
    float beep_timeout;
} callback_data_t;

static int callback(void *out, unsigned int samples, unsigned int channels, void *data)
{
    float *out_f = (float *)out;
    callback_data_t *data_p = (callback_data_t *)data;
    for(unsigned int i = 0; i < samples; i++) {
        *out_f++ = sinf(beeps[data_p->cur_beep].frequency * i * MX_TAU / samples);
        data_p->beep_timeout -= 1000.0f / samples;
        if(data_p->beep_timeout <= 0.0f) {
            if(++data_p->cur_beep >= NUM_BEEPS)
                data_p->cur_beep = 0;
            data_p->beep_timeout = beeps[data_p->cur_beep].ms;
        }
    }

    return PIPS_TRUE;
}

// ./sine_440 | aplay -f FLOAT_LE -r 8000 -c 1
int main(void)
{
    callback_data_t data;
    data.cur_beep = 0;
    data.beep_timeout = beeps[0].ms;

    // Again some sample rate hacks.
    // The beeps sound better when played back at 9000 Hz
    // sample rate while we are targeting 8000 Hz.
    // So 9000 / 8000 = 1.125
    // And then 8000 / 1.125 = 7111.11
    pips_generator_info_t info;
    info.format = PIPS_PCM32F;
    info.sample_rate = 7111; // so it would sound like 9000 Hz
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

