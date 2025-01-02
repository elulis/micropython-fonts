/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2024 Eluli
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef MICROPY_INCLUDED_FONT_Z1FONTS_H
#define MICROPY_INCLUDED_FONT_Z1FONTS_H

/*
 * To store 4 styles of fonts in the least space,
 * this file uses an approach that starts with a regular proportional font (up to 5 columns/bytes)
 * and adds additional information (3 bytes to utilize) for each column type,
 * allowing conversion into three other font styles:
 * regular & mono, bold & proportional, and bold & mono.
 *
 * There are 3 types of modifications:
 * 1. **Dup Append**: Insert a blank column after this column, duplicating this column (extends width by 1 pixel)
 * 2. **Dup Next**: Duplicate this column to the next column (does not affect width)
 * 3. **Dup Prev:** Duplicate this column to the previous column (does not affect width)

 * And there are 5 types of columns:
 * 1. `m` **Mono-cols**: Perform a Dup Append (extends width by 1) when p2m(converting from proportional to mono)
 * 2. `dn` **bold-cols-DN**: Perform a Dup Next (does not affect width) when r2b(converting from regular to bold)
 * 3. `dp` **bold-cols-DP**: Perform a Dup Prev when r2b
 * 4. `da` **bold-cols-DA**: Perform a Dup Append(extends width by 1) when r2b
 * 5. `-`  **Normal column**: No modification
 * Rules:
 * - `m` can combine with `dn` & `dp` but not `da`.
 * - The first column cannot be `dp`.
 * - Characters with five columns do not need alignment.
 *
 * Each character takes 8 bytes (5 bytes for 5 columns, 3 bytes for info including each column's type).
 * Example transformations: for 'b' 4 cols 0 da, 1 m, 2 -, 3 dp:
 * prop(base)  mono  prop_b  mono_b
 *  *          *      **     **
 *  *          *      **     **
 *  ***        ****   ****   *****
 *  *  *       *   *  ** **  **  **
 *  *  *       *   *  ** **  **  **
 *  *  *       *   *  ** **  **  **
 *  ***        ****   ****   *****
 *
 * Font names are:
 * - `z1mono8_6x8`: monospace & regular
 * - `z1mono8b_8x8`: monospace & bold
 * - `z1prop8_6x8`: proportional & regular, base font (`font_z1prop8_base_8x8`)
 * - `z1prop8b_8x8`: proportional & bold
 */

