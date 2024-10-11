#ifndef ES_DECIMAL_H
#define ES_DECIMAL_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define ADD_SUB 1
#define MUL 2
#define DIV 3
#define MIN_INT -2147483648
#define MAX_INT 2147483647
#define SYSBITS_CORRECT_MASK (int)0b10000000111111110000000000000000
#define SYSBITS_REVERSE_MASK (int)0b01111111000000001111111111111111
#define SYSBITS_SCALE_TOZERO (int)0b11111111000000001111111111111111
#define FULL_ZERO (int)0b00000000000000000000000000000000
#define FULL_ONES (int)0b11111111111111111111111111111111

typedef struct {
  int bits[4];
} es_decimal;

typedef struct {
  int bits[6];
} BIG_decimal;

void clear_sys_bits(es_decimal *num);
int get_sign(es_decimal value);
void set_sign(int sign, es_decimal *value);
unsigned int get_scale(es_decimal value);
int set_scale(unsigned int scale, es_decimal *value);

void copy_bits_dec(es_decimal value, es_decimal *dst);
void copy_bits_big(BIG_decimal value, BIG_decimal *dst);
void copy_bits_dec_to_big(es_decimal value, BIG_decimal *dst);
void copy_bits_big_to_dec(BIG_decimal value, es_decimal *dst);

int big_is_zero(BIG_decimal num);
int dec_is_zero(es_decimal num);

int get_bit_big(BIG_decimal value, int num);
int get_bit_dec(es_decimal value, int num);
int left_shift_big(BIG_decimal *value);
int add_big(BIG_decimal value_1, BIG_decimal value_2, BIG_decimal *result);
int sub_big(BIG_decimal value_1, BIG_decimal value_2, BIG_decimal *result);
int mul_big(BIG_decimal summer, BIG_decimal adder, BIG_decimal *result);
int mul_big_to_ten(BIG_decimal *value);
int div_big_once(BIG_decimal value_1, BIG_decimal value_2, BIG_decimal *result,
                 BIG_decimal *fraction);
int div_big(BIG_decimal value_1, BIG_decimal value_2, BIG_decimal *result,
            unsigned int *scale);
int div_big_to_ten(BIG_decimal *value, int *fraction);

int big_bits_is_less(BIG_decimal one, BIG_decimal two);
int big_bits_is_less_or_equal(BIG_decimal one, BIG_decimal two);
int big_bits_is_greater(BIG_decimal one, BIG_decimal two);
int big_bits_is_greater_or_equal(BIG_decimal one, BIG_decimal two);
int big_bits_is_equal(BIG_decimal one, BIG_decimal two);

int two_dec_to_big(es_decimal value_1, es_decimal value_2, BIG_decimal *left,
                   BIG_decimal *right, unsigned int *scale, int type);
int big_to_dec(BIG_decimal value, es_decimal *result, unsigned int scale,
               int res_sign, int delete_zero);
int bank_rounding(BIG_decimal *value, int last_num, int last_num_count);
void zero_deleting(BIG_decimal *value, unsigned int *scale);
int is_all_zero_mul(es_decimal value_1, es_decimal value_2);
double my_pow_of_two(int count);
void clear_result(es_decimal *result);

int es_add(es_decimal value_1, es_decimal value_2, es_decimal *result);
int es_sub(es_decimal value_1, es_decimal value_2, es_decimal *result);
int es_mul(es_decimal value_1, es_decimal value_2, es_decimal *result);
int es_div(es_decimal value_1, es_decimal value_2, es_decimal *result);

int es_is_less(es_decimal one, es_decimal two);
int es_is_less_or_equal(es_decimal one, es_decimal two);
int es_is_greater(es_decimal one, es_decimal two);
int es_is_greater_or_equal(es_decimal one, es_decimal two);
int es_is_equal(es_decimal one, es_decimal two);
int es_is_not_equal(es_decimal one, es_decimal two);

int es_from_int_to_decimal(int src, es_decimal *dst);
int es_from_float_to_decimal(float src, es_decimal *dst);
int es_from_decimal_to_int(es_decimal src, int *dst);
int es_from_decimal_to_float(es_decimal src, float *dst);

int es_floor(es_decimal value, es_decimal *result);
int es_round(es_decimal value, es_decimal *result);
int es_truncate(es_decimal value, es_decimal *result);
int es_negate(es_decimal value, es_decimal *result);

#endif  // ES_STRING_H
