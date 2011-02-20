#include "ruby_cpickle.h"
#include <ruby.h> 
#include <stdio.h>
#include "util.h"

// Array to hold function pointers to all operations
static void(*dispatch[256])(VirtualMachine*, VALUE) = {};

// Hold a global Marker
static VALUE Marker;

uchar read_one(VALUE stream) {
  VALUE byte = rb_funcall(stream, meth_read, 0);
  
  return NUM2CHR(byte);
}

VALUE pack_to_utf8(VALUE ary) {
  VALUE unicode = rb_str_new2("U*");
  return rb_funcall(ary, meth_pack, 1, unicode);
}

VALUE read_to_nl(VALUE stream) {
  VALUE string = rb_ary_new();
  
  uchar buf = read_one(stream);
  
  while(buf != P_NEWLINE) {
    rb_ary_push(string, CHR2FIX(buf));
    buf = read_one(stream);
  }
  
  string = pack_to_utf8(string);
  return string;
}

int eval_octal(uchar buf1, uchar buf2, uchar buf3) {
  int i1 = buf1 - 48;
  int i2 = buf2 - 48;
  int i3 = buf3 - 48;
  
  return i3 + i2 * 8 + i1 * 64;
}

int eval_hex(uchar buf1, uchar buf2) {
  buf1 = tolower(buf1);
  buf2 = tolower(buf2);
  
  int i1 = buf1 - 48;
  if(isalpha(buf1))
    i1 = i1 - 39;
  int i2 = buf2 - 48;
  if(isalpha(buf2))
    i2 = i2 - 39;
    
  return i2 + i1*16;
}

uchar unescape(uchar buf, VALUE stream) {
  uchar buf1, buf2;
  switch (buf) {
	case 'a':				/* \a -> audible bell */
		return '\a';
		break;
  case 'b':				/* \b -> backspace */
		return '\b';
		break;
	case 'f':				/* \f -> formfeed */
		return '\f';
		break;
	case 'n':				/* \n -> newline */
		return '\n';
		break;
	case 'r':				/* \r -> carriagereturn */
		return '\r';
		break;
	case 't':				/* \t -> horizontal tab */
		return '\t';
		break;
	case 'v':				/* \v -> vertical tab */
		return '\v';
		break;
  case '0':				/* \nnn -> ASCII value */
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
    buf1 = read_one(stream);
    buf2 = read_one(stream);
    return eval_octal(buf, buf1, buf2);
		break;
  case 'x':
    buf1 = read_one(stream);
    buf2 = read_one(stream);
    return eval_hex(buf1, buf2);
    break;
  default:
    return buf;
  }
}
void handle_escaping(VALUE stream, VALUE string) {
  uchar buf2, buf3;
  
  buf2 = read_one(stream);
  buf3 = unescape(buf2, stream);

  if(buf2 == buf3) {
    uchar escape = P_ESCAPE;
    rb_str_cat(string, &escape, 1);
    rb_str_cat(string, &buf2, 1);
  } else {
    rb_str_cat(string, &buf3, 1);
  }
}

VALUE read_to_nl_and_unescape(VALUE stream) {
  VALUE string = rb_str_new2("");
  
  uchar buf = read_one(stream);

  while(buf != P_NEWLINE) {
    if(buf == P_ESCAPE) {
      handle_escaping(stream, string);
    } else {
      rb_str_cat(string, &buf, 1);
    }
    buf = read_one(stream);
  }
  
  return string;
}

VALUE to_i(VALUE string) {
  return rb_funcall(string, meth_to_i, 0);
}

VALUE to_f(VALUE string) {
  return rb_funcall(string, meth_to_f, 0);
}


// Dispatch pickle operations to C functions. Uses 'dispatch' as map
// between operation and function pointer
static void dispatcher(VirtualMachine *vm, uchar op_code, VALUE stream) {
  void (*func)(VirtualMachine*, VALUE) = dispatch[op_code];
  
  if(func != NULL) {
    (*func)(vm, stream);
  } else {
    rb_raise(rb_eRuntimeError, "Missing Operation \"%c\"", op_code);
  }
}