static const uint8_t font_z1prop8_base_8x8[] = {
    // pixel array: a[0:5]
    // columns(4 bits): a[5] MSB
    // column types(4 bits): a[5] LSB, a[6] MSB, a[6] LSB, a[7] MSB, a[7] LSB
    // p2m & r2b column appends(2 x 2bits): a[7] LSB (if columns <= 4, for alignment)
    // column type: m 0x8, da 0x4, dp 0x2, dn 0x1, - 0x0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x04, 0x01, // 32= : 2 cols, 0~1 -, 2 da
    0x00, 0xdf, 0x00, 0x00, 0x00, 0x30, 0x40, 0x01, // 33=!: 3 cols, 0 -, 1 da, 2 -
    0x07, 0x00, 0x07, 0x00, 0x00, 0x34, 0x04, 0x02, // 34=": 3 cols, 0 da, 1 -, 2 da
    0x14, 0x7f, 0x14, 0x7f, 0x14, 0x50, 0x40, 0x40, // 35=#: 5 cols, 0 -, 1 da, 2 -, 3 da, 4 -
    0x24, 0x2a, 0x7f, 0x2a, 0x12, 0x50, 0x04, 0x00, // 36=$: 5 cols, 0~1 -, 2 da, 3~4 -
    0x26, 0x10, 0x08, 0x04, 0x32, 0x51, 0x11, 0x14, // 37=%: 5 cols, 0~3 dn, 4 da
    0x36, 0x49, 0x49, 0x26, 0x50, 0x51, 0x11, 0x14, // 38=&: 5 cols, 0~3 dn, 4 da
    0x00, 0x07, 0x00, 0x00, 0x00, 0x30, 0x40, 0x01, // 39=': 3 cols, 0 -, 1 da, 2 -
    0x1c, 0x22, 0x41, 0x00, 0x00, 0x31, 0x14, 0x01, // 40=(: 3 cols, 0~1 dn, 2 da
    0x41, 0x22, 0x1c, 0x00, 0x00, 0x31, 0x14, 0x01, // 41=): 3 cols, 0~1 dn, 2 da
    0x14, 0x0e, 0x14, 0x00, 0x00, 0x30, 0x40, 0x01, // 42=*: 3 cols, 0 -, 1 da, 2 -
    0x08, 0x08, 0x3e, 0x08, 0x08, 0x50, 0x04, 0x00, // 43=+: 5 cols, 0~1 -, 2 da, 3~4 -
    0x80, 0x60, 0x00, 0x00, 0x00, 0x31, 0x40, 0x01, // 44=,: 3 cols, 0 dn, 1 da, 2 -
    0x08, 0x08, 0x08, 0x08, 0x00, 0x40, 0x08, 0x45, // 45=-: 4 cols, 0~1 -, 2 m, 3 da
    0x00, 0xc0, 0x00, 0x00, 0x00, 0x30, 0x40, 0x01, // 46=.: 3 cols, 0 -, 1 da, 2 -
    0x60, 0x18, 0x06, 0x01, 0x00, 0x41, 0x11, 0x41, // 47=/: 4 cols, 0~2 dn, 3 da
    0x3e, 0x51, 0x49, 0x45, 0x3e, 0x51, 0x00, 0x14, // 48=0: 5 cols, 0 dn, 1~2 -, 3 dn, 4 da
    0x40, 0x42, 0x7f, 0x40, 0x40, 0x50, 0x11, 0x14, // 49=1: 5 cols, 0 -, 1~3 dn, 4 da
    0x42, 0x61, 0x51, 0x49, 0x46, 0x51, 0x11, 0x14, // 50=2: 5 cols, 0~3 dn, 4 da
    0x41, 0x49, 0x49, 0x49, 0x36, 0x50, 0x00, 0x14, // 51=3: 5 cols, 0~2 -, 3 dn, 4 da
    0x1f, 0x10, 0x10, 0x10, 0x7f, 0x51, 0x00, 0x04, // 52=4: 5 cols, 0 dn, 1~3 -, 4 da
    0x27, 0x45, 0x45, 0x45, 0x39, 0x51, 0x00, 0x14, // 53=5: 5 cols, 0 dn, 1~2 -, 3 dn, 4 da
    0x3e, 0x49, 0x49, 0x49, 0x32, 0x51, 0x00, 0x14, // 54=6: 5 cols, 0 dn, 1~2 -, 3 dn, 4 da
    0x01, 0x01, 0x79, 0x05, 0x03, 0x50, 0x01, 0x14, // 55=7: 5 cols, 0~1 -, 2~3 dn, 4 da
    0x36, 0x49, 0x49, 0x49, 0x36, 0x51, 0x00, 0x14, // 56=8: 5 cols, 0 dn, 1~2 -, 3 dn, 4 da
    0x26, 0x49, 0x49, 0x49, 0x3e, 0x51, 0x00, 0x14, // 57=9: 5 cols, 0 dn, 1~2 -, 3 dn, 4 da
    0x00, 0x66, 0x00, 0x00, 0x00, 0x30, 0x40, 0x01, // 58=:: 3 cols, 0 -, 1 da, 2 -
    0x80, 0x66, 0x00, 0x00, 0x00, 0x31, 0x40, 0x01, // 59=;: 3 cols, 0 dn, 1 da, 2 -
    0x08, 0x14, 0x22, 0x41, 0x00, 0x41, 0x11, 0x41, // 60=<: 4 cols, 0~2 dn, 3 da
    0x14, 0x14, 0x14, 0x14, 0x00, 0x44, 0x80, 0x05, // 61==: 4 cols, 0 da, 1 m, 2~3 -
    0x41, 0x22, 0x14, 0x08, 0x00, 0x41, 0x11, 0x41, // 62=>: 4 cols, 0~2 dn, 3 da
    0x06, 0x01, 0xd1, 0x09, 0x06, 0x51, 0x01, 0x14, // 63=?: 5 cols, 0 dn, 1 -, 2~3 dn, 4 da
    0x34, 0x4a, 0x7a, 0x42, 0x3c, 0x54, 0x20, 0x02, // 64=@: 5 cols, 0 da, 1 dp, 2~3 -, 4 dp
    0x7c, 0x12, 0x11, 0x12, 0x7c, 0x51, 0x11, 0x14, // 65=A: 5 cols, 0~3 dn, 4 da
    0x7f, 0x49, 0x49, 0x49, 0x36, 0x51, 0x00, 0x14, // 66=B: 5 cols, 0 dn, 1 -, 2 -, 3 dn, 4 da
    0x3e, 0x41, 0x41, 0x41, 0x22, 0x51, 0x00, 0x14, // 67=C: 5 cols, 0 dn, 1 -, 2 -, 3 dn, 4 da
    0x7f, 0x41, 0x41, 0x41, 0x3e, 0x51, 0x00, 0x14, // 68=D: 5 cols, 0 dn, 1 -, 2 -, 3 dn, 4 da
    0x7f, 0x49, 0x49, 0x41, 0x00, 0x44, 0x80, 0x05, // 69=E: 4 cols, 0 da, 1 m, 2~3 -
    0x7f, 0x09, 0x09, 0x01, 0x00, 0x44, 0x80, 0x05, // 70=F: 4 cols, 0 da, 1 m, 2~3 -
    0x3e, 0x41, 0x49, 0x49, 0x79, 0x54, 0x21, 0x12, // 71=G: 5 cols, 0 da, 1 dp, 2 dn, 3 dn, 4 dp
    0x7f, 0x08, 0x08, 0x08, 0x7f, 0x51, 0x00, 0x04, // 72=H: 5 cols, 0 dn, 1 -, 2 -, 3 -, 4 da
    0x41, 0x7f, 0x41, 0x00, 0x00, 0x30, 0x40, 0x01, // 73=I: 3 cols, 0 -, 1 da, 2 -
    0x81, 0x7f, 0x01, 0x00, 0x00, 0x31, 0x40, 0x01, // 74=J: 3 cols, 0 dn, 1 da, 2 -
    0x7f, 0x08, 0x14, 0x22, 0x41, 0x51, 0x11, 0x14, // 75=K: 5 cols, 0~3 dn, 4 da
    0x7f, 0x40, 0x40, 0x40, 0x00, 0x44, 0x80, 0x05, // 76=L: 4 cols, 0 da, 1 m, 2~3 -
    0x7f, 0x02, 0x1c, 0x02, 0x7f, 0x54, 0x00, 0x04, // 77=M: 5 cols, 0 da, 1~3 -, 4 da
    0x7f, 0x02, 0x04, 0x08, 0x7f, 0x51, 0x11, 0x14, // 78=N: 5 cols, 0~3 dn, 4 da
    0x3e, 0x41, 0x41, 0x41, 0x3e, 0x51, 0x00, 0x14, // 79=O: 5 cols, 0 dn, 1~2 -, 3 dn, 4 da
    0x7f, 0x11, 0x11, 0x11, 0x0e, 0x51, 0x00, 0x14, // 80=P: 5 cols, 0 dn, 1~2 -, 3 dn, 4 da
    0x3e, 0x41, 0x51, 0x61, 0x7e, 0x51, 0x00, 0x14, // 81=Q: 5 cols, 0 dn, 1~2 -, 3 dn, 4 da
    0x7f, 0x11, 0x11, 0x31, 0x4e, 0x51, 0x00, 0x14, // 82=R: 5 cols, 0 dn, 1~2 -, 3 dn, 4 da
    0x26, 0x49, 0x49, 0x49, 0x32, 0x51, 0x00, 0x14, // 83=S: 5 cols, 0 dn, 1~2 -, 3 dn, 4 da
    0x01, 0x01, 0x7f, 0x01, 0x01, 0x50, 0x04, 0x00, // 84=T: 5 cols, 0~1 -, 2 da, 3~4 -
    0x3f, 0x40, 0x40, 0x40, 0x3f, 0x51, 0x11, 0x14, // 85=U: 5 cols, 0~3 dn, 4 da
    0x1f, 0x20, 0x40, 0x20, 0x1f, 0x51, 0x11, 0x14, // 86=V: 5 cols, 0~3 dn, 4 da
    0x3f, 0x40, 0x3c, 0x40, 0x3f, 0x54, 0x20, 0x14, // 87=W: 5 cols, 0 da, 1 dp, 2 -, 3 dn, 4 da
    0x63, 0x14, 0x08, 0x14, 0x63, 0x51, 0x11, 0x14, // 88=X: 5 cols, 0~3 dn, 4 da
    0x07, 0x08, 0x70, 0x08, 0x07, 0x51, 0x11, 0x14, // 89=Y: 5 cols, 0~3 dn, 4 da
    0x61, 0x51, 0x49, 0x45, 0x43, 0x51, 0x11, 0x14, // 90=Z: 5 cols, 0~3 dn, 4 da
    0x7f, 0x41, 0x41, 0x00, 0x00, 0x34, 0x00, 0x01, // 91=[: 3 cols, 0 da, 1~2 -
    0x01, 0x06, 0x18, 0x60, 0x00, 0x41, 0x11, 0x41, // 92='\': 4 cols, 0~2 dn, 3 da
    0x41, 0x41, 0x7f, 0x00, 0x00, 0x30, 0x04, 0x01, // 93=]: 3 cols, 0~1 -, 2 da
    0x04, 0x02, 0x01, 0x02, 0x04, 0x51, 0x11, 0x14, // 94=^: 5 cols, 0~3 dn, 4 da
    0x40, 0x40, 0x40, 0x40, 0x00, 0x40, 0x08, 0x45, // 95=_: 4 cols, 0~1 -, 2 m, 3 da
    0x01, 0x02, 0x04, 0x00, 0x00, 0x31, 0x14, 0x01, // 96=`: 3 cols, 0~1 dn, 2 da
    0x20, 0x54, 0x54, 0x78, 0x00, 0x41, 0x81, 0x45, // 97=a: 4 cols, 0 dn, 1 m, 2 dn, 3 da
    0x7f, 0x44, 0x44, 0x38, 0x00, 0x44, 0x80, 0x25, // 98=b: 4 cols, 0 da, 1 m, 2 -, 3 dp
    0x38, 0x44, 0x44, 0x44, 0x00, 0x41, 0x80, 0x04, // 99=c: 4 cols, 0 dn, 1 m, 2 -, 3 -
    0x38, 0x44, 0x44, 0x7f, 0x00, 0x41, 0x80, 0x45, // 100=d: 4 cols, 0 dn, 1 m, 2 -, 3 da
    0x38, 0x54, 0x54, 0x58, 0x00, 0x41, 0x91, 0x45, // 101=e: 4 cols, 0 dn, 1 dn & m, 2 dn, 3 da
    0x08, 0x7e, 0x09, 0x00, 0x00, 0x38, 0x4a, 0x09, // 102=f: 3 cols, 0 m, 1 da, 2 dp & m
    0x18, 0xa4, 0xa4, 0x7c, 0x00, 0x41, 0x91, 0x45, // 103=g: 4 cols, 0 dn, 1 dn & m, 2 dn, 3 da
    0x7f, 0x04, 0x04, 0x78, 0x00, 0x44, 0x80, 0x25, // 104=h: 4 cols, 0 da, 1 m, 2 -, 3 dp
    0x7d, 0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x01, // 105=i: 1 cols, 0 da
    0x80, 0x7d, 0x00, 0x00, 0x00, 0x21, 0x40, 0x01, // 106=j: 2 cols, 0 dn, 1 da
    0x7f, 0x10, 0x28, 0x44, 0x00, 0x44, 0x92, 0x25, // 107=k: 4 cols, 0 da, 1 dn & m, 2 dp, 3 dp
    0x3f, 0x40, 0x00, 0x00, 0x00, 0x24, 0xa0, 0x05, // 108=l: 2 cols, 0 da, 1 dp & m
    0x7c, 0x04, 0x38, 0x04, 0x78, 0x54, 0x00, 0x14, // 109=m: 5 cols, 0 da, 1 - , 2 -, 3 dn, 4 da
    0x7c, 0x04, 0x04, 0x78, 0x00, 0x44, 0x80, 0x25, // 110=n: 4 cols, 0 da, 1 m, 2 -, 3 dp
    0x38, 0x44, 0x44, 0x38, 0x00, 0x41, 0x91, 0x45, // 111=o: 4 cols, 0 dn, 1 dn & m, 2 dn, 3 da
    0xfc, 0x24, 0x24, 0x18, 0x00, 0x41, 0x91, 0x45, // 112=p: 4 cols, 0 dn, 1 dn & m, 2 dn, 3 da
    0x18, 0x24, 0x24, 0xfc, 0x00, 0x41, 0x91, 0x45, // 113=q: 4 cols, 0 dn, 1 dn & m, 2 dn, 3 da
    0x7c, 0x04, 0x00, 0x00, 0x00, 0x24, 0x80, 0x05, // 114=r: 2 cols, 0 da, 1 m
    0x48, 0x54, 0x54, 0x24, 0x00, 0x41, 0x84, 0x25, // 115=s: 4 cols, 0 dn, 1 m, 2 da, 3 dp
    0x04, 0x3f, 0x44, 0x00, 0x00, 0x38, 0x4a, 0x09, // 116=t: 3 cols, 0 m, 1 da, 2 dp & m
    0x3c, 0x40, 0x40, 0x7c, 0x00, 0x41, 0x08, 0x45, // 117=u: 4 cols, 0 dn, 1 -, 2 m, 3 da
    0x1c, 0x20, 0x40, 0x20, 0x1c, 0x51, 0x11, 0x14, // 118=v: 5 cols, 0~3 dn, 4 da
    0x3c, 0x40, 0x38, 0x40, 0x7c, 0x54, 0x20, 0x04, // 119=w: 5 cols, 0 da, 1 dp, 2 -, 3 -, 4 da
    0x44, 0x28, 0x10, 0x28, 0x44, 0x51, 0x11, 0x14, // 120=x: 5 cols, 0~3 dn, 4 da
    0x9c, 0xa0, 0xa0, 0x7c, 0x00, 0x41, 0x81, 0x45, // 121=y: 4 cols, 0 dn, 1 m, 2 dn, 3 da
    0x44, 0x64, 0x54, 0x4c, 0x44, 0x50, 0x22, 0x20, // 122=z: 5 cols, 0 -, 1 dp, 2 dp, 3 dp, 4 -
    0x08, 0x36, 0x41, 0x00, 0x00, 0x31, 0x14, 0x01, // 123={: 3 cols, 0~1 dn, 2 da
    0x00, 0x7f, 0x00, 0x00, 0x00, 0x30, 0x40, 0x01, // 124=|: 3 cols, 0 -, 1 da, 2 -
    0x41, 0x36, 0x08, 0x00, 0x00, 0x31, 0x14, 0x01, // 125=}: 3 cols, 0~1 dn, 2 da
    0x08, 0x04, 0x08, 0x10, 0x08, 0x50, 0x11, 0x40, // 126=~: 5 cols, 0 -, 1 dn, 2 dn, 3 da, 4 -
    0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x50, 0x00, 0x00, // 127  : 5 cols, 0~4 -
};

#endif // MICROPY_INCLUDED_FONT_Z1FONTS_H
