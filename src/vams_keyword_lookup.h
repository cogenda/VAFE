/* C code produced by gperf version 3.0.3 */
/* Command-line: ./3rdparty/bin/gperf vams_keyword_lookup.gperf  */
/* Computed positions: -k'1-4,9,$' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gnu-gperf@gnu.org>."
#endif

#line 9 "vams_keyword_lookup.gperf"

#include <string.h>
#define IEEE_1364_1995 (1 << 0)
#define IEEE_1364_2001 (1 << 1)
#define IEEE_1364_2005 (1 << 2)
#define IEEE_1800_2005 (1 << 3)
#define VA_1_0         (1 << 4)
#define VAMS_1_3       (1 << 6)
#define VAMS_2_0       (1 << 7)
#define VAMS_2_1       (1 << 8)
#define VAMS_2_2       (1 << 9)
#define VAMS_2_3       (1 << 10)
#define VAMS_2_4       (1 << 11)
#line 8 "vams_keyword_lookup.gperf"
struct vams_keyword { const char *name; int languages; };

#define TOTAL_KEYWORDS 311
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 19
#define MIN_HASH_VALUE 33
#define MAX_HASH_VALUE 1261
/* maximum key range = 1229, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
vams_keyword_hash (str, len)
     register const char *str;
     register unsigned int len;
{
  static const unsigned short asso_values[] =
    {
      1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262,
      1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262,
      1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262,
      1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262,
      1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262,   15,    0,
         0, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262,
      1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262,
      1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262,
      1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262,
      1262, 1262, 1262, 1262, 1262,   10, 1262,   25,  500,  105,
        25,    0,  220,  420,  395,   30,    0,  110,  150,  185,
         0,   75,  140,   15,   55,   15,    5,  305,  300,  165,
       200,  420,  260, 1262, 1262, 1262, 1262, 1262, 1262, 1262,
      1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262,
      1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262,
      1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262,
      1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262,
      1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262,
      1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262,
      1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262,
      1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262,
      1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262,
      1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262,
      1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262,
      1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262, 1262,
      1262, 1262, 1262, 1262, 1262, 1262
    };
  register int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[8]];
      /*FALLTHROUGH*/
      case 8:
      case 7:
      case 6:
      case 5:
      case 4:
        hval += asso_values[(unsigned char)str[3]];
      /*FALLTHROUGH*/
      case 3:
        hval += asso_values[(unsigned char)str[2]];
      /*FALLTHROUGH*/
      case 2:
        hval += asso_values[(unsigned char)str[1]];
      /*FALLTHROUGH*/
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval + asso_values[(unsigned char)str[len - 1]];
}

