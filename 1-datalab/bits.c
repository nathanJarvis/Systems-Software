/* 
 * CS:APP Data Lab 
 * 
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* Copyright (C) 1991-2012 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* We do support the IEC 559 math functionality, real and complex.  */
/* wchar_t uses ISO/IEC 10646 (2nd ed., published 2011-03-15) /
   Unicode 6.0.  */
/* We do not support C11 <threads.h>.  */
/* 
 * bitAnd - x&y using only ~ and | 
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
  	/*Have the arguments negated then perform an inclusive or so that
 * only the naughted pairs of 1's  are preserved. inverst this result to get the AND */
	int nx = ~x;
	int ny = ~y;
	int result = nx | ny;
	return ~result;
}
/* 
 * leastBitPos - return a mask that marks the position of the
 *               least significant 1 bit. If x == 0, return 0
 *   Example: leastBitPos(96) = 0x20
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2 
 */
int leastBitPos(int x) {
  /*have the 2 complemet of x,
 * with this compelement the first bit to be 1 will remain a 1 while 
 * the others will be flipped.
 * AND this with the original X and you will only get the first bit to be one
 * in the mask
 */

    int cx = ~x+1;
    int result =  cx & x;
    return result;
}
/* 
 * replaceByte(x,n,c) - Replace byte n in x with c
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: replaceByte(0x12345678,1,0xab) = 0x1234ab78
 *   You can assume 0 <= n <= 3 and 0 <= c <= 255
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 3
 */
int replaceByte(int x, int n, int c) {
 /* Have a mask of ones that erase the data stored in 
 * byte n and set to all 0's in that byte
 * Then have c shifted to nth byte and inclusive or operation
 *
 */
   int mask = 0XFF;
   int shiftConst = n << 3;
   mask = mask << shiftConst;
   mask = ~mask;
   x = x & mask;
   c = c << shiftConst;
   return c|x;
}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
  /* Have x evaluated as a logical operator using !
 * so that it becomes 0 or 1 based on its original value
 *
 * take the complement of the logical result so that
 * x becomes all 0's or all 1's. use this as a mask
 * to implement the conditonal
 */
   int mask = !!x;
   int cMask = ~mask+1;
   return (cMask & y) | (~cMask & z);
 
}
/*
 * bitParity - returns 1 if x contains an odd number of 0's
 *   Examples: bitParity(5) = 0, bitParity(7) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int bitParity(int x) {
   /*have to compare all bits to one another using xor
 * the bit shifting compares x as it folds in on itself using xor
 * so that the LSB is a comparison of all given bits xor'd with one another
 *
 * if there was an even number of 1's then there is an even number of 0's
 * and so the LSB should be 0. The converse is true as well
 *
 * you then check the LSB against 1 as a mask
 */
   int compare = x >> 16;
   x = compare ^ x;
   compare = x >> 8;
   x = compare ^ x;
   compare = x >> 4;
   x = compare ^ x;
   compare = x >> 2;
   x = compare ^ x;
   compare = x >> 1;
   x = compare ^ x;
   compare = 1;
   return x & compare;
}
/* 
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *    Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
/*Check that x shifted n bits does not overflow
 * overflow is detected by having the original x equal to itself
 */
	int org_X = x;
	int length = 32;
	length = length + (~n + 1);
	x = x << length;
	x = x >> length;
	return !(org_X ^ x);

  

}
/* 
 * isPositive - return 1 if x > 0, return 0 otherwise 
 *   Example: isPositive(-1) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 3
 */
