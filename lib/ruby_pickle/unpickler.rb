require 'ruby_pickle/operations'

module RubyPickle
  class Unpickler
    autoload :VirtualMachine, 'ruby_pickle/unpickler/virtual_machine'
    autoload :Util, 'ruby_pickle/unpickler/util'
    autoload :SimpleTypes, 'ruby_pickle/unpickler/simple_types'
    autoload :ComplexTypes, 'ruby_pickle/unpickler/complex_types'

    Mark = Object.new.freeze
    
    include Operations
    include VirtualMachine
    include Util
    include SimpleTypes
    include ComplexTypes
    attr_accessor :pickle, :stack, :memo
    
    def pickle= pickle_string
      @pickle = String === pickle_string ? StringIO.new(pickle_string) : pickle_string
    end
    
    def initialize pickle = nil
      self.pickle = pickle
    end
    
    def to_ruby
      return nil if pickle.nil? || !pickle.is_a?(StringIO)
      
      setup_vm
      pickle.rewind
      
      while (next_op = read)
        break if next_op == STOP
        dispatch next_op
      end
      
      pop
    end
    
    protected
    def dispatch next_op
      case next_op
      when INT
        load_integer
      when STRING
        load_string
      when FLOAT
        load_float
      when LONG
        load_long_int
      when BININT
        load_bin_int
      when BININT1
        load_bin_int1
      when NONE
        nil
      when BINSTRING
        load_bin_string
      when SHORT_BINSTRING
        load_short_bin_string
      when UNICODE
        load_unicode_string
      when BINUNICODE
        load_short_unicode_string
      when MARK
        set_mark
      when LIST
        build_list
      when PUT
        put
      when APPEND
        append
      when DICT
        build_dict
      when SETITEM
        set_item
      when EMPTY_LIST
        load_empty_list
      when EMPTY_DICT
        load_empty_dict
      when EMPTY_TUPLE
        load_empty_tuple
      when TUPLE
        build_tuple
      when POP
        pop
      when POP_MARK
        pop_stack_to_mark
      when DUP
        dup
      when APPENDS
        appends
      when GET
        get
      when SETITEMS
        set_items
      when TUPLE1
        load_tuple(1)
      when TUPLE2
        load_tuple(2)
      when TUPLE3
        load_tuple(3)
      when NEWTRUE
        load_true
      when NEWFALSE
        load_false
      else
        raise "KENN ISCH NIT: '#{next_op}'"
      end
    end
    
    def set_mark
      push Mark
    end
  end
end