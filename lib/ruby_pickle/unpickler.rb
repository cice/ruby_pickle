module RubyPickle
  class Unpickler
    autoload :Operations, 'ruby_pickle/unpickler/operations'
    autoload :VirtualMachine, 'ruby_pickle/unpickler/virtual_machine'
    autoload :Util, 'ruby_pickle/unpickler/util'
    autoload :SimpleTypes, 'ruby_pickle/unpickler/simple_types'
    autoload :Lists, 'ruby_pickle/unpickler/lists'

    Marker = Object.new
    
    include Operations
    include VirtualMachine
    include Util
    include SimpleTypes
    include Lists
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
    
    def load_true
      push true
    end
    
    def load_false
      push false
    end
    
    def load_tuple n
      push pop_n(n).reverse
    end
    
    def set_items
      elements = pop_stack_to_mark
      peek.merge! Hash[*elements]
    end
    
    def get
      pos = read_arg.to_i
      push memo[pos]
    end
    
    def appends
      elements = pop_stack_to_mark
      peek.concat elements
    end
    
    def dup
      push peek
    end
    
    def build_tuple
      push [*pop_stack_to_mark]
    end    
    
    def load_empty_tuple
      push []
    end
    
    def load_empty_dict
      push Hash.new
    end
    
    def load_empty_list
      push []
    end
    
    def build_dict
      push Hash[*pop_stack_to_mark]
    end
    
    def set_item
      val, key = pop, pop
      peek[key] = val
    end
    
    def append
      element = pop
      peek << element
    end
    
    def peek
      stack.last
    end
    
    def put
      pos = read_arg.to_i
      memo[pos] = peek
    end
    
    def build_list
      push [*pop_stack_to_mark]
    end
    
    def pop_n n
      [].tap do |array|
        n.times do
          array << pop
        end
      end
    end
    
    def pop_stack_to_mark
      elements = []
      element = pop
      
      while element != Marker do
        elements.unshift element
        element = pop
      end
      
      elements
    end
    
    def push_some elements
      elements.each do |e|
        push e
      end
    end
    
    def set_mark
      push Marker
    end
  end
end