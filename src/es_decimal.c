#include "es_decimal.h"

void clear_sys_bits(es_decimal *num) { num->bits[3] &= SYSBITS_CORRECT_MASK; }

int get_sign(es_decimal value) { return value.bits[3] >= 0 ? 1 : -1; }

void set_sign(int sign, es_decimal *value) {
  if (sign == 1) {
    value->bits[3] &= ~(1 << 31);
  } else if (sign == -1) {
    value->bits[3] |= (1 << 31);
  }
}

unsigned int get_scale(es_decimal value) {
  unsigned int scale = value.bits[3] << 8;
  scale >>= 24;

  return scale;
}

int set_scale(unsigned int scale, es_decimal *value) {
  int res = 0;

  if (scale > 28) {
    res = 1;
  } else {
    value->bits[3] &= SYSBITS_SCALE_TOZERO;
    value->bits[3] |= (scale << 16);
  }

  return res;
}

void copy_bits_dec(es_decimal value, es_decimal *dst) {
  for (int i = 0; i <= 3; i++) {
    dst->bits[i] = value.bits[i];
  }
}

void copy_bits_big(BIG_decimal value, BIG_decimal *dst) {
  for (int i = 0; i <= 5; i++) {
    dst->bits[i] = value.bits[i];
  }
}

void copy_bits_dec_to_big(es_decimal value, BIG_decimal *dst) {
  for (int i = 0; i <= 2; i++) {
    dst->bits[i] = value.bits[i];
  }
}

void copy_bits_big_to_dec(BIG_decimal value, es_decimal *dst) {
  for (int i = 0; i <= 2; i++) {
    dst->bits[i] = value.bits[i];
  }
}

int big_is_zero(BIG_decimal num) {
  int res = 1;

  for (int i = 0; i <= 5; i++) {
    if (num.bits[i] != 0) res = 0;
  }

  return res;
}

int dec_is_zero(es_decimal num) {
  int res = 1;

  for (int i = 0; i <= 2; i++) {
    if (num.bits[i] != 0) res = 0;
  }

  return res;
}

int add_big(BIG_decimal summer, BIG_decimal adder, BIG_decimal *result) {
  int res = 0;
  int external_overflow = 0;
  BIG_decimal overflow = {{0, 0, 0, 0, 0, 0}};

  while (!big_is_zero(adder) && !res) {
    for (int i = 0; i <= 5; i++) {
      overflow.bits[i] = summer.bits[i] & adder.bits[i];
      summer.bits[i] = summer.bits[i] ^ adder.bits[i];
      adder.bits[i] = (overflow.bits[i] << 1);
      if (external_overflow) {
        adder.bits[i] |= 1;
        external_overflow = 0;
      }
      if (overflow.bits[i] < 0 && i != 5) {
        external_overflow = 1;
      }
      if (overflow.bits[i] < 0 && i == 5) {
        res = 1;
      }
    }
    external_overflow = 0;
  }

  if (!res) copy_bits_big(summer, result);

  return res;
}

int sub_big(BIG_decimal summer, BIG_decimal adder, BIG_decimal *result) {
  int res = 0;
  int external_overflow = 0;
  BIG_decimal overflow = {{0, 0, 0, 0, 0, 0}};

  while (!big_is_zero(adder) && !res) {
    for (int i = 0; i <= 5; i++) {
      overflow.bits[i] = (~summer.bits[i]) & adder.bits[i];
      summer.bits[i] = summer.bits[i] ^ adder.bits[i];
      adder.bits[i] = (overflow.bits[i] << 1);
      if (external_overflow) {
        adder.bits[i] |= 1;
        external_overflow = 0;
      }
      if (overflow.bits[i] < 0 && i != 5) {
        external_overflow = 1;
      }
      if (overflow.bits[i] < 0 && i == 5) {
        res = 1;
      }
    }
    external_overflow = 0;
  }

  if (!res) copy_bits_big(summer, result);

  return res;
}