int isPositive(int x) { 
  /*check if MSB is 1 and if it is return 0 
 * have edge case for 0 where need to verify if x=0
 *
 */
   int cpy_X = x;  
   x = !x; 
   cpy_X  = cpy_X >> 31;
   return !(cpy_X | x); 
}
/* 
 * isLess - if x < y  then return 1, else return 0 
 *   Example: isLess(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLess(int x, int y) {
/*find difference between x and y
 * check if positive or negative result
 * 
 * only compare MSB for result
 * account edge cases 
*/
   int neg_Y = ~y + 1; 
   int diff = x + neg_Y;
  //check signs using xor MSB
   int same_sign = ~(x ^ y);
  //x neg and y positive w/ MSB set to 1
   int case1 = (x & ~y);
  //result has same sign and diff is negative
   int case2 = same_sign & diff;
   return !!( (case1 | case2) >> 31 );

}
/*
 * satMul3 - multiplies by 3, saturating to Tmin or Tmax if overflow
 *  Examples: satMul3(0x10000000) = 0x30000000
 *            satMul3(0x30000000) = 0x7FFFFFFF (Saturate to TMax)
 *            satMul3(0x70000000) = 0x7FFFFFFF (Saturate to TMax)
 *            satMul3(0xD0000000) = 0x80000000 (Saturate to TMin)
 *            satMul3(0xA0000000) = 0x80000000 (Saturate to TMin)
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 3
 */
int satMul3(int x) {
   /*check for overflow by checking the MSB changing during operations
 */
    int xMSB = (x >> 31);
    int double_x = x << 1;
    int double_xMSB = (double_x >> 31);
    int thrice_x = double_x + x;
    int thrice_xMSB = (thrice_x >> 31);
    int comp1 = xMSB ^ double_xMSB;
    int comp2 = double_xMSB ^ thrice_xMSB;
    int overFlow = (comp1 | comp2);
   //find correct saturation floor/ceiling  according to the xMSB
    int sat = ~xMSB ^ (1 << 31);
  
    return  (~overFlow & thrice_x) | (overFlow & sat);  
    
}
/* 
 * absVal - absolute value of x
 *   Example: absVal(-1) = 1.
 *   You may assume -TMax <= x <= TMax
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */
int absVal(int x) {
  //if x is negative then mask is -1 and take compelement of x
  //if x is positive then mask is 0 so the inverted is -1 and return x
   int sign_mask = (x >> 31); 
   return (~sign_mask & x) | (sign_mask &  (~x + 1)); 
}
/* 
 * float_neg - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_neg(unsigned uf) {
  /*If exponent is all 1's and mantissa is non zero
 * then NaN
 *
 * Otherwise flip the sign bit
 */
   int exp_mask = 0x7F800000; 
   int comp_exp = !((uf & exp_mask) ^ exp_mask);
   int comp_man = uf &  0x7FFFFF;
   if(comp_man && comp_exp){
      return uf;
   }
 
   return uf ^ (1 << 31);
}
/* 
 * float_half - Return bit-level equivalent of expression 0.5*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_half(unsigned uf) {
/* Calculate the rounded bits as 1 or 0 based on bottom three bits
 *
 * Check if Nan or 0
 *
 *if the exponent is 1 then need to go from normalized to denormalized
 * reduce the expoenent to 0 and add 1 to MSB shifted mantissa
 *
 *if denormalized then just shift mantissa
 *
 * if normalized then just shift the exponent
 */
   int temp = 0;
   int sign = uf & 0x80000000;
   int exponent = uf & 0x7F800000;
   int mantissa = uf & 0x7fffff;
   //value to add to mantissa for rounding in denormalized
   //if bottom bits are 0x03 then add 1 to mantissa
   int rounded_bits = ((uf & 0x03) == 0x03); 
 
 
  // NaN/infinity  or  ±0
   if((exponent == 0x7F800000) || !(exponent | mantissa)){
	//do nothing
   }
   //exponent = 1, normal -> denormal
   else if(exponent == 0x800000){
   
      exponent = 0x00;
      temp = mantissa >> 1; 
    
      mantissa = temp | 0x400000;

      mantissa = mantissa + rounded_bits;
    

   }
   //already denormalized
   else if(exponent == 0x00){
      mantissa >>= 1;
      mantissa += rounded_bits;
      

   }

   //normalized
   else{
      exponent = (exponent >> 23) - 1;
      exponent = exponent << 23;
   }

   return sign | exponent | mantissa;


}
