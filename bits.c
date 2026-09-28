/* WARNING: Do not include any other libraries here,
 * otherwise you will get an error while running test.py
 * You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.
 *
 * Using printf will interfere with our script capturing the execution results.
 * At this point, you can only test correctness with ./btest.
 * After confirming everything is correct in ./btest, remove the printf
 * and run the complete tests with test.py.
 */

 /*
 * bitAnd - x & y using only ~ and |
 * Example: bitAnd(4, 5) = 4
 * Legal ops: ~ |
 * Max ops: 7
 * Difficulty: 1
 */
int bitAnd(int x, int y) {
    return ~(~x | ~y);
}

// 这里我们用德摩根公式来实现&，a & b = ~ (~ a | ~ b)

/*
 * bitXor - x ^ y using only ~ and &
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 7
 *   Difficulty: 1
 */
int bitXor(int x, int y) {
    return (~(x & y)) & (~(~x & ~ y));
}

// 这里是要我们实现异或操作，二者相同时返回0，不同时返回1。字面意思的异或就是要返回1，就是两个至少一个1，但不能同时是1。可以拆成 (x|y) & ~(x&y)。可以保证两个相同时至少一个是零。接着用德摩根公式进行转换。

/*
 * samesign - Determines if two integers have the same sign.
 *   0 is not positive, nor negative
 *   Example: samesign(0, 1) = 0, samesign(0, 0) = 1
 *            samesign(-4, -5) = 1, samesign(-4, 5) = 0
 *   Legal ops: >> << ! ^ && if else &
 *   Max ops: 12
 *   Difficulty: 2
 *
 * Parameters:
 *   x - The first integer.
 *   y - The second integer.
 *
 * Returns:
 *   1 if x and y have the same sign , 0 otherwise.
 */
int samesign(int x, int y) {
    if (!x & !y){
        return 1;
    }
    else if (!y){
        return 0;
    }
    else if (!x){
        return 0;
    }
    else{
        return !((x>>31) ^ (y>>31));
    }
}

// 这里重要的是对零的单独考虑。两个都为零当然返回1，但是其中只有一个零返回0。其余可以看最高位是零还是1就可以判断。

/*
 * logtwo - Calculate the base-2 logarithm of a positive integer using bit
 *   shifting. (Think about bitCount)
 *   Note: You may assume that v > 0
 *   Example: logtwo(32) = 5
 *   Legal ops: > < >> << |
 *   Max ops: 25
 *   Difficulty: 4
 */
int logtwo(int v) {
    int res, step;
    
// 就是这里我们相当于是用二分法，用16，8，4，2，1依次进行划分，最终确定最高位。

    res = (v > 0xFFFF) << 4; // 如果成立，代表最高位至少16位，再删去后面16位，看看剩下来的。后面也是以此类推。
    v >>= res;

    step = (v > 0xFF) << 3;
    v >>= step;
    res |= step;

    step = (v > 0xF) << 2 ;
    v >>= step;
    res |=step;

    step = (v> 0x3) << 1;
    v >>=step;
    res |=step;

    res |= (v>1);
    return res;
}

/*
 *  byteSwap - swaps the nth byte and the mth byte
 *    Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *              byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *    Note: You may assume that 0 <= n <= 3, 0 <= m <= 3
 *    Legal ops: ! ~ & ^ | + << >>
 *    Max ops: 17
 *    Difficulty: 2
 */
int byteSwap(int x, int n, int m) {
// 利用一些异或的性质，就是 a^(a^b)=b,这样就能对a和b实现翻转的操作。交换就是逐位实现翻转的操作。所以我们的关键是确定a
    int shiftn = (n << 3) ;
    int shiftm = (m << 3) ;
    int byten = (x >> shiftn) & 0xFF ; //这里我们保留最低的八位，也就是我们需要交换的部分
    int bytem = (x >> shiftm) & 0xFF ;
    int diff = byten ^ bytem; //到这里我们就求解出了相应的a,接下来就是异或的操作
    int res = x^(diff << shiftn) ^ (diff << shiftm);
    return res; 
}