static VALUE reader(VALUE self) {
  VALUE stream = rb_iv_get(self, "@stream");
  rb_funcall(stream, meth_rewind, 0);
  
  VirtualMachine *vm = ALLOC(VirtualMachine);
  init_vm(vm);
  
  uchar op_code = read_one(stream);
  while(op_code != P_STOP) {
    dispatcher(vm, op_code, stream);
    op_code = read_one(stream);
  }
  VALUE retval = vmpop(vm);
  
  // free_vm(vm);
  return retval;
}

static void load_int(VirtualMachine *vm, VALUE stream) {
  VALUE string = read_to_nl(stream);

  int len = RSTRING(string)->len;
  if(len == 2) {
    char *str = RSTRING(string)->ptr;
    if(strcmp(str, P_TRUE) == 0) {
      vmpush(vm, Qtrue);
      return;
    }
    if(strcmp(str, P_FALSE) == 0) {
      vmpush(vm, Qfalse);
      return;
    }
  }
  vmpush(vm, to_i(string));
}

static void load_string(VirtualMachine *vm, VALUE stream) {
  VALUE string = read_to_nl_and_unescape(stream);
  int length = RSTRING(string)->len - 2;
  string = rb_funcall(string, meth_slice, 2, INT2FIX(1), INT2FIX(length));

  vmpush(vm, string);
}

static void load_float(VirtualMachine *vm, VALUE stream) {
  VALUE string = read_to_nl(stream);
  
  vmpush(vm, to_f(string));
}

static void put_memo(VirtualMachine *vm, VALUE stream) {
  VALUE string = read_to_nl(stream);
  int pos = NUM2INT(to_i(string));
  
  vmput(vm, pos);
}

static void get_memo(VirtualMachine *vm, VALUE stream) {
  VALUE string = read_to_nl(stream);
  int pos = NUM2INT(to_i(string));

  vmpush(vm, vmget(vm, pos));
}

static void load_dict(VirtualMachine *vm, VALUE stream) {
  VALUE dict = rb_hash_new();
  
  VALUE val = vmpop(vm);
  VALUE key;
  
  if(val != Marker) {
    key = vmpop(vm);
    while(key != Marker) {
      rb_hash_aset(dict, key, val);
    
      val = vmpop(vm);
      if(val == Marker)
        break;
      key = vmpop(vm);
    }
  }
  
  vmpush(vm, dict);
}

static void mark_stack(VirtualMachine *vm, VALUE stream) {
  vmpush(vm, Marker);
}

static void load_list(VirtualMachine *vm, VALUE stream) {
  VALUE list = rb_ary_new();
  
  VALUE element = vmpop(vm);
  while(element != Marker) {
    rb_ary_unshift(list, element);
    element = vmpop(vm);
  }
  
  vmpush(vm, list);
}

static void append(VirtualMachine *vm, VALUE stream) {
  VALUE element = vmpop(vm);
  VALUE list = vmpeek(vm);
  
  rb_ary_push(list, element);
}

static void load_empty_list(VirtualMachine *vm, VALUE stream) {
  vmpush(vm, rb_ary_new());
}

static void appends(VirtualMachine *vm, VALUE stream) {
  VALUE element = vmpop(vm);
  VALUE list = rb_ary_new();
  
  while(element != Marker) {
    rb_ary_unshift(list, element);
    element = vmpop(vm);
  }
  
  VALUE list_on_stack = vmpeek(vm);
  
  int i;
  int len = RARRAY(list)->len;
  VALUE *elements = RARRAY(list)->ptr;
  for(i = 0; i < len; i++) {
    rb_ary_push(list_on_stack, elements[i]);
  }
}

static void pop_stack(VirtualMachine *vm, VALUE stream) {
  vmpop(vm);
}

static void set_item(VirtualMachine *vm, VALUE stream) {
  VALUE val = vmpop(vm);
  VALUE key = vmpop(vm);
  VALUE dict = vmpeek(vm);
  rb_hash_aset(dict, key, val);
}