int mul_big(BIG_decimal summer, BIG_decimal adder, BIG_decimal *result) {
  int res = 0;
  BIG_decimal sum = {{0, 0, 0, 0, 0, 0}};
  BIG_decimal copy = {{0, 0, 0, 0, 0, 0}};

  for (int i = 0; i <= 191 && !res; i++) {
    if (get_bit_big(adder, i)) {
      copy = summer;
      for (int j = 0; j < i && !res; j++) res = left_shift_big(&copy);
      res = add_big(sum, copy, &sum);
    }
  }

  if (!res) copy_bits_big(sum, result);

  return res;
}

int get_bit_big(BIG_decimal value, int num) {
  int res = 0;

  if (num < 0 || num > 191) {
    res = -1;
  } else {
    int int_num = num / 32;
    res = value.bits[int_num] >> (num - 32 * int_num) & 1u;
  }

  return res;
}

int get_bit_dec(es_decimal value, int num) {
  int res = 0;

  if (num < 0 || num > 95) {
    res = -1;
  } else {
    int int_num = num / 32;
    res = value.bits[int_num] >> (num - 32 * int_num) & 1u;
  }

  return res;
}

int left_shift_big(BIG_decimal *value) {
  int external_overflow = 0;
  int res = 0;
  BIG_decimal copy = {{0, 0, 0, 0, 0, 0}};

  copy_bits_big(*value, &copy);

  for (int j = 0; j <= 5; j++) {
    if (external_overflow) {
      external_overflow = 0;
      if (copy.bits[j] < 0 && j != 5) {
        external_overflow = 1;
      }
      if (copy.bits[j] < 0 && j == 5) {
        res = 1;
      }
      copy.bits[j] <<= 1;
      copy.bits[j] |= 1;
    } else {
      if (copy.bits[j] < 0 && j != 5) {
        external_overflow = 1;
      }
      if (copy.bits[j] < 0 && j == 5) {
        res = 1;
      }
      copy.bits[j] <<= 1;
    }
  }
  external_overflow = 0;

  if (!res) copy_bits_big(copy, value);

  return res;
}

int mul_big_to_ten(BIG_decimal *value) {
  int res = 0;
  BIG_decimal eight = {{0, 0, 0, 0, 0, 0}};
  BIG_decimal two = {{0, 0, 0, 0, 0, 0}};

  copy_bits_big(*value, &eight);
  copy_bits_big(*value, &two);
  for (int i = 1; i <= 3 && !res; i++) {
    res += left_shift_big(&eight);
  }

  if (!res) res += left_shift_big(&two);

  if (!res) res += add_big(eight, two, &two);

  if (!res) copy_bits_big(two, value);

  return res > 0 ? 1 : 0;
}

int div_big_to_ten(BIG_decimal *value, int *fraction) {
  int counter = 0;
  int err = 0;
  int err_great = 0;
  BIG_decimal ten_orig = {{10, 0, 0, 0, 0, 0}};
  BIG_decimal ten = ten_orig;
  BIG_decimal ten_copy = ten;
  BIG_decimal div = *value;
  BIG_decimal res = {{0, 0, 0, 0, 0, 0}};

  while (big_bits_is_greater_or_equal(div, ten_orig) && !err) {
    BIG_decimal step = {{1, 0, 0, 0, 0, 0}};

    while (big_bits_is_less(ten_copy, div) && !err_great) {
      ten = ten_copy;
      err_great = left_shift_big(&ten_copy);
      counter++;
    }

    if (big_bits_is_greater(ten_copy, div) || err_great) {
      counter--;
      err += sub_big(div, ten, &div);
    } else {
      err += sub_big(div, ten_copy, &div);
    }

    for (int i = 0; i < counter; i++) err += left_shift_big(&step);
    err += add_big(res, step, &res);

    counter = 0;
    err_great = 0;
    ten = ten_orig;
    ten_copy = ten;
  }

  if (!err) {
    *value = res;
    if (fraction != NULL) *fraction = div.bits[0];
  }

  return err;
}

