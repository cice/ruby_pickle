#include "util.h"

// uchar read_one(VALUE stream) {
//   VALUE byte = rb_funcall(stream, meth_read, 0);
//   
//   return NUM2CHR(byte);
// }
// 
// VALUE read_to_nl(VALUE stream) {
//   VALUE string = rb_ary_new();
//   
//   uchar buf = read_one(stream);
//   
//   while(buf != P_NEWLINE) {
//     rb_ary_push(string, CHR2FIX(buf));
//     buf = read_one(stream);
//   }
//   
//   string = rb_funcall(string, meth_pack, 1, rb_str_new2("U*"));
//   return string;
// }
// 
// int eval_octal(uchar buf1, uchar buf2, uchar buf3) {
//   int i1 = buf1 - 48;
//   int i2 = buf2 - 48;
//   int i3 = buf3 - 48;
//   
//   return i3 + i2 * 8 + i1 * 64;
// }
// 
// int eval_hex(uchar buf1, uchar buf2) {
//   buf1 = tolower(buf1);
//   buf2 = tolower(buf2);
//   
//   int i1 = buf1 - 48;
//   if(isalpha(buf1))
//     i1 = i1 - 39;
//   int i2 = buf2 - 48;
//   if(isalpha(buf2))
//     i2 = i2 - 39;
//     
//   return i2 + i1*16;
// }
// 
// uchar unescape(uchar buf, VALUE stream) {
//   uchar buf1, buf2;
//   switch (buf) {
//  case 'a':       /* \a -> audible bell */
//    return '\a';
//    break;
//   case 'b':        /* \b -> backspace */
//    return '\b';
//    break;
//  case 'f':       /* \f -> formfeed */
//    return '\f';
//    break;
//  case 'n':       /* \n -> newline */
//    return '\n';
//    break;
//  case 'r':       /* \r -> carriagereturn */
//    return '\r';
//    break;
//  case 't':       /* \t -> horizontal tab */
//    return '\t';
//    break;
//  case 'v':       /* \v -> vertical tab */
//    return '\v';
//    break;
//   case '0':        /* \nnn -> ASCII value */
//   case '1':
//   case '2':
//   case '3':
//   case '4':
//   case '5':
//   case '6':
//   case '7':
//     buf1 = read_one(stream);
//     buf2 = read_one(stream);
//     return eval_octal(buf, buf1, buf2);
//    break;
//   case 'x':
//     buf1 = read_one(stream);
//     buf2 = read_one(stream);
//     return eval_hex(buf1, buf2);
//     break;
//   default:
//     return buf;
//   }
// }
// 
// VALUE read_to_nl_and_unescape(VALUE stream) {
//   VALUE string = rb_str_new2("");
//   
//   uchar buf = read_one(stream);
//   uchar buf2, buf3;
//   
//   while(buf != P_NEWLINE) {
//     if(buf == P_ESCAPE) {
//       buf2 = read_one(stream);
//       buf3 = unescape(buf2, stream);
//       if(buf2 == buf3) {
//         rb_str_cat(string, &buf, 1);
//         rb_str_cat(string, &buf2, 1);
//       } else {
//         rb_str_cat(string, &buf3, 1);
//       }
//     } else {
//       rb_str_cat(string, &buf, 1);
//     }
//     buf = read_one(stream);
//   }
//   
//   return string;
// }
// 
// VALUE to_i(VALUE string) {
//   return rb_funcall(string, meth_to_i, 0);
// }
// 
// VALUE to_f(VALUE string) {
//   return rb_funcall(string, meth_to_f, 0);
// }