/*
 * reverse - Reverse the bit order of a 32-bit unsigned integer.
 *   Example: reverse(0xFFFF0000) = 0x0000FFFF reverse(0x80000000)=0x1 reverse(0xA0000000)=0x5
 *   Note: You may assume that an unsigned integer is 32 bits long.
 *   Legal ops: << | & - + >> for while ! ~ (You can define unsigned in this function)
 *   Max ops: 30
 *   Difficulty: 3
 */
// 我感觉这道题还是交换，但这里我们类似于使用二分法的交换。就是说我们尝试前16位后16位交换，然后在16位里面前八位和后八位进行交换，这样依次进行。
// 关键在于掩码的构建，如何理解和构建掩码。分别保留要交换二者的其中一个，另一个值为零然后再用或进行合并的操作。
unsigned reverse(unsigned v) {
    // 先11进行交换，0101，就相当于是5
    v = ((v >> 1) & 0x55555555) | ((v & 0x55555555) << 1);
    // 然后再进行二二操作，0011 0011 就相当于是3
    v = ((v >> 2) & 0x33333333) | ((v & 0x33333333) << 2);
    // 然后再进行44操作 0000 1111 就相当于0f
    v = ((v >> 4) & 0x0F0F0F0F) | ((v & 0x0F0F0F0F) << 4);
    // 然后88，00ff这样
    v = ((v >> 8) & 0x00FF00FF) | ((v & 0x00FF00FF) << 8);

    return (v >> 16) | (v << 16);
}

/*
 * logicalShift - shift x to the right by n, using a logical shift
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Note: You can assume that 0 <= n <= 31
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Difficulty: 3
 */

// 右移然后补零。这里就是可能最高位是1可能补一，所以最好拿掩码修正。但是这里有一个问题，怎么构建掩码让他前某几位是零而不会出现普通右移会出现的错误。
// 筛子怎么做呢，n=0要怎么处理呢。
int logicalShift(int x, int n) {
    int nonzero = !!n;
    int shift = n + ~nonzero + 1;
    int lowmask = 0x7FFFFFFF >> shift;
    int iszero = !n;
    int zeroMask = ~iszero+1; // 如果是零我们就构造一个全是1的序列，如果不是，最高位之下and操作其实也不影响。但是我们这里不能直接构造0xFFFFFFFF
    int mask = lowmask | ((~0x7FFFFFFF) & zeroMask);

    return (x>>n)&mask;
}

/*
 * leftBitCount - returns count of number of consective 1's in left-hand (most) end of word.
 *   Examples: leftBitCount(-1) = 32, leftBitCount(0xFFF0F0F0) = 12,
 *             leftBitCount(0xFE00FF0F) = 7
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 50
 *   Difficulty: 4
 */
// 其实类似于前面二分法求最高的位，我们也是前面16有没有连续的1，前面八位，4位，2位，1位
int leftBitCount(int x) {
    int count = 0;

    count += (!(~(x>>16))) << 4;
    count += (!(~(x >> (24 + (~count + 1))))) << 3; // 如果count为零，前16位就不满足，那还得移动8位就24位了。但是如果不是，那就是16，那就只用再移动八位。32-（已经确定的位数+还需要确定的位数）
    count +=(!(~(x >> (28 + (~count + 1))))) << 2;
    count +=(!(~(x >> (30 + (~ count +1))))) << 1;
    count += !(~(x >> (31 + (~count + 1))));

    return count + ! (~x);

}

/*
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but it is to be interpreted as
 *   the bit-level representation of a single-precision floating point values.
 *   Legal ops: if else while for & | ~ + - >> << < > ! ==
 *   Max ops: 30
 *   Difficulty: 4
 */

