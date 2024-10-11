using System;
using System.IO;

class s21_decimal {
  public static int DotPosition(string s) {
    int i = s.IndexOf('.');
    if (i == -1)
      i = s.Length;
    
    return i;
  }


  // -------------------------------------------------------------------------------------


  public static void MakeTestPair(string name, int testNum, StreamWriter stream, string s1, string s2) {
    char[] separators = new char[] { ' ', ',' };
    string[] one = s1.Split(separators, StringSplitOptions.RemoveEmptyEntries);
    string[] two = s2.Split(separators, StringSplitOptions.RemoveEmptyEntries);
    int[] first = new int [] {Int32.Parse(one[0]), Int32.Parse(one[1]), Int32.Parse(one[2]), Int32.Parse(one[3])};
    int[] second = new int [] {Int32.Parse(two[0]), Int32.Parse(two[1]), Int32.Parse(two[2]), Int32.Parse(two[3])};
    decimal first_dec = new decimal(first);
    decimal second_dec = new decimal(second);

    stream.WriteLine("#test " + name + "_" + testNum);

    switch(name) {
      case "add": {
        decimal orig_res = new Decimal(new int[] {0, 0, 0, 0});
        int exp_res = 0;

        try {
          orig_res = Decimal.Add(new decimal(first), new decimal(second));
        } catch(System.OverflowException) {
          if (first[3] >= 0 && second[3] >= 0)
            exp_res = 1;
          else
            exp_res = 2;
        }
        int[] orig_bits = decimal.GetBits(orig_res);
        
        stream.WriteLine("  //  {0}{1}", new string(' ', 32 - DotPosition(first_dec.ToString())), first_dec.ToString());
        stream.WriteLine("  // +{0}{1}", new string(' ', 32 - DotPosition(second_dec.ToString())), second_dec.ToString());
        stream.WriteLine("  //   ------------------------------------------------");
        stream.WriteLine("  //  {0}{1}", new string(' ', 32 - DotPosition(orig_res.ToString())), orig_res.ToString());
        stream.WriteLine("  //  {0}{{{{{1}, {2}, {3}, {4}}}}};", new string(' ', 32 - DotPosition(orig_res.ToString())), orig_bits[0], orig_bits[1], orig_bits[2], orig_bits[3]);
        stream.WriteLine("");
        stream.WriteLine("  s21_decimal one = {{{{{0}, {1}, {2}, {3}}}}};", first[0], first[1], first[2], first[3]);
        stream.WriteLine("  s21_decimal two = {{{{{0}, {1}, {2}, {3}}}}};", second[0], second[1], second[2], second[3]);
        stream.WriteLine("  s21_decimal res = {{0, 0, 0, 0}};");
        stream.WriteLine("  ck_assert_int_eq(s21_" + name + "(one, two, &res), " + "{0});", exp_res);

        stream.WriteLine("  ck_assert_int_eq(res.bits[0], {0});", orig_bits[0]);
        stream.WriteLine("  ck_assert_int_eq(res.bits[1], {0});", orig_bits[1]);
        stream.WriteLine("  ck_assert_int_eq(res.bits[2], {0});", orig_bits[2]);
        stream.WriteLine("  ck_assert_int_eq(res.bits[3], {0});", orig_bits[3]);
        break;
      }

      case "sub": {
        decimal orig_res = new Decimal(new int[] {0, 0, 0, 0});
        int exp_res = 0;

        try {
          orig_res = Decimal.Subtract(new decimal(first), new decimal(second));
        } catch(System.OverflowException) {
          if (first[3] < 0 && second[3] >= 0)
            exp_res = 2;
          else
            exp_res = 1;
        }
        int[] orig_bits = decimal.GetBits(orig_res);
        
        stream.WriteLine("  //  {0}{1}", new string(' ', 32 - DotPosition(first_dec.ToString())), first_dec.ToString());
        stream.WriteLine("  // -{0}{1}", new string(' ', 32 - DotPosition(second_dec.ToString())), second_dec.ToString());
        stream.WriteLine("  //   ------------------------------------------------");
        stream.WriteLine("  //  {0}{1}", new string(' ', 32 - DotPosition(orig_res.ToString())), orig_res.ToString());
        stream.WriteLine("  //  {0}{{{{{1}, {2}, {3}, {4}}}}};", new string(' ', 32 - DotPosition(orig_res.ToString())), orig_bits[0], orig_bits[1], orig_bits[2], orig_bits[3]);
        stream.WriteLine("");
        stream.WriteLine("  s21_decimal one = {{{{{0}, {1}, {2}, {3}}}}};", first[0], first[1], first[2], first[3]);
        stream.WriteLine("  s21_decimal two = {{{{{0}, {1}, {2}, {3}}}}};", second[0], second[1], second[2], second[3]);
        stream.WriteLine("  s21_decimal res = {{0, 0, 0, 0}};");
        stream.WriteLine("  ck_assert_int_eq(s21_" + name + "(one, two, &res), " + "{0});", exp_res);

        stream.WriteLine("  ck_assert_int_eq(res.bits[0], {0});", orig_bits[0]);
        stream.WriteLine("  ck_assert_int_eq(res.bits[1], {0});", orig_bits[1]);
        stream.WriteLine("  ck_assert_int_eq(res.bits[2], {0});", orig_bits[2]);
        stream.WriteLine("  ck_assert_int_eq(res.bits[3], {0});", orig_bits[3]);
        break;
      }

      case "mul": {
        decimal orig_res = new Decimal(new int[] {0, 0, 0, 0});
        int exp_res = 0;

        try {
          orig_res = Decimal.Multiply(new decimal(first), new decimal(second));
        } catch(System.OverflowException) {
          if (first[3] >= 0 && second[3] >= 0)
            exp_res = 1;
          else if (first[3] < 0 && second[3] < 0)
            exp_res = 1;
          else
            exp_res = 2;
        }
        int[] orig_bits = decimal.GetBits(orig_res);
        
        stream.WriteLine("  //  {0}{1}", new string(' ', 32 - DotPosition(first_dec.ToString())), first_dec.ToString());
        stream.WriteLine("  // *{0}{1}", new string(' ', 32 - DotPosition(second_dec.ToString())), second_dec.ToString());
        stream.WriteLine("  //   ------------------------------------------------");
        stream.WriteLine("  //  {0}{1}", new string(' ', 32 - DotPosition(orig_res.ToString())), orig_res.ToString());
        stream.WriteLine("  //  {0}{{{{{1}, {2}, {3}, {4}}}}};", new string(' ', 32 - DotPosition(orig_res.ToString())), orig_bits[0], orig_bits[1], orig_bits[2], orig_bits[3]);
        stream.WriteLine("");
        stream.WriteLine("  s21_decimal one = {{{{{0}, {1}, {2}, {3}}}}};", first[0], first[1], first[2], first[3]);
        stream.WriteLine("  s21_decimal two = {{{{{0}, {1}, {2}, {3}}}}};", second[0], second[1], second[2], second[3]);
        stream.WriteLine("  s21_decimal res = {{0, 0, 0, 0}};");
        stream.WriteLine("  ck_assert_int_eq(s21_" + name + "(one, two, &res), " + "{0});", exp_res);

        stream.WriteLine("  ck_assert_int_eq(res.bits[0], {0});", orig_bits[0]);
        stream.WriteLine("  ck_assert_int_eq(res.bits[1], {0});", orig_bits[1]);
        stream.WriteLine("  ck_assert_int_eq(res.bits[2], {0});", orig_bits[2]);
        stream.WriteLine("  ck_assert_int_eq(res.bits[3], {0});", orig_bits[3]);
        break;
      }

      case "div": {
        decimal orig_res = new Decimal(new int[] {0, 0, 0, 0});
        int exp_res = 0;

        try {
          orig_res = Decimal.Divide(new decimal(first), new decimal(second));
        } catch(System.OverflowException) {
          if (first[3] >= 0 && second[3] >= 0)
            exp_res = 1;
          else if (first[3] < 0 && second[3] < 0)
            exp_res = 1;
          else
            exp_res = 2;
        } catch(System.DivideByZeroException) {
          exp_res = 3;
        }
        int[] orig_bits = decimal.GetBits(orig_res);
        
        stream.WriteLine("  //  {0}{1}", new string(' ', 32 - DotPosition(first_dec.ToString())), first_dec.ToString());
        stream.WriteLine("  // /{0}{1}", new string(' ', 32 - DotPosition(second_dec.ToString())), second_dec.ToString());
        stream.WriteLine("  //   ------------------------------------------------");
        stream.WriteLine("  //  {0}{1}", new string(' ', 32 - DotPosition(orig_res.ToString())), orig_res.ToString());
        stream.WriteLine("  //  {0}{{{{{1}, {2}, {3}, {4}}}}};", new string(' ', 32 - DotPosition(orig_res.ToString())), orig_bits[0], orig_bits[1], orig_bits[2], orig_bits[3]);
        stream.WriteLine("");
        stream.WriteLine("  s21_decimal one = {{{{{0}, {1}, {2}, {3}}}}};", first[0], first[1], first[2], first[3]);
        stream.WriteLine("  s21_decimal two = {{{{{0}, {1}, {2}, {3}}}}};", second[0], second[1], second[2], second[3]);
        stream.WriteLine("  s21_decimal res = {{0, 0, 0, 0}};");
        stream.WriteLine("  ck_assert_int_eq(s21_" + name + "(one, two, &res), " + "{0});", exp_res);

        stream.WriteLine("  ck_assert_int_eq(res.bits[0], {0});", orig_bits[0]);
        stream.WriteLine("  ck_assert_int_eq(res.bits[1], {0});", orig_bits[1]);
        stream.WriteLine("  ck_assert_int_eq(res.bits[2], {0});", orig_bits[2]);
        stream.WriteLine("  ck_assert_int_eq(res.bits[3], {0});", orig_bits[3]);
        break;
      }

      case "mod": {
        
        break;
      }

      case "is_less": {
        int orig_res = 0;

        decimal f1 = new decimal(first);
        decimal f2 = new decimal(second);
        if (f1 < f2)
          orig_res = 1;
        else 
          orig_res = 0;
        
        stream.WriteLine("  //  {0}{1}", new string(' ', 32 - DotPosition(first_dec.ToString())), first_dec.ToString());
        stream.WriteLine("  // <{0}{1}", new string(' ', 32 - DotPosition(second_dec.ToString())), second_dec.ToString());
        stream.WriteLine("  //   ------------------------------------------------");
        stream.WriteLine("  //  {0}{1}", new string(' ', 32 - DotPosition(orig_res.ToString())), orig_res.ToString());
        stream.WriteLine("");
        stream.WriteLine("  s21_decimal one = {{{{{0}, {1}, {2}, {3}}}}};", first[0], first[1], first[2], first[3]);
        stream.WriteLine("  s21_decimal two = {{{{{0}, {1}, {2}, {3}}}}};", second[0], second[1], second[2], second[3]);
        stream.WriteLine("  ck_assert_int_eq(s21_" + name + "(one, two), " + "{0});", orig_res);
        break;
      }

      case "is_less_or_equal": {
        int orig_res = 0;

        decimal f1 = new decimal(first);
        decimal f2 = new decimal(second);
        if (f1 <= f2)
          orig_res = 1;
        else 
          orig_res = 0;
        
        stream.WriteLine("  //  {0}{1}", new string(' ', 32 - DotPosition(first_dec.ToString())), first_dec.ToString());
        stream.WriteLine("  //<={0}{1}", new string(' ', 32 - DotPosition(second_dec.ToString())), second_dec.ToString());
        stream.WriteLine("  //   ------------------------------------------------");
        stream.WriteLine("  //  {0}{1}", new string(' ', 32 - DotPosition(orig_res.ToString())), orig_res.ToString());
        stream.WriteLine("");
        stream.WriteLine("  s21_decimal one = {{{{{0}, {1}, {2}, {3}}}}};", first[0], first[1], first[2], first[3]);
        stream.WriteLine("  s21_decimal two = {{{{{0}, {1}, {2}, {3}}}}};", second[0], second[1], second[2], second[3]);
        stream.WriteLine("  ck_assert_int_eq(s21_" + name + "(one, two), " + "{0});", orig_res);
        break;
      }

      case "is_greater": {
        int orig_res = 0;

        decimal f1 = new decimal(first);
        decimal f2 = new decimal(second);
        if (f1 > f2)
          orig_res = 1;
        else 
          orig_res = 0;
        
        stream.WriteLine("  //  {0}{1}", new string(' ', 32 - DotPosition(first_dec.ToString())), first_dec.ToString());
        stream.WriteLine("  // >{0}{1}", new string(' ', 32 - DotPosition(second_dec.ToString())), second_dec.ToString());
        stream.WriteLine("  //   ------------------------------------------------");
        stream.WriteLine("  //  {0}{1}", new string(' ', 32 - DotPosition(orig_res.ToString())), orig_res.ToString());
        stream.WriteLine("");
        stream.WriteLine("  s21_decimal one = {{{{{0}, {1}, {2}, {3}}}}};", first[0], first[1], first[2], first[3]);
        stream.WriteLine("  s21_decimal two = {{{{{0}, {1}, {2}, {3}}}}};", second[0], second[1], second[2], second[3]);
        stream.WriteLine("  ck_assert_int_eq(s21_" + name + "(one, two), " + "{0});", orig_res);
        break;
      }

      case "is_greater_or_equal": {
        int orig_res = 0;

        decimal f1 = new decimal(first);
        decimal f2 = new decimal(second);
        if (f1 >= f2)
          orig_res = 1;
        else 
          orig_res = 0;
        
        stream.WriteLine("  //  {0}{1}", new string(' ', 32 - DotPosition(first_dec.ToString())), first_dec.ToString());
        stream.WriteLine("  // >={0}{1}", new string(' ', 32 - DotPosition(second_dec.ToString())), second_dec.ToString());
        stream.WriteLine("  //   ------------------------------------------------");
        stream.WriteLine("  //  {0}{1}", new string(' ', 32 - DotPosition(orig_res.ToString())), orig_res.ToString());
        stream.WriteLine("");
        stream.WriteLine("  s21_decimal one = {{{{{0}, {1}, {2}, {3}}}}};", first[0], first[1], first[2], first[3]);
        stream.WriteLine("  s21_decimal two = {{{{{0}, {1}, {2}, {3}}}}};", second[0], second[1], second[2], second[3]);
        stream.WriteLine("  ck_assert_int_eq(s21_" + name + "(one, two), " + "{0});", orig_res);
        break;
      }

      case "is_equal": {
        int orig_res = 0;

        decimal f1 = new decimal(first);
        decimal f2 = new decimal(second);
        if (f1 == f2)
          orig_res = 1;
        else 
          orig_res = 0;
        
        stream.WriteLine("  //  {0}{1}", new string(' ', 32 - DotPosition(first_dec.ToString())), first_dec.ToString());
        stream.WriteLine("  //=={0}{1}", new string(' ', 32 - DotPosition(second_dec.ToString())), second_dec.ToString());
        stream.WriteLine("  //   ------------------------------------------------");
        stream.WriteLine("  //  {0}{1}", new string(' ', 32 - DotPosition(orig_res.ToString())), orig_res.ToString());
        stream.WriteLine("");
        stream.WriteLine("  s21_decimal one = {{{{{0}, {1}, {2}, {3}}}}};", first[0], first[1], first[2], first[3]);
        stream.WriteLine("  s21_decimal two = {{{{{0}, {1}, {2}, {3}}}}};", second[0], second[1], second[2], second[3]);
        stream.WriteLine("  ck_assert_int_eq(s21_" + name + "(one, two), " + "{0});", orig_res);
        break;
      }

      case "is_not_equal": {
        int orig_res = 0;

        decimal f1 = new decimal(first);
        decimal f2 = new decimal(second);
        if (f1 != f2)
          orig_res = 1;
        else 
          orig_res = 0;
        
        stream.WriteLine("  //  {0}{1}", new string(' ', 32 - DotPosition(first_dec.ToString())), first_dec.ToString());
        stream.WriteLine("  //!={0}{1}", new string(' ', 32 - DotPosition(second_dec.ToString())), second_dec.ToString());
        stream.WriteLine("  //   ------------------------------------------------");
        stream.WriteLine("  //  {0}{1}", new string(' ', 32 - DotPosition(orig_res.ToString())), orig_res.ToString());
        stream.WriteLine("");
        stream.WriteLine("  s21_decimal one = {{{{{0}, {1}, {2}, {3}}}}};", first[0], first[1], first[2], first[3]);
        stream.WriteLine("  s21_decimal two = {{{{{0}, {1}, {2}, {3}}}}};", second[0], second[1], second[2], second[3]);
        stream.WriteLine("  ck_assert_int_eq(s21_" + name + "(one, two), " + "{0});", orig_res);
        break;
      }
    }

    stream.WriteLine("");
  }


// -------------------------------------------------------------------------------------