int div_big(BIG_decimal value_1, BIG_decimal value_2, BIG_decimal *result,
            unsigned int *scale) {
  int err = 0;
  BIG_decimal res = {{0, 0, 0, 0, 0, 0}};
  BIG_decimal div = value_1;
  BIG_decimal fraction = {{0, 0, 0, 0, 0, 0}};
  BIG_decimal final = {{0, 0, 0, 0, 0, 0}};

  do {
    div_big_once(div, value_2, &res, &fraction);
    add_big(final, res, &final);
    if (!big_is_zero(fraction)) {
      err += mul_big_to_ten(&final);
      if (!err) {
        mul_big_to_ten(&fraction);
        div = fraction;
        *scale = *scale + 1;
      }
    }
  } while (!big_is_zero(fraction) && !err && !final.bits[4]);

  *result = final;

  return err;
}

int div_big_once(BIG_decimal value_1, BIG_decimal value_2, BIG_decimal *result,
                 BIG_decimal *fraction) {
  int counter = 0;
  int err = 0;
  int err_great = 0;
  BIG_decimal diver = value_2;
  BIG_decimal copy_diver = diver;
  BIG_decimal res = {{0, 0, 0, 0, 0, 0}};

  while (big_bits_is_greater_or_equal(value_1, diver) && !err) {
    BIG_decimal step = {{1, 0, 0, 0, 0, 0}};

    while (big_bits_is_less(copy_diver, value_1) && !err_great) {
      diver = copy_diver;
      err_great = left_shift_big(&copy_diver);
      counter++;
    }

    if (big_bits_is_greater(copy_diver, value_1) || err_great) {
      counter--;
      err += sub_big(value_1, diver, &value_1);
    } else {
      err += sub_big(value_1, copy_diver, &value_1);
    }

    for (int i = 0; i < counter; i++) err += left_shift_big(&step);
    err += add_big(res, step, &res);

    counter = 0;
    err_great = 0;
    diver = value_2;
    copy_diver = diver;
  }

  if (!err) {
    *result = res;
    if (fraction != NULL) *fraction = value_1;
  }

  return err;
}

int big_bits_is_less_or_equal(BIG_decimal one, BIG_decimal two) {
  int res = 1;
  unsigned int one_bit = 0;
  unsigned int two_bit = 0;

  for (int i = 5; i >= 0 && res == 1; i--) {
    for (int j = 0; j <= 31 && res == 1; j++) {
      one_bit = (one.bits[i] << j) >> 31;
      two_bit = (two.bits[i] << j) >> 31;
      if (one_bit < two_bit) {
        res = 2;
      } else if (one_bit > two_bit) {
        res = 0;
      }
    }
  }

  if (res == 2) res = 1;

  return res;
}

int big_bits_is_less(BIG_decimal one, BIG_decimal two) {
  int res = 0;
  unsigned int one_bit = 0;
  unsigned int two_bit = 0;

  for (int i = 5; i >= 0 && res == 0; i--) {
    for (int j = 0; j <= 31 && res == 0; j++) {
      one_bit = (one.bits[i] << j) >> 31;
      two_bit = (two.bits[i] << j) >> 31;
      if (one_bit < two_bit) {
        res = 1;
      } else if (one_bit > two_bit) {
        res = 2;
      }
    }
  }

  if (res == 2) res = 0;

  return res;
}

int big_bits_is_greater_or_equal(BIG_decimal one, BIG_decimal two) {
  int res = 1;
  unsigned int one_bit = 0;
  unsigned int two_bit = 0;

  for (int i = 5; i >= 0 && res == 1; i--) {
    for (int j = 0; j <= 31 && res == 1; j++) {
      one_bit = (one.bits[i] << j) >> 31;
      two_bit = (two.bits[i] << j) >> 31;
      if (one_bit > two_bit) {
        res = 2;
      } else if (one_bit < two_bit) {
        res = 0;
      }
    }
  }

  if (res == 2) res = 1;

  return res;
}

