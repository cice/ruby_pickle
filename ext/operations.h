#define P_MARK             40   // "("  // push special markobject on stack
#define P_STOP             46   //"."   // every pickle ends with STOP
#define P_POP              48   //"0"   // discard topmost stack item
#define P_POP_MARK         49   //"1"   // discard stack top through topmost markobject
#define P_DUP              50   //"2"   // duplicate top stack item
#define P_FLOAT            70   //"F"   // push float object; decimal string argument
#define P_INT              73   //"I"   // push integer or bool; decimal string argument
#define P_BININT           74   //"J"   // push four-byte signed int
#define P_BININT1          75   //"K"   // push 1-byte unsigned int
#define P_LONG             76   //"L"   // push long; decimal string argument
#define P_BININT2          77   //"M"   // push 2-byte unsigned int
#define P_NONE             78   //"N"   // push None
#define P_PERSID           80   //"P"   // push persistent object; id is taken from string arg
#define P_BINPERSID        81   //"Q"   //  "       "         "  ;  "  "   "     "  stack
#define P_REDUCE           82   //"R"   // apply callable to argtuple, both on stack
#define P_STRING           83   //"S"   // push string; NL-terminated string argument
#define P_BINSTRING        84   //"T"   // push string; counted binary string argument
#define P_SHORT_BINSTRING  85   //"U"   //  "     "   ;    "      "       "      " < 256 bytes
#define P_UNICODE          86   //"V"   // push Unicode string; raw-unicode-escaped"d argument
#define P_BINUNICODE       88   //"X"   //   "     "       "  ; counted UTF-8 string argument
#define P_APPEND           97   //"a"   // append stack top to list below it
#define P_BUILD            98   //"b"   // call __setstate__ or __dict__.update()
#define P_GLOBAL           99   //"c"   // push self.find_class(modname, name); 2 string args
#define P_DICT             100  //"d"   // build a dict from stack items
#define P_EMPTY_DICT       125  //"}"   // push empty dict
#define P_APPENDS          101  //"e"   // extend list on stack by topmost stack slice
#define P_GET              103  //"g"   // push item from memo on stack; index is string arg
#define P_BINGET           104  //"h"   //   "    "    "    "   "   "  ;   "    " 1-byte arg
#define P_INST             105  //"i"   // build & push class instance
#define P_LONG_BINGET      106  //"j"   // push item from memo on stack; index is 4-byte arg
#define P_LIST             108  //"l"   // build list from topmost stack items
#define P_EMPTY_LIST       93   //"]"   // push empty list
#define P_OBJ              111  //"o"   // build & push class instance
#define P_PUT              112  //"p"   // store stack top in memo; index is string arg
#define P_BINPUT           113  //"q"   //   "     "    "   "   " ;   "    " 1-byte arg
#define P_LONG_BINPUT      114  //"r"   //   "     "    "   "   " ;   "    " 4-byte arg
#define P_SETITEM          115  //"s"   // add key+value pair to dict
#define P_TUPLE            116  //"t"   // build tuple from topmost stack items
#define P_EMPTY_TUPLE      41   //")"   // push empty tuple
#define P_SETITEMS         117  //"u"   // modify dict by adding topmost key+value pairs
#define P_BINFLOAT         71   //"G"   // push float; arg is 8-byte float encoding

#define P_TRUE             "01"  // not an opcode; see INT docs in pickletools.py
#define P_FALSE            "00"  // not an opcode; see INT docs in pickletools.py

// Protocol 2
#define P_PROTO            128  //"\x80"  // identify pickle protocol
#define P_NEWOBJ           129  //"\x81"  // build object by applying cls.__new__ to argtuple
#define P_EXT1             130  //"\x82"  // push object from extension registry; 1-byte index
#define P_EXT2             131  //"\x83"  // ditto, but 2-byte index
#define P_EXT4             132  //"\x84"  // ditto, but 4-byte index
#define P_TUPLE1           133  //"\x85"  // build 1-tuple from stack top
#define P_TUPLE2           134  //"\x86"  // build 2-tuple from two topmost stack items
#define P_TUPLE3           135  //"\x87"  // build 3-tuple from three topmost stack items
#define P_NEWTRUE          136  //"\x88"  // push True
#define P_NEWFALSE         137  //"\x89"  // push False
#define P_LONG1            138  //"\x8a"  // push long from < 256 bytes
#define P_LONG4            139  //"\x8b"  // push really big long

#define P_NEWLINE          10   //"\n"
#define P_ESCAPE           92   //"\\"