  public static void MakeTestSingle(string name, int testNum, StreamWriter stream, string s) {
    char[] separators = new char[] { ' ', ',' };
    string[] one = s.Split(separators, StringSplitOptions.RemoveEmptyEntries);
    int[] first = new int [] {Int32.Parse(one[0]), Int32.Parse(one[1]), Int32.Parse(one[2]), Int32.Parse(one[3])};
    decimal first_dec = new decimal(first);

    stream.WriteLine("#test " + name + "_" + testNum);

    switch(name) {
      case "from_float_to_decimal": {
        
        break;
      }

      case "from_decimal_to_int": {
        int orig_res = 0;
        int exp_res = 0;

        try {
          orig_res = Decimal.ToInt32(first_dec);
        } catch(System.OverflowException) {
          exp_res = 1;
        }
        
        stream.WriteLine("  //  {0}{1}", new string(' ', 32 - DotPosition(first_dec.ToString())), first_dec.ToString());
        stream.WriteLine("  //   -dec->int---------------------------------------");
        stream.WriteLine("  //  {0}{1}", new string(' ', 32 - DotPosition(orig_res.ToString())), orig_res.ToString());
        stream.WriteLine("");
        stream.WriteLine("  s21_decimal one = {{{{{0}, {1}, {2}, {3}}}}};", first[0], first[1], first[2], first[3]);
        stream.WriteLine("  int res = 0;");
        stream.WriteLine("  ck_assert_int_eq(s21_" + name + "(one, &res), " + "{0});", exp_res);

        stream.WriteLine("  ck_assert_int_eq(res, {0});", orig_res);
        break;
      }

      case "from_decimal_to_float": {
        float orig_res = 0;
        int exp_res = 0;

        try {
          orig_res = decimal.ToSingle(first_dec);
        } catch(System.OverflowException) {
          exp_res = 1;
        }
        
        stream.WriteLine("  //  {0}{1}", new string(' ', 32 - DotPosition(first_dec.ToString())), first_dec.ToString());
        stream.WriteLine("  //   -dec->float-------------------------------------");
        stream.WriteLine("  //  {0}", orig_res);
        stream.WriteLine("");
        stream.WriteLine("  s21_decimal one = {{{{{0}, {1}, {2}, {3}}}}};", first[0], first[1], first[2], first[3]);
        stream.WriteLine("  float res = 0;");
        stream.WriteLine("  ck_assert_int_eq(s21_" + name + "(one, &res), " + "{0});", exp_res);

        stream.WriteLine("  ck_assert_float_eq(res, {0});", orig_res);
        break;
      }

      case "floor": {
        decimal orig_res = new Decimal(new int[] {0, 0, 0, 0});
        int exp_res = 0;

        try {
          orig_res = Decimal.Floor(first_dec);
        } catch(System.OverflowException) {
          exp_res = 1;
        }
        int[] orig_bits = decimal.GetBits(orig_res);
        
        stream.WriteLine("  //  {0}{1}", new string(' ', 32 - DotPosition(first_dec.ToString())), first_dec.ToString());
        stream.WriteLine("  //   -floor------------------------------------------");
        stream.WriteLine("  //  {0}{1}", new string(' ', 32 - DotPosition(orig_res.ToString())), orig_res.ToString());
        stream.WriteLine("  //  {0}{{{{{1}, {2}, {3}, {4}}}}};", new string(' ', 32 - DotPosition(orig_res.ToString())), orig_bits[0], orig_bits[1], orig_bits[2], orig_bits[3]);
        stream.WriteLine("");
        stream.WriteLine("  s21_decimal one = {{{{{0}, {1}, {2}, {3}}}}};", first[0], first[1], first[2], first[3]);
        stream.WriteLine("  s21_decimal res = {{0, 0, 0, 0}};");
        stream.WriteLine("  ck_assert_int_eq(s21_" + name + "(one, &res), " + "{0});", exp_res);

        stream.WriteLine("  ck_assert_int_eq(res.bits[0], {0});", orig_bits[0]);
        stream.WriteLine("  ck_assert_int_eq(res.bits[1], {0});", orig_bits[1]);
        stream.WriteLine("  ck_assert_int_eq(res.bits[2], {0});", orig_bits[2]);
        stream.WriteLine("  ck_assert_int_eq(res.bits[3], {0});", orig_bits[3]);
        break;
      }

      case "round": {
        decimal orig_res = new Decimal(new int[] {0, 0, 0, 0});
        int exp_res = 0;

        try {
          orig_res = Decimal.Round(first_dec);
        } catch(System.OverflowException) {
          exp_res = 1;
        }
        int[] orig_bits = decimal.GetBits(orig_res);
        
        stream.WriteLine("  //  {0}{1}", new string(' ', 32 - DotPosition(first_dec.ToString())), first_dec.ToString());
        stream.WriteLine("  //   -round------------------------------------------");
        stream.WriteLine("  //  {0}{1}", new string(' ', 32 - DotPosition(orig_res.ToString())), orig_res.ToString());
        stream.WriteLine("  //  {0}{{{{{1}, {2}, {3}, {4}}}}};", new string(' ', 32 - DotPosition(orig_res.ToString())), orig_bits[0], orig_bits[1], orig_bits[2], orig_bits[3]);
        stream.WriteLine("");
        stream.WriteLine("  s21_decimal one = {{{{{0}, {1}, {2}, {3}}}}};", first[0], first[1], first[2], first[3]);
        stream.WriteLine("  s21_decimal res = {{0, 0, 0, 0}};");
        stream.WriteLine("  ck_assert_int_eq(s21_" + name + "(one, &res), " + "{0});", exp_res);

        stream.WriteLine("  ck_assert_int_eq(res.bits[0], {0});", orig_bits[0]);
        stream.WriteLine("  ck_assert_int_eq(res.bits[1], {0});", orig_bits[1]);
        stream.WriteLine("  ck_assert_int_eq(res.bits[2], {0});", orig_bits[2]);
        stream.WriteLine("  ck_assert_int_eq(res.bits[3], {0});", orig_bits[3]);
        break;
      }

      case "truncate": {
        decimal orig_res = new Decimal(new int[] {0, 0, 0, 0});
        int exp_res = 0;

        try {
          orig_res = Decimal.Truncate(first_dec);
        } catch(System.OverflowException) {
          exp_res = 1;
        }
        int[] orig_bits = decimal.GetBits(orig_res);
        
        stream.WriteLine("  //  {0}{1}", new string(' ', 32 - DotPosition(first_dec.ToString())), first_dec.ToString());
        stream.WriteLine("  //   -truncate---------------------------------------");
        stream.WriteLine("  //  {0}{1}", new string(' ', 32 - DotPosition(orig_res.ToString())), orig_res.ToString());
        stream.WriteLine("  //  {0}{{{{{1}, {2}, {3}, {4}}}}};", new string(' ', 32 - DotPosition(orig_res.ToString())), orig_bits[0], orig_bits[1], orig_bits[2], orig_bits[3]);
        stream.WriteLine("");
        stream.WriteLine("  s21_decimal one = {{{{{0}, {1}, {2}, {3}}}}};", first[0], first[1], first[2], first[3]);
        stream.WriteLine("  s21_decimal res = {{0, 0, 0, 0}};");
        stream.WriteLine("  ck_assert_int_eq(s21_" + name + "(one, &res), " + "{0});", exp_res);

        stream.WriteLine("  ck_assert_int_eq(res.bits[0], {0});", orig_bits[0]);
        stream.WriteLine("  ck_assert_int_eq(res.bits[1], {0});", orig_bits[1]);
        stream.WriteLine("  ck_assert_int_eq(res.bits[2], {0});", orig_bits[2]);
        stream.WriteLine("  ck_assert_int_eq(res.bits[3], {0});", orig_bits[3]);
        break;
      }

      case "negate": {
        decimal orig_res = new Decimal(new int[] {0, 0, 0, 0});
        int exp_res = 0;

        try {
          orig_res = Decimal.Negate(first_dec);
        } catch(System.OverflowException) {
          exp_res = 1;
        }
        int[] orig_bits = decimal.GetBits(orig_res);
        
        stream.WriteLine("  //  {0}{1}", new string(' ', 32 - DotPosition(first_dec.ToString())), first_dec.ToString());
        stream.WriteLine("  //   -negate-----------------------------------------");
        stream.WriteLine("  //  {0}{1}", new string(' ', 32 - DotPosition(orig_res.ToString())), orig_res.ToString());
        stream.WriteLine("  //  {0}{{{{{1}, {2}, {3}, {4}}}}};", new string(' ', 32 - DotPosition(orig_res.ToString())), orig_bits[0], orig_bits[1], orig_bits[2], orig_bits[3]);
        stream.WriteLine("");
        stream.WriteLine("  s21_decimal one = {{{{{0}, {1}, {2}, {3}}}}};", first[0], first[1], first[2], first[3]);
        stream.WriteLine("  s21_decimal res = {{0, 0, 0, 0}};");
        stream.WriteLine("  ck_assert_int_eq(s21_" + name + "(one, &res), " + "{0});", exp_res);

        stream.WriteLine("  ck_assert_int_eq(res.bits[0], {0});", orig_bits[0]);
        stream.WriteLine("  ck_assert_int_eq(res.bits[1], {0});", orig_bits[1]);
        stream.WriteLine("  ck_assert_int_eq(res.bits[2], {0});", orig_bits[2]);
        stream.WriteLine("  ck_assert_int_eq(res.bits[3], {0});", orig_bits[3]);
        break;
      }
    }

    stream.WriteLine("");
  }