static void set_items(VirtualMachine *vm, VALUE stream) {
  VALUE list = rb_ary_new();
  
  VALUE element = vmpop(vm);
  while(element != Marker) {
    rb_ary_unshift(list, element);
    element = vmpop(vm);
  }
  
  VALUE dict = vmpeek(vm);
  
  int i;
  VALUE key, val;
  int len = RARRAY(list)->len;
  VALUE *elements = RARRAY(list)->ptr;
  for(i = 0; i < len; i += 2) {
    key = elements[i];
    val = elements[i+1];
    rb_hash_aset(dict, key, val);
  }
}

static void load_empty_dict(VirtualMachine *vm, VALUE stream) {
  vmpush(vm, rb_hash_new());
}

static void load_tuple1(VirtualMachine *vm, VALUE stream) {
  printf("HERE");
  VALUE element = vmpop(vm);
  
  VALUE list = rb_ary_new3(1, element);
  vmpush(vm, list);
}

static void load_tuple2(VirtualMachine *vm, VALUE stream) {
  VALUE element2 = vmpop(vm);
  VALUE element1 = vmpop(vm);
  
  VALUE list = rb_ary_new3(2, element1, element2);
  vmpush(vm, list);
}

static void load_tuple3(VirtualMachine *vm, VALUE stream) {
  VALUE element3 = vmpop(vm);
  VALUE element2 = vmpop(vm);
  VALUE element1 = vmpop(vm);
  
  VALUE list = rb_ary_new3(3, element1, element2, element3);
  vmpush(vm, list);
}

static void load_binint(VirtualMachine *vm, VALUE stream) {
  uchar i1 = read_one(stream);
  uchar i2 = read_one(stream);
  uchar i3 = read_one(stream);
  uchar i4 = read_one(stream);
  int i = i1 + i2*256 + i3*256*256 + i4*256*256*256;
  
  vmpush(vm, INT2NUM(i));
}

static void load_binint1(VirtualMachine *vm, VALUE stream) {
  int i = read_one(stream);
  
  vmpush(vm, INT2NUM(i));
}

static void load_binint2(VirtualMachine *vm, VALUE stream) {
  uchar i1 = read_one(stream);
  uchar i2 = read_one(stream);
  int i = i1 + i2*256;
  
  vmpush(vm, INT2NUM(i));
}

static void load_none(VirtualMachine *vm, VALUE stream) {
  vmpush(vm, Qnil);
}

static void load_binstring(VirtualMachine *vm, VALUE stream) {
  uchar i1 = read_one(stream);
  uchar i2 = read_one(stream);
  uchar i3 = read_one(stream);
  uchar i4 = read_one(stream);
  int len = i1 + i2*256 + i3*256*256 + i4*256*256*256;

  VALUE string = rb_str_new(NULL, len);
  uchar* chars = RSTRING(string)->ptr;
  int i;
  uchar buf;
  
  for(i = 0; i < len; i++) {
    buf = read_one(stream);
    chars[i] = buf;
  }
  
  vmpush(vm, string);
}

static void load_short_binstring(VirtualMachine *vm, VALUE stream) {
  int len = read_one(stream);

  VALUE string = rb_str_new(NULL, len);
  uchar* chars = RSTRING(string)->ptr;
  int i;
  uchar buf;
  
  for(i = 0; i < len; i++) {
    buf = read_one(stream);
    chars[i] = buf;
  }
  
  vmpush(vm, string);
}

static void load_newtrue(VirtualMachine *vm, VALUE stream) {
  vmpush(vm, Qtrue);
}

static void load_newfalse(VirtualMachine *vm, VALUE stream) {
  vmpush(vm, Qfalse);
}

void unescape_unicode_ary(VALUE stream, VALUE ary) {
  uchar buf1 = read_one(stream);
  uchar buf2 = read_one(stream);
  uchar buf3 = read_one(stream);
  uchar buf4 = read_one(stream);
  
  uchar chr1 = eval_hex(buf1, buf2);
  uchar chr2 = eval_hex(buf3, buf4);
  
  int utf16 = chr2 + chr1 * 256;

  rb_ary_push(ary, INT2FIX(utf16));
}