// 这里要我们转化为浮点数，关键就是得到前面符号位、指数位以及尾数位。
unsigned float_i2f(int x) {
    unsigned sign = 0; // 判断需要处理的书的正负
    unsigned ax;
    unsigned exp;
    unsigned frac;
    unsigned rest;
    unsigned half;
    unsigned t;
    int e = 0;
    int k;

    if (x == 0)
        return 0; // 零浮点数表示其实也是零

    if (x < 0) {
        sign = 1;
        sign = sign << 31;
        ax = -(x + 1); // 不能直接用-x，因为二者表示的数的范围是不相同的。
        ax = ax + 1;
    } else {
        ax = x;
    }
// 转化为我们要求解的绝对值数
    t = ax;
    while (t >> 1) {
        t = t >> 1;
        e = e + 1;
    }

    exp = (e + 127) << 23; // 浮点数右边三位是尾数位

    if (e <= 23) {
        frac = (ax << (23 - e)) & 0x7FFFFF;// 23位几乎全是1
    } else {
        k = e - 23;
        frac = (ax >> k) & 0x7FFFFF;
        rest = ax & ((1 << k) - 1);
        half = 1 << (k - 1);

        if (rest > half)
            frac = frac + 1;

        if (rest == half)
            if (frac & 1)
                frac = frac + 1;
    }

    return sign | (exp + frac);// 最后这些位数相结合
}

/*
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: & >> << | if > < >= <= ! ~ else + ==
 *   Max ops: 30
 *   Difficulty: 4
 */

// 乘以二本来是说指数加一，但是我们这里要对不同情况进行考虑。
unsigned floatScale2(unsigned uf) {
    unsigned sign = uf & 0x80000000;
    unsigned exp = (uf >> 23) & 0xFF;
    unsigned frac = uf & 0x7FFFFF;
// 用掩码取出各个位数
    if (exp == 0xFF)
    return uf;

    if (exp == 0)
    return sign | (frac << 1);

    exp = exp + 1;

    if (exp == 0xFF)
    frac = 0;

    return sign | (exp <<23) | frac;

    // 就是分情况处理不同的数字
}

/*
 * float64_f2i - Convert a 64-bit IEEE 754 floating-point number to a 32-bit signed integer. // 双精度浮点数变为整数
 *   The conversion rounds towards zero.
 *   Note: Assumes IEEE 754 representation and standard two's complement integer format.
 *   Parameters:
 *     uf1 - The lower 32 bits of the 64-bit floating-point number.
 *     uf2 - The higher 32 bits of the 64-bit floating-point number.
 *   Returns:
 *     The converted integer value, or 0x80000000 on overflow, or 0 on underflow.
 *   Legal ops: >> << | & ~ ! + - > < >= <= if else
 *   Max ops: 60
 *   Difficulty: 3
 */
int float64_f2i(unsigned uf1, unsigned uf2) { //双精度，有高三十二位以及低三十二位
    unsigned sign = uf2 >> 31; //先判断正负
    int exp = (uf2 >> 20) & 0x7FF;
    int e = exp - 1023;
    unsigned top = (uf2 & 0xFFFFF) | (1 << 20);
    unsigned magnitude;
    int result;

    if (e < 0)
        return 0;

    if (e >= 31)
        return -2147483647 - 1;

    if (e <= 20)
        magnitude = top >> (20 - e);
    else
        magnitude = (top << (e - 20)) | (uf1 >> (52 - e));

    result = magnitude;

    if (sign)
        return -result;

    return result;
}

/*
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 *
 *   Legal ops: < > <= >= << >> + - & | ~ ! if else &&
 *   Max ops: 30
 *   Difficulty: 4
 */
unsigned floatPower2(int x) {
    if (x < -149)
        return 0;

    if (x < -126)
        return 1 << (x + 149); //这个1要像左移动的距离。

    if (x > 127)
        return 0xFF << 23;

    return (x + 127) << 23;
}