  // -------------------------------------------------------------------------------------


  public static void MakeTestInt(string name, int testNum, StreamWriter stream, string s) {
    char[] separators = new char[] { ' ', ',' };
    string[] one = s.Split(separators, StringSplitOptions.RemoveEmptyEntries);
    int first = Int32.Parse(one[0]);

    stream.WriteLine("#test " + name + "_" + testNum);

    switch (name) {
      case "from_int_to_decimal": {
          decimal orig_res = new Decimal(new int[] {0, 0, 0, 0});
          int exp_res = 0;

          orig_res = Convert.ToDecimal(first);
          int[] orig_bits = decimal.GetBits(orig_res);
          
          stream.WriteLine("  //  {0}{1}", new string(' ', 32 - DotPosition(first.ToString())), first.ToString());
          stream.WriteLine("  //   -int->dec---------------------------------------");
          stream.WriteLine("  //  {0}{1}", new string(' ', 32 - DotPosition(orig_res.ToString())), orig_res.ToString());
          stream.WriteLine("");
          stream.WriteLine("  int one = {0};", first);
          stream.WriteLine("  s21_decimal res_dec = {{0, 0, 0, 0}};");
          stream.WriteLine("");
          stream.WriteLine("  ck_assert_int_eq(s21_" + name + "(one, &res_dec), " + "{0});", exp_res);
          stream.WriteLine("  ck_assert_int_eq(res_dec.bits[0], {0});", orig_bits[0]);
          stream.WriteLine("  ck_assert_int_eq(res_dec.bits[1], {0});", orig_bits[1]);
          stream.WriteLine("  ck_assert_int_eq(res_dec.bits[2], {0});", orig_bits[2]);
          stream.WriteLine("  ck_assert_int_eq(res_dec.bits[3], {0});", orig_bits[3]);

          break;
      }
    }

    stream.WriteLine("");
  }