int big_bits_is_greater(BIG_decimal one, BIG_decimal two) {
  int res = 0;
  unsigned int one_bit = 0;
  unsigned int two_bit = 0;

  for (int i = 5; i >= 0 && res == 0; i--) {
    for (int j = 0; j <= 31 && res == 0; j++) {
      one_bit = (one.bits[i] << j) >> 31;
      two_bit = (two.bits[i] << j) >> 31;
      if (one_bit > two_bit) {
        res = 1;
      } else if (one_bit < two_bit) {
        res = 2;
      }
    }
  }

  if (res == 2) res = 0;

  return res;
}

int big_bits_is_equal(BIG_decimal one, BIG_decimal two) {
  int res = 1;

  for (int i = 0; i <= 191 && res; i++) {
    if (get_bit_big(one, i) != get_bit_big(two, i)) res = 0;
  }

  return res;
}

int two_dec_to_big(es_decimal value_1, es_decimal value_2, BIG_decimal *left,
                   BIG_decimal *right, unsigned int *scale, int type) {
  int res = 0;
  unsigned int scale_1 = get_scale(value_1);
  unsigned int scale_2 = get_scale(value_2);
  unsigned int max_scale = (scale_1 >= scale_2) ? scale_1 : scale_2;
  unsigned int min_scale = (scale_1 <= scale_2) ? scale_1 : scale_2;
  unsigned int buffer_scale = max_scale - min_scale;

  if (type == ADD_SUB)
    *scale = max_scale;
  else if (type == MUL)
    *scale = scale_1 + scale_2;

  copy_bits_dec_to_big(value_1, left);
  copy_bits_dec_to_big(value_2, right);

  if (type == ADD_SUB) {
    for (int i = buffer_scale; i > 0 && !res; i--) {
      if (scale_1 == max_scale)
        res += mul_big_to_ten(right);
      else if (scale_2 == max_scale)
        res += mul_big_to_ten(left);
    }
  }

  return res > 0 ? 1 : 0;
}

int big_to_dec(BIG_decimal value, es_decimal *result, unsigned int scale,
               int res_sign, int delete_zero) {
  int res = 0;
  int again = 0;
  int last_num = 0;
  int last_num_count = 0;

  do {
    again = 0;

    if (value.bits[3] != 0 || value.bits[4] != 0 || value.bits[5] != 0 ||
        scale > 28) {
      while (
          ((value.bits[3] != 0 || value.bits[4] != 0 || value.bits[5] != 0) &&
           (scale > 0)) ||
          scale > 28) {
        div_big_to_ten(&value, &last_num);
        last_num_count++;
        scale--;
      }
      if (value.bits[3] != 0 || value.bits[4] != 0 || value.bits[5] != 0) {
        res = res_sign > 0 ? 1 : 2;
      } else {
        again = bank_rounding(&value, last_num, last_num_count);
      }
    }
  } while (again);

  if (delete_zero) zero_deleting(&value, &scale);

  if (!set_scale(scale, result) && !res) {
    set_sign(res_sign, result);
    copy_bits_big_to_dec(value, result);
  } else {
    res = res_sign > 0 ? 1 : 2;
  }

  return res;
}

void zero_deleting(BIG_decimal *value, unsigned int *scale) {
  int last_num = 0;
  BIG_decimal copy_del_zero = *value;

  div_big_to_ten(&copy_del_zero, &last_num);

  while (*scale != 0 && last_num == 0) {
    div_big_to_ten(value, NULL);
    *scale = *scale - 1;
    copy_del_zero = *value;
    div_big_to_ten(&copy_del_zero, &last_num);
  }
}

int bank_rounding(BIG_decimal *value, int last_num, int last_num_count) {
  int again = 0;

  if (last_num == 5) {
    int last_num_result = 0;
    BIG_decimal copy = *value;
    div_big_to_ten(&copy, &last_num_result);
    if (((last_num_result % 2) == 1) || last_num_count >= 2) {
      BIG_decimal one = {{1, 0, 0, 0, 0, 0}};
      add_big(*value, one, value);
      again = 1;
    }
  } else if (last_num > 5) {
    BIG_decimal one = {{1, 0, 0, 0, 0, 0}};
    add_big(*value, one, value);
    again = 1;
  }

  return again;
}

