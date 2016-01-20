/* 
 *  libTML:  A BEEP based Messaging Suite
 *  Copyright (C) 2015 wobe-systems GmbH
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  as published by the Free Software Foundation; either version 2.1
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this program; if not, write to the Free
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307 USA
 *  
 *  You may find a copy of the license under this software is released
 *  at COPYING file. This is LGPL software: you are welcome to develop
 *  proprietary applications using this library without any royalty or
 *  fee but returning back any change, improvement or addition in the
 *  form of source code, project image, documentation patches, etc.
 *
 *  Homepage:
 *    http://www.libtml.org
 *
 *  For professional support contact us:
 *
 *    wobe-systems GmbH
 *    support@libtml.org
 *
 * Contributors:
 *    wobe-systems GmbH
 */

#include <stdlib.h>
#include <string.h>
#include "tmlCoreHash.h"


// Constants are the integer part of the sines of integers (in radians) * 2^32.
const uint32_t k[64] = {
0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee ,
0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501 ,
0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be ,
0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821 ,
0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa ,
0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8 ,
0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed ,
0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a ,
0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c ,
0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70 ,
0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05 ,
0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665 ,
0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039 ,
0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1 ,
0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1 ,
0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 };

// leftrotate function definition
#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))

/** 
 * @brief calculate an md5 hash
 */
char* md5Hash::process(uint8_t *in_msg, size_t in_msg_len, TML_INT64* hRet, TML_INT64* lRet)
{
    uint32_t hashes[4];

    // prepare message
    uint8_t *msg = NULL;
    int new_len;
    uint32_t bits_len;
    int offset;
    uint32_t b, c, d, i, f, g, temp;

    // Note: All variables are unsigned 32 bit and wrap modulo 2^32 when calculating

    // r specifies the per-round shift amounts
    const uint32_t r[] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
                          5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
                          4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                          6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

    // Initialize variables - simple count in nibbles:
    hashes[0] = 0x67452301;
    hashes[1] = 0xefcdab89;
    hashes[2] = 0x98badcfe;
    hashes[3] = 0x10325476;

    for(new_len = in_msg_len*8 + 1; new_len%512!=448; new_len++);
    new_len /= 8;

    msg = (uint8_t*)calloc(new_len + 64, 1); 
    memcpy(msg, in_msg, in_msg_len);
    msg[in_msg_len] = 128; 

    bits_len = 8*in_msg_len; 
    memcpy(msg + new_len, &bits_len, 4);  

    // Process the message in successive 512-bit chunks:
    // for each 512-bit chunk of message:
    for(offset=0; offset<new_len; offset += (512/8)) {
        uint32_t a;
        uint32_t *w;

        // break chunk into sixteen 32-bit words w[j], 0 = j = 15
        w = (uint32_t *) (msg + offset);

		// Initialize hash value for this chunk:
        a = hashes[0];
        b = hashes[1];
        c = hashes[2];
        d = hashes[3];

        // Main loop:
        for(i = 0; i<64; i++) {

             if (i < 16) {
                f = (b & c) | ((~b) & d);
                g = i;
            } else if (i < 32) {
                f = (d & b) | ((~d) & c);
                g = (5*i + 1) % 16;
            } else if (i < 48) {
                f = b ^ c ^ d;
                g = (3*i + 5) % 16;          
            } else {
                f = c ^ (b | (~d));
                g = (7*i) % 16;
            }

            temp = d;
            d = c;
            c = b;
            b = b + LEFTROTATE((a + f + k[i] + w[g]), r[i]);
            a = temp;

        }

        // Add this chunk's hash to result so far:
        hashes[0] += a;
        hashes[1] += b;
        hashes[2] += c;
        hashes[3] += d;

    }

    // cleanup
    free(msg);

    char cMap[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

    uint8_t digits[16];
    for (int i = 0, ii = 0; i < 4; i++, ii += 4) {
      digits[ii]   = uint8_t(hashes[i] >> 24 & 0xFF);
      digits[ii+1] = uint8_t(hashes[i] >> 16 & 0xFF);
      digits[ii+2] = uint8_t(hashes[i] >> 8  & 0xFF);
      digits[ii+3] = uint8_t(hashes[i]       & 0xFF);
    }
    for(int i = 0, strIdx = 0 ; i < 16 ; i++)
    {
      m_hash[strIdx++] = cMap[digits[i] >> 4];
      m_hash[strIdx++] = cMap[digits[i] & 0xF];
    }
    m_hash[32] = '\0';  

    TML_INT64 hValue = 0;
    TML_INT64 lValue = 0;

    for(TML_UINT32 i = 0 ; i < 8 ; i++){
      hValue = hValue << 8; 
      hValue = hValue | digits[i]; 
      lValue = lValue << 8; 
      lValue = lValue | digits[i+8]; 
    }

    *hRet = hValue;
    *lRet = lValue;


    return m_hash;
};

/**
 * @brief constructor
 */
md5Hash::md5Hash(){
  	m_hash = new char[MD5_BLOCK_SIZE + 1]; // add one byte for a terminating \x00
};

/**
 * @brief destructor
 */
md5Hash::~md5Hash(){
  delete [] m_hash;
};