  // -------------------------------------------------------------------------------------


  public static void MakeTestFloat(string name, int testNum, StreamWriter stream, string s) {
    char[] separators = new char[] { ' ', ',' };
    string[] one = s.Split(separators, StringSplitOptions.RemoveEmptyEntries);
    float first = Single.Parse(one[0]);

    stream.WriteLine("#test " + name + "_" + testNum);

    switch (name) {
      case "from_float_to_decimal": {
        decimal orig_res = new Decimal(new int[] {0, 0, 0, 0});
        int exp_res = 0;

        try {
          orig_res = Convert.ToDecimal(first);;
        } catch(System.OverflowException) {
          exp_res = 1;
        }

        int[] orig_bits = decimal.GetBits(orig_res);
        
        stream.WriteLine("  //  {0}{1}", new string(' ', 32 - DotPosition(first.ToString())), first.ToString());
        stream.WriteLine("  //   -float->dec-------------------------------------");
        stream.WriteLine("  //  {0}{1}", new string(' ', 32 - DotPosition(orig_res.ToString())), orig_res.ToString());
        stream.WriteLine("");
        stream.WriteLine("  float one = {0};", first);
        stream.WriteLine("  s21_decimal res_dec = {{0, 0, 0, 0}};");
        stream.WriteLine("");
        stream.WriteLine("  ck_assert_int_eq(s21_" + name + "(one, &res_dec), " + "{0});", exp_res);
        stream.WriteLine("  ck_assert_int_eq(res_dec.bits[0], {0});", orig_bits[0]);
        stream.WriteLine("  ck_assert_int_eq(res_dec.bits[1], {0});", orig_bits[1]);
        stream.WriteLine("  ck_assert_int_eq(res_dec.bits[2], {0});", orig_bits[2]);
        stream.WriteLine("  ck_assert_int_eq(res_dec.bits[3], {0});", orig_bits[3]);

        break;
      }
    }

    stream.WriteLine("");
  }