int is_all_zero_mul(es_decimal value_1, es_decimal value_2) {
  int res = 0;

  if ((dec_is_zero(value_1) || dec_is_zero(value_2)) &&
      (value_1.bits[2] != 0 || value_2.bits[2] != 0))
    res = 1;

  return res;
}

void clear_result(es_decimal *result) {
  result->bits[0] = 0;
  result->bits[1] = 0;
  result->bits[2] = 0;
  result->bits[3] = 0;
}

int es_add(es_decimal value_1, es_decimal value_2, es_decimal *result) {
  int res = 0;
  int sign_one = 0;
  int sign_two = 0;
  int res_sign = 0;
  unsigned int scale = 0;
  BIG_decimal one = {{0, 0, 0, 0, 0, 0}};
  BIG_decimal two = {{0, 0, 0, 0, 0, 0}};
  BIG_decimal sum = {{0, 0, 0, 0, 0, 0}};

  clear_result(result);
  sign_one = get_sign(value_1);
  sign_two = get_sign(value_2);

  if (!two_dec_to_big(value_1, value_2, &one, &two, &scale, ADD_SUB)) {
    if ((sign_one + sign_two) == 2) {
      res_sign = 1;
      add_big(one, two, &sum);
    } else if ((sign_one + sign_two) == -2) {
      res_sign = -1;
      add_big(one, two, &sum);
    } else {
      if (dec_is_zero(value_1) && dec_is_zero(value_2)) {
        res_sign =
            get_scale(value_1) < get_scale(value_2) ? sign_two : sign_one;
        sub_big(one, two, &sum);
      } else if (big_bits_is_greater_or_equal(one, two)) {
        res_sign = sign_one;
        sub_big(one, two, &sum);
      } else {
        res_sign = sign_two;
        sub_big(two, one, &sum);
      }
    }

    res = big_to_dec(sum, result, scale, res_sign, 0);
  } else {
    res = 1;
  }

  return res;
}

int es_sub(es_decimal value_1, es_decimal value_2, es_decimal *result) {
  int res = 0;
  int sign_one = 0;
  int sign_two = 0;
  int res_sign = 0;
  unsigned int scale = 0;
  BIG_decimal one = {{0, 0, 0, 0, 0, 0}};
  BIG_decimal two = {{0, 0, 0, 0, 0, 0}};
  BIG_decimal sum = {{0, 0, 0, 0, 0, 0}};

  clear_result(result);
  sign_one = get_sign(value_1);
  sign_two = get_sign(value_2);

  if (!two_dec_to_big(value_1, value_2, &one, &two, &scale, ADD_SUB)) {
    if (sign_one == -1 && sign_two == 1) {
      res_sign = -1;
      add_big(one, two, &sum);
    } else if (sign_one == 1 && sign_two == -1) {
      res_sign = 1;
      add_big(one, two, &sum);
    } else {
      if (dec_is_zero(value_1) && dec_is_zero(value_2)) {
        res_sign =
            get_scale(value_1) < get_scale(value_2) ? -sign_one : sign_one;
        sub_big(one, two, &sum);
      } else if (big_bits_is_greater_or_equal(one, two)) {
        if ((sign_one + sign_two) == 2)
          res_sign = 1;
        else
          res_sign = -1;
        sub_big(one, two, &sum);
      } else {
        if ((sign_one + sign_two) == 2)
          res_sign = -1;
        else
          res_sign = 1;
        sub_big(two, one, &sum);
      }
    }
    res = big_to_dec(sum, result, scale, res_sign, 0);
  } else {
    res = 1;
  }

  return res;
}