#ifdef __GNUC__
__inline
#ifdef __GNUC_STDC_INLINE__
__attribute__ ((__gnu_inline__))
#endif
#endif
const struct vams_keyword *
vams_keyword_lookup (str, len)
     register const char *str;
     register unsigned int len;
{
  static const struct vams_keyword wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 283 "vams_keyword_lookup.gperf"
      {"tan", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 118 "vams_keyword_lookup.gperf"
      {"endnature", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""},
#line 126 "vams_keyword_lookup.gperf"
      {"endtable", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""},
#line 174 "vams_keyword_lookup.gperf"
      {"int", IEEE_1800_2005},
#line 176 "vams_keyword_lookup.gperf"
      {"interface", IEEE_1800_2005},
      {""}, {""}, {""},
#line 264 "vams_keyword_lookup.gperf"
      {"sin", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""},
#line 124 "vams_keyword_lookup.gperf"
      {"endsequence", IEEE_1800_2005},
      {""},
#line 106 "vams_keyword_lookup.gperf"
      {"end", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 177 "vams_keyword_lookup.gperf"
      {"intersect", IEEE_1800_2005},
      {""}, {""}, {""},
#line 173 "vams_keyword_lookup.gperf"
      {"instance", IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VAMS_2_3 | VAMS_2_4},
#line 47 "vams_keyword_lookup.gperf"
      {"atan", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 48 "vams_keyword_lookup.gperf"
      {"atan2", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""},
#line 88 "vams_keyword_lookup.gperf"
      {"ddt", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""},
#line 44 "vams_keyword_lookup.gperf"
      {"assert", IEEE_1800_2005 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 156 "vams_keyword_lookup.gperf"
      {"idt", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""},
#line 91 "vams_keyword_lookup.gperf"
      {"deassign", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 42 "vams_keyword_lookup.gperf"
      {"asin", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 95 "vams_keyword_lookup.gperf"
      {"design", IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 41 "vams_keyword_lookup.gperf"
      {"and", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 204 "vams_keyword_lookup.gperf"
      {"nand", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 172 "vams_keyword_lookup.gperf"
      {"inside", IEEE_1800_2005},
      {""}, {""},
#line 100 "vams_keyword_lookup.gperf"
      {"dist", IEEE_1800_2005},
      {""}, {""}, {""},
#line 214 "vams_keyword_lookup.gperf"
      {"not", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 293 "vams_keyword_lookup.gperf"
      {"tran", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 45 "vams_keyword_lookup.gperf"
      {"assign", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 295 "vams_keyword_lookup.gperf"
      {"tranif1", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 299 "vams_keyword_lookup.gperf"
      {"tri1", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""},
#line 175 "vams_keyword_lookup.gperf"
      {"integer", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 272 "vams_keyword_lookup.gperf"
      {"sqrt", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""},
#line 96 "vams_keyword_lookup.gperf"
      {"disable", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""},
#line 294 "vams_keyword_lookup.gperf"
      {"tranif0", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 178 "vams_keyword_lookup.gperf"
      {"join", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""},
#line 242 "vams_keyword_lookup.gperf"
      {"randcase", IEEE_1800_2005},
#line 180 "vams_keyword_lookup.gperf"
      {"join_none", IEEE_1800_2005},
      {""},
#line 216 "vams_keyword_lookup.gperf"
      {"notif1", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 243 "vams_keyword_lookup.gperf"
      {"randsequence", IEEE_1800_2005},
      {""}, {""}, {""}, {""}, {""},
#line 297 "vams_keyword_lookup.gperf"
      {"tri", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 298 "vams_keyword_lookup.gperf"
      {"tri0", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""}, {""},
#line 89 "vams_keyword_lookup.gperf"
      {"ddt_nature", VA_1_0 | VAMS_1_3 | VAMS_2_3 | VAMS_2_4},
#line 215 "vams_keyword_lookup.gperf"
      {"notif0", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""},
#line 240 "vams_keyword_lookup.gperf"
      {"rand", IEEE_1800_2005},
#line 157 "vams_keyword_lookup.gperf"
      {"idt_nature", VA_1_0 | VAMS_1_3 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 107 "vams_keyword_lookup.gperf"
      {"endcase", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 255 "vams_keyword_lookup.gperf"
      {"rtran", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 300 "vams_keyword_lookup.gperf"
      {"triand", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 127 "vams_keyword_lookup.gperf"
      {"endtask", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 257 "vams_keyword_lookup.gperf"
      {"rtranif1", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 64 "vams_keyword_lookup.gperf"
      {"case", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""},
#line 190 "vams_keyword_lookup.gperf"
      {"ln", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 108 "vams_keyword_lookup.gperf"
      {"endclass", IEEE_1800_2005},
      {""}, {""}, {""},
#line 276 "vams_keyword_lookup.gperf"
      {"strong1", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""},
#line 273 "vams_keyword_lookup.gperf"
      {"static", IEEE_1800_2005},
      {""},
#line 256 "vams_keyword_lookup.gperf"
      {"rtranif0", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""}, {""},
#line 105 "vams_keyword_lookup.gperf"
      {"else", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 296 "vams_keyword_lookup.gperf"
      {"transition", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 275 "vams_keyword_lookup.gperf"
      {"strong0", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""},
#line 101 "vams_keyword_lookup.gperf"
      {"do", IEEE_1800_2005},
      {""},
#line 207 "vams_keyword_lookup.gperf"
      {"net_resolution", VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""},
#line 121 "vams_keyword_lookup.gperf"
      {"endprimitive", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 99 "vams_keyword_lookup.gperf"
      {"discrete", VAMS_2_3 | VAMS_2_4},
      {""},
#line 97 "vams_keyword_lookup.gperf"
      {"discipline", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 218 "vams_keyword_lookup.gperf"
      {"or", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 212 "vams_keyword_lookup.gperf"
      {"nor", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""},
#line 74 "vams_keyword_lookup.gperf"
      {"connect", VAMS_2_3 | VAMS_2_4},
#line 80 "vams_keyword_lookup.gperf"
      {"continue", IEEE_1800_2005},
      {""}, {""},
#line 120 "vams_keyword_lookup.gperf"
      {"endparamset", VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 79 "vams_keyword_lookup.gperf"
      {"context", IEEE_1800_2005},
      {""}, {""}, {""}, {""}, {""},
#line 112 "vams_keyword_lookup.gperf"
      {"enddiscipline", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 77 "vams_keyword_lookup.gperf"
      {"const", IEEE_1800_2005},
#line 250 "vams_keyword_lookup.gperf"
      {"repeat", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""},
#line 78 "vams_keyword_lookup.gperf"
      {"constraint", IEEE_1800_2005},
#line 135 "vams_keyword_lookup.gperf"
      {"extern", IEEE_1800_2005},
#line 249 "vams_keyword_lookup.gperf"
      {"release", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 82 "vams_keyword_lookup.gperf"
      {"cos", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 241 "vams_keyword_lookup.gperf"
      {"randc", IEEE_1800_2005},
      {""}, {""},
#line 201 "vams_keyword_lookup.gperf"
      {"min", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 117 "vams_keyword_lookup.gperf"
      {"endmodule", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 165 "vams_keyword_lookup.gperf"
      {"incdir", IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VAMS_2_3 | VAMS_2_4},
#line 168 "vams_keyword_lookup.gperf"
      {"initial", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 40 "vams_keyword_lookup.gperf"
      {"analysis", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 289 "vams_keyword_lookup.gperf"
      {"time", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 301 "vams_keyword_lookup.gperf"
      {"trior", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 134 "vams_keyword_lookup.gperf"
      {"extends", IEEE_1800_2005},
      {""}, {""}, {""}, {""},
#line 169 "vams_keyword_lookup.gperf"
      {"initial_step", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 292 "vams_keyword_lookup.gperf"
      {"timeunit", IEEE_1800_2005},
#line 316 "vams_keyword_lookup.gperf"
      {"wait", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""},
#line 225 "vams_keyword_lookup.gperf"
      {"posedge", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 246 "vams_keyword_lookup.gperf"
      {"realtime", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 31 "vams_keyword_lookup.gperf"
      {"acos", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""},
#line 318 "vams_keyword_lookup.gperf"
      {"wand", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""}, {""},
#line 33 "vams_keyword_lookup.gperf"
      {"alias", IEEE_1800_2005},
      {""}, {""}, {""},
#line 324 "vams_keyword_lookup.gperf"
      {"wire", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 30 "vams_keyword_lookup.gperf"
      {"access", VA_1_0 | VAMS_1_3 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 223 "vams_keyword_lookup.gperf"
      {"paramset", VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""},
#line 290 "vams_keyword_lookup.gperf"
      {"timeprecision", IEEE_1800_2005},
      {""},
#line 111 "vams_keyword_lookup.gperf"
      {"endconnectrules", VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""},
#line 75 "vams_keyword_lookup.gperf"
      {"connectmodule", VAMS_2_3 | VAMS_2_4},
#line 285 "vams_keyword_lookup.gperf"
      {"task", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 87 "vams_keyword_lookup.gperf"
      {"cross", IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""}, {""},
#line 158 "vams_keyword_lookup.gperf"
      {"idtmod", VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""},
#line 291 "vams_keyword_lookup.gperf"
      {"timer", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 92 "vams_keyword_lookup.gperf"
      {"default", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""},
#line 113 "vams_keyword_lookup.gperf"
      {"endfunction", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 116 "vams_keyword_lookup.gperf"
      {"endinterface", IEEE_1800_2005},
      {""}, {""},
#line 317 "vams_keyword_lookup.gperf"
      {"wait_order", IEEE_1800_2005},
      {""},
#line 166 "vams_keyword_lookup.gperf"
      {"include", IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 209 "vams_keyword_lookup.gperf"
      {"nmos", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 251 "vams_keyword_lookup.gperf"
      {"resolveto", VAMS_2_3 | VAMS_2_4},
#line 320 "vams_keyword_lookup.gperf"
      {"weak1", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""},
#line 129 "vams_keyword_lookup.gperf"
      {"event", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 315 "vams_keyword_lookup.gperf"
      {"vt", VA_1_0 | VAMS_1_3},
      {""}, {""},
#line 70 "vams_keyword_lookup.gperf"
      {"class", IEEE_1800_2005},
#line 102 "vams_keyword_lookup.gperf"
      {"domain", VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""},
#line 319 "vams_keyword_lookup.gperf"
      {"weak0", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""},
#line 309 "vams_keyword_lookup.gperf"
      {"use", IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""},
#line 258 "vams_keyword_lookup.gperf"
      {"scalared", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 331 "vams_keyword_lookup.gperf"
      {"zi_nd", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 161 "vams_keyword_lookup.gperf"
      {"ifnone", IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 208 "vams_keyword_lookup.gperf"
      {"new", IEEE_1800_2005},
#line 232 "vams_keyword_lookup.gperf"
      {"protected", IEEE_1800_2005},
#line 253 "vams_keyword_lookup.gperf"
      {"rnmos", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""}, {""},
#line 205 "vams_keyword_lookup.gperf"
      {"nature", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 197 "vams_keyword_lookup.gperf"
      {"matches", IEEE_1800_2005},
#line 259 "vams_keyword_lookup.gperf"
      {"sequence", IEEE_1800_2005},
      {""},
#line 271 "vams_keyword_lookup.gperf"
      {"split", VAMS_2_3 | VAMS_2_4},
      {""},
#line 29 "vams_keyword_lookup.gperf"
      {"ac_stim", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""},
#line 65 "vams_keyword_lookup.gperf"
      {"casex", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 132 "vams_keyword_lookup.gperf"
      {"expect", IEEE_1800_2005},
      {""},
#line 327 "vams_keyword_lookup.gperf"
      {"wor", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""},
#line 306 "vams_keyword_lookup.gperf"
      {"unique", IEEE_1800_2005},
      {""}, {""}, {""},
#line 307 "vams_keyword_lookup.gperf"
      {"units", VA_1_0 | VAMS_1_3 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""},
#line 222 "vams_keyword_lookup.gperf"
      {"parameter", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 46 "vams_keyword_lookup.gperf"
      {"assume", IEEE_1800_2005},
      {""}, {""}, {""}, {""},
#line 252 "vams_keyword_lookup.gperf"
      {"return", IEEE_1800_2005},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 308 "vams_keyword_lookup.gperf"
      {"unsigned", IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VAMS_2_3 | VAMS_2_4},
#line 245 "vams_keyword_lookup.gperf"
      {"real", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 122 "vams_keyword_lookup.gperf"
      {"endprogram", IEEE_1800_2005},
#line 35 "vams_keyword_lookup.gperf"
      {"always", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 220 "vams_keyword_lookup.gperf"
      {"package", IEEE_1800_2005},
#line 330 "vams_keyword_lookup.gperf"
      {"xor", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 329 "vams_keyword_lookup.gperf"
      {"xnor", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 277 "vams_keyword_lookup.gperf"
      {"struct", IEEE_1800_2005},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 72 "vams_keyword_lookup.gperf"
      {"cmos", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 328 "vams_keyword_lookup.gperf"
      {"wreal", VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""},
#line 323 "vams_keyword_lookup.gperf"
      {"wildcard", IEEE_1800_2005},
      {""}, {""}, {""}, {""},
#line 142 "vams_keyword_lookup.gperf"
      {"for", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 66 "vams_keyword_lookup.gperf"
      {"casez", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 221 "vams_keyword_lookup.gperf"
      {"packed", IEEE_1800_2005},
#line 145 "vams_keyword_lookup.gperf"
      {"forever", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""},
#line 305 "vams_keyword_lookup.gperf"
      {"union", IEEE_1800_2005},
      {""}, {""}, {""},
#line 228 "vams_keyword_lookup.gperf"
      {"primitive", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 170 "vams_keyword_lookup.gperf"
      {"inout", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""},
#line 137 "vams_keyword_lookup.gperf"
      {"final_step", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 133 "vams_keyword_lookup.gperf"
      {"export", IEEE_1800_2005},
#line 206 "vams_keyword_lookup.gperf"
      {"negedge", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 150 "vams_keyword_lookup.gperf"
      {"generate", IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 136 "vams_keyword_lookup.gperf"
      {"final", IEEE_1800_2005},
#line 199 "vams_keyword_lookup.gperf"
      {"medium", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""},
#line 224 "vams_keyword_lookup.gperf"
      {"pmos", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""},
#line 202 "vams_keyword_lookup.gperf"
      {"modport", IEEE_1800_2005},
#line 311 "vams_keyword_lookup.gperf"
      {"var", IEEE_1800_2005},
#line 67 "vams_keyword_lookup.gperf"
      {"ceil", VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 244 "vams_keyword_lookup.gperf"
      {"rcmos", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 164 "vams_keyword_lookup.gperf"
      {"import", IEEE_1800_2005},
      {""},
#line 312 "vams_keyword_lookup.gperf"
      {"vectored", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 332 "vams_keyword_lookup.gperf"
      {"zi_np", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""},
#line 104 "vams_keyword_lookup.gperf"
      {"edge", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 34 "vams_keyword_lookup.gperf"
      {"aliasparam", VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""},
#line 90 "vams_keyword_lookup.gperf"
      {"ddx", VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 49 "vams_keyword_lookup.gperf"
      {"atanh", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""},
#line 314 "vams_keyword_lookup.gperf"
      {"void", IEEE_1800_2005},
#line 143 "vams_keyword_lookup.gperf"
      {"force", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 130 "vams_keyword_lookup.gperf"
      {"exclude", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 287 "vams_keyword_lookup.gperf"
      {"this", IEEE_1800_2005},
      {""}, {""}, {""},
#line 147 "vams_keyword_lookup.gperf"
      {"forkjoin", IEEE_1800_2005},
      {""},
#line 43 "vams_keyword_lookup.gperf"
      {"asinh", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 159 "vams_keyword_lookup.gperf"
      {"if", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 167 "vams_keyword_lookup.gperf"
      {"inf", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 254 "vams_keyword_lookup.gperf"
      {"rpmos", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""}, {""},
#line 114 "vams_keyword_lookup.gperf"
      {"endgenerate", IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 131 "vams_keyword_lookup.gperf"
      {"exp", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 171 "vams_keyword_lookup.gperf"
      {"input", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 86 "vams_keyword_lookup.gperf"
      {"coverpoint", IEEE_1800_2005},
#line 263 "vams_keyword_lookup.gperf"
      {"signed", IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 247 "vams_keyword_lookup.gperf"
      {"ref", IEEE_1800_2005},
#line 266 "vams_keyword_lookup.gperf"
      {"slew", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 181 "vams_keyword_lookup.gperf"
      {"laplace_nd", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""},
#line 239 "vams_keyword_lookup.gperf"
      {"pure", IEEE_1800_2005},
      {""}, {""}, {""}, {""}, {""},
#line 191 "vams_keyword_lookup.gperf"
      {"local", IEEE_1800_2005},
#line 189 "vams_keyword_lookup.gperf"
      {"limexp", VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 76 "vams_keyword_lookup.gperf"
      {"connectrules", VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""},
#line 81 "vams_keyword_lookup.gperf"
      {"continuous", VAMS_2_3 | VAMS_2_4},
      {""}, {""},
#line 139 "vams_keyword_lookup.gperf"
      {"flicker_noise", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 278 "vams_keyword_lookup.gperf"
      {"super", IEEE_1800_2005},
      {""}, {""}, {""}, {""},
#line 213 "vams_keyword_lookup.gperf"
      {"noshowcancelled", IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""},
#line 226 "vams_keyword_lookup.gperf"
      {"potential", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 267 "vams_keyword_lookup.gperf"
      {"small", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 274 "vams_keyword_lookup.gperf"
      {"string", IEEE_1800_2005 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 69 "vams_keyword_lookup.gperf"
      {"chandle", IEEE_1800_2005},
#line 179 "vams_keyword_lookup.gperf"
      {"join_any", IEEE_1800_2005},
      {""}, {""},
#line 219 "vams_keyword_lookup.gperf"
      {"output", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""},
#line 84 "vams_keyword_lookup.gperf"
      {"cover", IEEE_1800_2005},
      {""}, {""},
#line 56 "vams_keyword_lookup.gperf"
      {"bit", IEEE_1800_2005},
      {""},
#line 268 "vams_keyword_lookup.gperf"
      {"solve", IEEE_1800_2005},
      {""},
#line 313 "vams_keyword_lookup.gperf"
      {"virtual", IEEE_1800_2005},
#line 227 "vams_keyword_lookup.gperf"
      {"pow", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""},
#line 260 "vams_keyword_lookup.gperf"
      {"shortint", IEEE_1800_2005},
      {""}, {""}, {""}, {""},
#line 25 "vams_keyword_lookup.gperf"
      {"abs", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 68 "vams_keyword_lookup.gperf"
      {"cell", IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VAMS_2_3 | VAMS_2_4},
#line 310 "vams_keyword_lookup.gperf"
      {"uwire", IEEE_1364_2005 | IEEE_1800_2005 | VAMS_2_3 | VAMS_2_4},
      {""}, {""},
#line 103 "vams_keyword_lookup.gperf"
      {"driver_update", VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 54 "vams_keyword_lookup.gperf"
      {"bins", IEEE_1800_2005},
      {""}, {""}, {""}, {""},
#line 303 "vams_keyword_lookup.gperf"
      {"type", IEEE_1800_2005},
      {""},
#line 302 "vams_keyword_lookup.gperf"
      {"trireg", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""},
#line 146 "vams_keyword_lookup.gperf"
      {"fork", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 192 "vams_keyword_lookup.gperf"
      {"localparam", IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""},
#line 93 "vams_keyword_lookup.gperf"
      {"defparam", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 140 "vams_keyword_lookup.gperf"
      {"floor", VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 162 "vams_keyword_lookup.gperf"
      {"ignore_bins", IEEE_1800_2005},
      {""}, {""},
#line 53 "vams_keyword_lookup.gperf"
      {"bind", IEEE_1800_2005},
      {""}, {""}, {""}, {""}, {""},
#line 333 "vams_keyword_lookup.gperf"
      {"zi_zd", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 109 "vams_keyword_lookup.gperf"
      {"endclocking", IEEE_1800_2005},
      {""},
#line 115 "vams_keyword_lookup.gperf"
      {"endgroup", IEEE_1800_2005},
      {""},
#line 119 "vams_keyword_lookup.gperf"
      {"endpackage", IEEE_1800_2005},
#line 203 "vams_keyword_lookup.gperf"
      {"module", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 27 "vams_keyword_lookup.gperf"
      {"absdelta", VAMS_2_4},
      {""}, {""},
#line 326 "vams_keyword_lookup.gperf"
      {"within", IEEE_1800_2005},
      {""}, {""}, {""}, {""}, {""},
#line 280 "vams_keyword_lookup.gperf"
      {"supply1", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 98 "vams_keyword_lookup.gperf"
      {"discontinuity", VA_1_0 | VAMS_1_3},
      {""}, {""}, {""}, {""},
#line 198 "vams_keyword_lookup.gperf"
      {"max", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 182 "vams_keyword_lookup.gperf"
      {"laplace_np", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""},
#line 32 "vams_keyword_lookup.gperf"
      {"acosh", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 279 "vams_keyword_lookup.gperf"
      {"supply0", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""},
#line 94 "vams_keyword_lookup.gperf"
      {"delay", VA_1_0 | VAMS_1_3},
#line 39 "vams_keyword_lookup.gperf"
      {"analog", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""},
#line 50 "vams_keyword_lookup.gperf"
      {"automatic", IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 210 "vams_keyword_lookup.gperf"
      {"noise_table", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""},
#line 322 "vams_keyword_lookup.gperf"
      {"white_noise", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 149 "vams_keyword_lookup.gperf"
      {"function", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 270 "vams_keyword_lookup.gperf"
      {"specparam", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""},
#line 186 "vams_keyword_lookup.gperf"
      {"last_crossing", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""},
#line 286 "vams_keyword_lookup.gperf"
      {"temperature", VA_1_0 | VAMS_1_3},
      {""}, {""}, {""}, {""},
#line 123 "vams_keyword_lookup.gperf"
      {"endproperty", IEEE_1800_2005},
      {""}, {""}, {""},
#line 185 "vams_keyword_lookup.gperf"
      {"large", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 195 "vams_keyword_lookup.gperf"
      {"longint", IEEE_1800_2005},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 128 "vams_keyword_lookup.gperf"
      {"enum", IEEE_1800_2005},
      {""}, {""}, {""}, {""}, {""},
#line 281 "vams_keyword_lookup.gperf"
      {"table", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 196 "vams_keyword_lookup.gperf"
      {"macromodule", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 269 "vams_keyword_lookup.gperf"
      {"specify", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 262 "vams_keyword_lookup.gperf"
      {"showcancelled", IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 125 "vams_keyword_lookup.gperf"
      {"endspecify", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 200 "vams_keyword_lookup.gperf"
      {"merged", VAMS_2_3 | VAMS_2_4},
      {""},
#line 160 "vams_keyword_lookup.gperf"
      {"iff", IEEE_1800_2005},
      {""},
#line 59 "vams_keyword_lookup.gperf"
      {"break", IEEE_1800_2005},
      {""}, {""}, {""}, {""}, {""},
#line 28 "vams_keyword_lookup.gperf"
      {"abstol", VA_1_0 | VAMS_1_3 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""},
#line 334 "vams_keyword_lookup.gperf"
      {"zi_zp", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 148 "vams_keyword_lookup.gperf"
      {"from", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""},
#line 229 "vams_keyword_lookup.gperf"
      {"priority", IEEE_1800_2005},
      {""}, {""},
#line 138 "vams_keyword_lookup.gperf"
      {"first_match", IEEE_1800_2005},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 321 "vams_keyword_lookup.gperf"
      {"while", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""},
#line 234 "vams_keyword_lookup.gperf"
      {"pull1", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 144 "vams_keyword_lookup.gperf"
      {"foreach", IEEE_1800_2005},
#line 235 "vams_keyword_lookup.gperf"
      {"pulldown", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""}, {""},
#line 217 "vams_keyword_lookup.gperf"
      {"null", IEEE_1800_2005},
#line 183 "vams_keyword_lookup.gperf"
      {"laplace_zd", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""},
#line 233 "vams_keyword_lookup.gperf"
      {"pull0", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""}, {""},
#line 55 "vams_keyword_lookup.gperf"
      {"binsof", IEEE_1800_2005},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 141 "vams_keyword_lookup.gperf"
      {"flow", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 151 "vams_keyword_lookup.gperf"
      {"genvar", IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 238 "vams_keyword_lookup.gperf"
      {"pulsestyle_onevent", IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VAMS_2_3 | VAMS_2_4},
#line 237 "vams_keyword_lookup.gperf"
      {"pulsestyle_ondetect", IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VAMS_2_3 | VAMS_2_4},
#line 194 "vams_keyword_lookup.gperf"
      {"logic", IEEE_1800_2005},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 304 "vams_keyword_lookup.gperf"
      {"typedef", IEEE_1800_2005},
      {""}, {""}, {""}, {""},
#line 38 "vams_keyword_lookup.gperf"
      {"always_latch", IEEE_1800_2005},
      {""}, {""}, {""},
#line 51 "vams_keyword_lookup.gperf"
      {"before", IEEE_1800_2005},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 37 "vams_keyword_lookup.gperf"
      {"always_ff", IEEE_1800_2005},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 284 "vams_keyword_lookup.gperf"
      {"tanh", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 73 "vams_keyword_lookup.gperf"
      {"config", IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 231 "vams_keyword_lookup.gperf"
      {"property", IEEE_1800_2005},
#line 265 "vams_keyword_lookup.gperf"
      {"sinh", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""},
#line 187 "vams_keyword_lookup.gperf"
      {"liblist", IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 261 "vams_keyword_lookup.gperf"
      {"shortreal", IEEE_1800_2005},
#line 288 "vams_keyword_lookup.gperf"
      {"throughout", IEEE_1800_2005},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 163 "vams_keyword_lookup.gperf"
      {"illegal_bins", IEEE_1800_2005},
      {""}, {""}, {""}, {""}, {""},
#line 71 "vams_keyword_lookup.gperf"
      {"clocking", IEEE_1800_2005},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 184 "vams_keyword_lookup.gperf"
      {"laplace_zp", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 230 "vams_keyword_lookup.gperf"
      {"program", IEEE_1800_2005},
      {""}, {""}, {""},
#line 152 "vams_keyword_lookup.gperf"
      {"ground", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""},
#line 236 "vams_keyword_lookup.gperf"
      {"pullup", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 248 "vams_keyword_lookup.gperf"
      {"reg", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""},
#line 282 "vams_keyword_lookup.gperf"
      {"tagged", IEEE_1800_2005},
      {""}, {""}, {""},
#line 24 "vams_keyword_lookup.gperf"
      {"above", VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 63 "vams_keyword_lookup.gperf"
      {"byte", IEEE_1800_2005},
      {""}, {""}, {""}, {""}, {""},
#line 85 "vams_keyword_lookup.gperf"
      {"covergroup", IEEE_1800_2005},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 36 "vams_keyword_lookup.gperf"
      {"always_comb", IEEE_1800_2005},
      {""}, {""}, {""},
#line 52 "vams_keyword_lookup.gperf"
      {"begin", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 110 "vams_keyword_lookup.gperf"
      {"endconfig", IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 58 "vams_keyword_lookup.gperf"
      {"branch", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 83 "vams_keyword_lookup.gperf"
      {"cosh", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""},
#line 26 "vams_keyword_lookup.gperf"
      {"absdelay", VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
#line 325 "vams_keyword_lookup.gperf"
      {"with", IEEE_1800_2005},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 57 "vams_keyword_lookup.gperf"
      {"bound_step", VA_1_0 | VAMS_1_3},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 155 "vams_keyword_lookup.gperf"
      {"hypot", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 211 "vams_keyword_lookup.gperf"
      {"noise_table_log", VAMS_2_4},
      {""}, {""}, {""}, {""}, {""},
#line 62 "vams_keyword_lookup.gperf"
      {"bufif1", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 193 "vams_keyword_lookup.gperf"
      {"log", VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 61 "vams_keyword_lookup.gperf"
      {"bufif0", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 188 "vams_keyword_lookup.gperf"
      {"library", IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 154 "vams_keyword_lookup.gperf"
      {"highz1", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""},
#line 60 "vams_keyword_lookup.gperf"
      {"buf", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 153 "vams_keyword_lookup.gperf"
      {"highz0", IEEE_1364_1995 | IEEE_1364_2001 | IEEE_1364_2005 | IEEE_1800_2005 | VA_1_0 | VAMS_1_3 | VAMS_2_0 | VAMS_2_1 | VAMS_2_2 | VAMS_2_3 | VAMS_2_4}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = vams_keyword_hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strncmp (str + 1, s + 1, len - 1) && s[len] == '\0')
            return &wordlist[key];
        }
    }
  return 0;
}
#line 335 "vams_keyword_lookup.gperf"

