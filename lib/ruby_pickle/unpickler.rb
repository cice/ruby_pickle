require 'ruby_pickle/operations'
require 'ruby_pickle/virtual_machine'
require 'ruby_pickle/util'
require 'ruby_pickle/simple_types'
require 'ruby_pickle/lists'

module RubyPickle
  class Unpickler
    Mark = Object.new
    
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
      when 'I'
        load_integer
      when 'S'
        load_string
      when 'F'
        load_float
      when 'L'
        load_long_int
      when 'J'
        load_bin_int
      when 'K'
        load_bin_int1
      when 'N'
        nil
      when 'T'
        load_bin_string
      when 'U'
        load_short_bin_string
      when 'V'
        load_unicode_string
      when 'X'
        load_short_unicode_string
      when '('
        set_mark
      when 'l'
        build_list
      when 'p'
        put
      when 'a'
        append
      when 'd'
        build_dict
      when 's'
        set_item
      when ']'
        load_empty_list
      else
        raise "KENN ISCH NIT: '#{next_op}'"
      end
    end
    
    def load_empty_list
      push []
    end
    
    def build_dict
      push Hash[*read_stack_to_mark]
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
      push [*read_stack_to_mark]
    end
    
    def read_stack_to_mark
      elements = []
      element = pop
      
      while element != Mark do
        elements.unshift element
        element = pop
      end
      
      push_some elements
      elements
    end
    
    def push_some elements
      elements.each do |e|
        push e
      end
    end
    
    def set_mark
      push Mark
    end
  end
end