int es_mul(es_decimal value_1, es_decimal value_2, es_decimal *result) {
  int res = 0;
  int sign_one = 0;
  int sign_two = 0;
  int res_sign = 0;
  unsigned int scale = 0;
  BIG_decimal one = {{0, 0, 0, 0, 0, 0}};
  BIG_decimal two = {{0, 0, 0, 0, 0, 0}};
  BIG_decimal mul = {{0, 0, 0, 0, 0, 0}};

  clear_result(result);
  sign_one = get_sign(value_1);
  sign_two = get_sign(value_2);

  if (is_all_zero_mul(value_1, value_2)) {
    result->bits[0] = 0;
    result->bits[1] = 0;
    result->bits[2] = 0;
    result->bits[3] = 0;
  } else if (!two_dec_to_big(value_1, value_2, &one, &two, &scale, MUL)) {
    if ((sign_one + sign_two) == 2) {
      res_sign = 1;
      mul_big(one, two, &mul);
    } else if ((sign_one + sign_two) == -2) {
      res_sign = 1;
      mul_big(one, two, &mul);
    } else {
      res_sign = -1;
      mul_big(one, two, &mul);
    }

    res = big_to_dec(mul, result, scale, res_sign, 0);
  } else {
    res = 1;
  }

  return res;
}

int es_div(es_decimal value_1, es_decimal value_2, es_decimal *result) {
  int res = 0;
  int sign_one = 0;
  int sign_two = 0;
  int res_sign = 0;
  unsigned int scale = 0;
  unsigned int scale_1 = 0;
  unsigned int scale_2 = 0;
  BIG_decimal one = {{0, 0, 0, 0, 0, 0}};
  BIG_decimal two = {{0, 0, 0, 0, 0, 0}};
  BIG_decimal div = {{0, 0, 0, 0, 0, 0}};

  clear_result(result);
  sign_one = get_sign(value_1);
  sign_two = get_sign(value_2);
  scale_1 = get_scale(value_1);
  scale_2 = get_scale(value_2);

  if (dec_is_zero(value_2)) {
    res = 3;
  } else if (dec_is_zero(value_1)) {
    result->bits[0] = 0;
    result->bits[1] = 0;
    result->bits[2] = 0;
    set_scale((scale_1 >= scale_2 ? scale_1 - scale_2 : 0), result);
    set_sign(
        ((sign_one + sign_two) == 2 || (sign_one + sign_two) == -2) ? 1 : -1,
        result);
  } else if (!two_dec_to_big(value_1, value_2, &one, &two, &scale, DIV)) {
    if ((sign_one + sign_two) == 2 || (sign_one + sign_two) == -2) {
      res_sign = 1;
    } else {
      res_sign = -1;
    }
    div_big(one, two, &div, &scale);
    if (scale_1 >= scale_2)
      scale = scale + scale_1 - scale_2;
    else {
      while (scale < (scale_2 - scale_1) && !res) {
        res = mul_big_to_ten(&div);
        if (!res) {
          scale++;
        } else {
          res = res_sign == 1 ? 1 : 2;
        }
      }
      scale = scale + scale_1 - scale_2;
    }

    if (!res) res = big_to_dec(div, result, scale, res_sign, 1);
  } else {
    res = 1;
  }

  return res;
}

int es_is_less(es_decimal one, es_decimal two) {
  int res = 0;
  int sign_one = 0;
  int sign_two = 0;
  unsigned int scale = 0;
  BIG_decimal bone = {{0, 0, 0, 0, 0, 0}};
  BIG_decimal btwo = {{0, 0, 0, 0, 0, 0}};

  sign_one = get_sign(one);
  sign_two = get_sign(two);
  two_dec_to_big(one, two, &bone, &btwo, &scale, ADD_SUB);

  if (big_is_zero(bone) && big_is_zero(btwo)) {
    res = 0;
  } else if (sign_one > sign_two) {
    res = 0;
  } else if (sign_one < sign_two) {
    res = 1;
  } else if ((sign_one + sign_two) == 2) {
    if (big_bits_is_less(bone, btwo)) {
      res = 1;
    } else {
      res = 0;
    }
  } else if ((sign_one + sign_two) == -2) {
    if (big_bits_is_less_or_equal(bone, btwo)) {
      res = 0;
    } else {
      res = 1;
    }
  }

  return res;
}

int es_is_less_or_equal(es_decimal one, es_decimal two) {
  int res = 0;

  res = es_is_greater(one, two) ? 0 : 1;

  return res;
}

