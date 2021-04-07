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
#ifndef PIPS_H__
#define PIPS_H__ 1
#include <stddef.h>
#include <unistd.h>

#define PIPS_U8     1
#define PIPS_S16_LE 2
#define PIPS_S16_BE 3
#define PIPS_S32_LE 4
#define PIPS_S32_BE 5
#define PIPS_F32_LE 6
#define PIPS_F32_BE 7

#define PIPS_TRUE   1
#define PIPS_FALSE  0

typedef struct pips_generator_info_t {
    unsigned int format;
    unsigned int sample_rate;
    unsigned int channels;
    int(*callback)(void *, unsigned int, unsigned int, void *);
    void *user_data;
    int write_fileno;
} pips_generator_info_t;

typedef struct pips_generator_t {
    pips_generator_info_t info;
    size_t buffer_size;
    void *buffer;
} pips_generator_t;

int PIPS_createGenerator(pips_generator_t *generator, const pips_generator_info_t *info);
void PIPS_destroyGenerator(pips_generator_t *generator);
void PIPS_generate(pips_generator_t *generator, size_t seconds);

#endif