void handle_escaping_ary(VALUE stream, VALUE ary) {
  uchar buf, unescaped;
  
  buf = read_one(stream);
  if(buf == 'u') {
    unescape_unicode_ary(stream, ary);
  } else {
    unescaped = unescape(buf, stream);
    if(buf == unescaped) {
      rb_raise(rb_eRuntimeError, "Missing unescaping instruction: \"%c\"", buf);
    } else {
      rb_ary_push(ary, CHR2FIX(unescaped));
    }
  }
}

static void load_unicode(VirtualMachine *vm, VALUE stream) {
  VALUE bytes = rb_ary_new();
  
  uchar buf = read_one(stream);
  
  while(buf != P_NEWLINE) {
    if(buf == P_ESCAPE) {
      handle_escaping_ary(stream, bytes);
    } else {
      rb_ary_push(bytes, CHR2FIX(buf));
    }
    buf = read_one(stream);
  }
  
  VALUE string = pack_to_utf8(bytes);
  vmpush(vm, string);
}

static void load_binunicode(VirtualMachine *vm, VALUE stream) {
  uchar i1 = read_one(stream);
  uchar i2 = read_one(stream);
  uchar i3 = read_one(stream);
  uchar i4 = read_one(stream);
  int len = i1 + i2*256 + i3*256*256 + i4*256*256*256;
  
  uchar chars[len];
  
  int i;
  for(i = 0; i < len; i++) {
    uchar buf = read_one(stream);
    
    chars[i] = buf;
  }
  
  VALUE string = rb_str_new((uchar*)chars, len);
  vmpush(vm, string);
}

static void init_dispatch() {
  Marker = rb_cObject;
  
  dispatch[P_INT]             = load_int;
  dispatch[P_STRING]          = load_string;
  dispatch[P_PUT]             = put_memo;
  dispatch[P_FLOAT]           = load_float;
  dispatch[P_MARK]            = mark_stack;
  dispatch[P_LIST]            = load_list;
  dispatch[P_APPEND]          = append;
  dispatch[P_EMPTY_LIST]      = load_empty_list;
  dispatch[P_EMPTY_TUPLE]     = load_empty_list;
  dispatch[P_TUPLE]           = load_list;
  dispatch[P_APPENDS]         = appends;
  dispatch[P_GET]             = get_memo;
  dispatch[P_POP]             = pop_stack;
  dispatch[P_DICT]            = load_dict;
  dispatch[P_SETITEM]         = set_item;
  dispatch[P_EMPTY_DICT]      = load_empty_dict;
  dispatch[P_SETITEMS]        = set_items;
  dispatch[P_TUPLE1]          = load_tuple1;
  dispatch[P_TUPLE2]          = load_tuple2;
  dispatch[P_TUPLE3]          = load_tuple3;
  dispatch[P_LONG]            = load_int;
  dispatch[P_BININT]          = load_binint;
  dispatch[P_BININT1]         = load_binint1;
  dispatch[P_BININT2]         = load_binint2;
  dispatch[P_NONE]            = load_none;
  dispatch[P_BINSTRING]       = load_binstring;
  dispatch[P_SHORT_BINSTRING] = load_short_binstring;
  dispatch[P_NEWTRUE]         = load_newtrue;
  dispatch[P_NEWFALSE]        = load_newfalse;
  dispatch[P_UNICODE]         = load_unicode;
  dispatch[P_BINUNICODE]      = load_binunicode;
}

static void init_methods() {
  meth_read = rb_intern("readbyte");
  meth_to_i = rb_intern("to_i");
  meth_to_f = rb_intern("to_f");
  meth_rewind = rb_intern("rewind");
  meth_slice = rb_intern("slice");
  meth_pack = rb_intern("pack");
}

void Init_ruby_cpickle() {
  init_methods();
  init_dispatch();
  
  VALUE rubyPickle = rb_define_module("RubyPickle");
  VALUE pickler = rb_define_class_under(rubyPickle, "Pickler", rb_cObject);
  
  VALUE unpickler = rb_define_class_under(rubyPickle, "Unpickler", rb_cObject);
  rb_define_method(unpickler, "to_ruby", reader, 0);
}