int es_is_greater(es_decimal one, es_decimal two) {
  int res = 0;

  res = es_is_less(one, two) ? 0 : (es_is_equal(one, two) ? 0 : 1);

  return res;
}

int es_is_greater_or_equal(es_decimal one, es_decimal two) {
  int res = 0;

  res = es_is_less(one, two) ? 0 : 1;

  return res;
}

int es_is_equal(es_decimal one, es_decimal two) {
  int res = 0;
  int sign_one = 0;
  int sign_two = 0;
  unsigned int scale = 0;
  BIG_decimal bone = {{0, 0, 0, 0, 0, 0}};
  BIG_decimal btwo = {{0, 0, 0, 0, 0, 0}};

  sign_one = get_sign(one);
  sign_two = get_sign(two);
  two_dec_to_big(one, two, &bone, &btwo, &scale, ADD_SUB);

  if (big_is_zero(bone) && big_is_zero(btwo)) {
    res = 1;
  } else if (sign_one > sign_two) {
    res = 0;
  } else if (sign_one < sign_two) {
    res = 0;
  } else if ((sign_one + sign_two) == 2 || (sign_one + sign_two) == -2) {
    res = big_bits_is_equal(bone, btwo) ? 1 : 0;
  }

  return res;
}

int es_is_not_equal(es_decimal one, es_decimal two) {
  int res = 0;

  res = es_is_equal(one, two) ? 0 : 1;

  return res;
}

int es_from_int_to_decimal(int src, es_decimal *dst) {
  int res = 0;

  clear_result(dst);

  if (src == MIN_INT) {
    dst->bits[0] = MIN_INT;
    dst->bits[1] = 0;
    dst->bits[2] = 0;
    dst->bits[3] = MIN_INT;
  } else {
    dst->bits[0] = src >= 0 ? src : src - src - src;
    dst->bits[1] = 0;
    dst->bits[2] = 0;
    dst->bits[3] = src >= 0 ? 0 : MIN_INT;
  }

  return res;
}

int es_from_decimal_to_int(es_decimal src, int *dst) {
  int res = 0;
  int sign = 0;

  *dst = 0;
  sign = get_sign(src);

  if (!es_truncate(src, &src)) {
    if (src.bits[1] == 0 && src.bits[2] == 0) {
      if (src.bits[0] == MIN_INT && sign == -1) {
        *dst = MIN_INT;
      } else if (src.bits[0] < 0) {
        res = 1;
      } else {
        *dst = sign > 0 ? src.bits[0] : src.bits[0] - src.bits[0] - src.bits[0];
      }
    } else {
      res = 1;
    }
  } else {
    res = 1;
  }

  return res;
}

double my_pow_of_two(int count) {
  double res = 1;

  for (int i = 1; i <= count; i++) {
    res *= 2;
  }

  return res;
}

int es_from_decimal_to_float(es_decimal src, float *dst) {
  int res = 0;
  int sign = 0;
  int scale = 0;
  double copy = 0.0;

  *dst = 0.0;
  sign = get_sign(src);
  scale = get_scale(src);

  if (scale <= 28) {
    for (int i = 0; i <= 95 && !res; i++) {
      copy += my_pow_of_two(i) * get_bit_dec(src, i);
    }

    while (scale-- > 0) {
      // copy = scale > 6 ? roundf(copy) / 10 : copy / 10;
      copy /= 10;
    }

    *dst = (float)copy;
    *dst *= sign;
  } else {
    res = 1;
  }

  return res;
}

int es_floor(es_decimal value, es_decimal *result) {
  int res = 0;
  int sign = 0;
  unsigned int scale = 0;

  clear_result(result);
  sign = get_sign(value);
  scale = get_scale(value);

  if (scale != 0) {
    if (sign == 1) {
      res += es_truncate(value, result);
    } else if (sign == -1) {
      es_decimal one = {{1, 0, 0, 0}};
      es_decimal copy = {{0, 0, 0, 0}};
      res += es_truncate(value, &copy);
      res += es_sub(copy, one, result);
    }

    if (res > 0) res = 1;
  } else {
    *result = value;
  }

  return res;
}