  // -------------------------------------------------------------------------------------


  public static void CheckMaker(string name, int type) {
    string fileName = "../task_test/" + name + ".check";
    int testNum = 0;
    string[] lines = File.ReadAllLines("../decimal_cases.txt");
    string[] lines2 = File.ReadAllLines("../int_cases.txt");
    string[] lines3 = File.ReadAllLines("../float_cases.txt");

    using (StreamWriter stream = new StreamWriter(fileName)) {
      stream.WriteLine("#suite " + name + "_test\n");
      if (type == 2) {
        foreach (string s1 in lines) {
          foreach (string s2 in lines) {
            testNum++;
            MakeTestPair(name, testNum, stream, s1, s2);
          }
        } 
      } else if (type == 1) {
          foreach (string s in lines) {
            testNum++;
            MakeTestSingle(name, testNum, stream, s);
          }
      } else if (type == 3) {
          foreach (string s in lines2) {
            testNum++;
            MakeTestInt(name, testNum, stream, s);
          }
      } else if (type == 4) {
          foreach (string s in lines3) {
            testNum++;
            MakeTestFloat(name, testNum, stream, s);
          }
      }
    }
  }


// -------------------------------------------------------------------------------------


  static void Main() {
  // string[] tests_single = {"from_int_to_decimal", "from_float_to_decimal",
  //                          "from_decimal_to_int", "from_decimal_to_float",
  //                          "floor", "round", "truncate", "negate"};

  // string[] tests_pair = {"add", "sub", "mul", "div", "mod", 
  //                     "is_less", "is_less_or_equal", "is_greater",
  //                     "is_greater_or_equal", "is_equal", "is_not_equal"};

    string[] tests_single = {"from_decimal_to_int", "from_decimal_to_float", "floor", "round", "truncate", "negate"};

    string[] tests_pair = {"add", "sub", "mul", "div",
                          "is_less", "is_less_or_equal", "is_greater",
                          "is_greater_or_equal", "is_equal", "is_not_equal"};
    
    string[] tests_int = {"from_int_to_decimal"};

    string[] tests_float = {"from_float_to_decimal"};

    foreach (string s in tests_pair) {
      CheckMaker(s, 2);
    }

    foreach (string s in tests_single) {
      CheckMaker(s, 1);
    }

    foreach (string s in tests_int) {
      CheckMaker(s, 3);
    }

    foreach (string s in tests_float) {
      CheckMaker(s, 4);
    }

    Console.WriteLine("========TESTS DONE========");
  }
}
