/**
 * MIT License
 * Copyright (c) 2020 Mitosis-Project
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

///< this is the cacheline size
#define CONFIG_CACHELINE_SIZE 64

#define CONFIG_LARGE_PAGE_SIZE (1 << 21)

///< define this if you have a modified kernel with Mitosis support
#define CONFIG_HAVE_MODIFIED_KERNEL 1

///< the name of the shared memory file created
#define CONFIG_SHM_FILE_NAME "/tmp/securefs-hashjoin-bench"

///< the random seed
#define CONFIG_RAND_SEED 42

///< the size of the outer table in elements
#define CONFIG_DEFAULT_OUTER_SIZE 100000

///< the size of the inner table in elements
#define CONFIG_DEFAULT_INNER_SIZE 100000

///< the size of the hash table
#define CONFIG_DEFAULT_HASH_SIZE CONFIG_DEFAULT_INNER_SIZE

///< the default amout of lookups
#define CONFIG_DEFAULT_NUM_LOOKUPS 50

///< the key stride for the inner table
#define CONFIG_INNER_KEY_STRIDE 4


///< the size of the tuple in quad words
#define CONFIG_ELEMENT_TUPLE_SIZE 15