int es_round(es_decimal value, es_decimal *result) {
  int res = 0;
  int sign = 0;
  unsigned int scale = 0;
  int fraction = 0;
  BIG_decimal copy = {{0, 0, 0, 0, 0, 0}};

  clear_result(result);
  sign = get_sign(value);
  scale = get_scale(value);

  if (scale != 0) {
    if (set_scale(0, &value)) {
      res = 1;
    } else {
      copy_bits_dec_to_big(value, &copy);

      for (unsigned int i = 0; i < scale; i++) {
        div_big_to_ten(&copy, &fraction);
      }
      bank_rounding(&copy, fraction, 0);
      copy_bits_big_to_dec(copy, result);
      set_sign(sign, result);
    }
  } else {
    *result = value;
  }

  return res;
}

int es_truncate(es_decimal value, es_decimal *result) {
  int res = 0;
  int sign = 0;
  unsigned int scale = 0;
  BIG_decimal copy = {{0, 0, 0, 0, 0, 0}};

  clear_result(result);
  sign = get_sign(value);
  scale = get_scale(value);

  if (scale != 0) {
    if (set_scale(0, &value)) {
      res = 1;
    } else {
      copy_bits_dec_to_big(value, &copy);

      for (unsigned int i = 0; i < scale; i++) {
        div_big_to_ten(&copy, NULL);
      }

      copy_bits_big_to_dec(copy, result);
      set_sign(sign, result);
    }
  } else {
    *result = value;
  }

  return res;
}

int es_negate(es_decimal value, es_decimal *result) {
  int res = 0;

  clear_result(result);

  if (get_scale(value) > 28) {
    res = 1;
  } else if (get_sign(value) == 1) {
    value.bits[3] |= (1 << 31);
    copy_bits_dec(value, result);
  } else if (get_sign(value) == -1) {
    value.bits[3] &= ~(1 << 31);
    copy_bits_dec(value, result);
  }

  return res;
}

int es_from_float_to_decimal(float src, es_decimal *dst) {
  int res = 0;

  clear_result(dst);

  if (src != src || src == 1.0 / 0.0 || src == -1.0 / 0.0) {
    res = 1;
  } else {
    int sign = 0;
    char str[30] = "\0";
    char num[8] = "\0";
    char scale[3] = "\0";
    int int_scale = 0;
    unsigned int res_scale = 6;
    es_decimal copy = {{0, 0, 0, 0}};
    es_decimal ten = {{10, 0, 0, 0}};

    sign = src >= 0 ? 1 : -1;

    if (sign == -1) src *= -1;
    sprintf(str, "%.6e", src);

    num[0] = str[0];
    for (int i = 2; i <= 7; i++) {
      num[i - 1] = str[i];
    }

    scale[0] = str[10];
    scale[1] = str[11];
    if (str[9] == '-') {
      int_scale = -(atoi(scale));
    } else if (str[9] == '+') {
      int_scale = atoi(scale);
    }

    if (int_scale > 28 || int_scale < -28 ||
        (int_scale == 28 && atoi(num) >= 7922817) ||
        (int_scale == -28 && atoi(num) <= -7922817)) {
      res = 1;
    } else {
      set_sign(sign, &copy);
      copy.bits[0] = atoi(num);
      set_scale(res_scale, &copy);

      BIG_decimal big_copy = {{0, 0, 0, 0, 0, 0}};
      copy_bits_dec_to_big(copy, &big_copy);
      zero_deleting(&big_copy, &res_scale);
      copy_bits_big_to_dec(big_copy, &copy);
      set_scale(res_scale, &copy);

      while (int_scale < 0) {
        es_div(copy, ten, &copy);
        int_scale++;
      }

      while (int_scale > 0) {
        es_mul(copy, ten, &copy);
        int_scale--;
      }

      res_scale = get_scale(copy);
      copy_bits_dec_to_big(copy, &big_copy);
      zero_deleting(&big_copy, &res_scale);
      copy_bits_big_to_dec(big_copy, &copy);
      set_scale(res_scale, &copy);

      *dst = copy;
    }
  }

  return res;
}
