require 'ruby_pickle/operations'

module RubyPickle
  class VirtualMachine
    include Operations
    attr_accessor :pickle, :stack, :memo
    
    def pickle= pickle_string
      @pickle = StringIO.new pickle_string
    end
    
    def initialize pickle = nil
      @pickle = StringIO.new pickle
    end
    
    def to_ruby
      return nil if pickle.nil? || !pickle.is_a?(StringIO)
      
      setup_vm
      pickle.rewind
      
      while (next_op = read)
        break if next_op == STOP
        dispatch next_op
      end
      
      @stack.pop
    end
    
    protected
    def load_integer
      @stack.push read_arg.to_i
    end
    
    def load_string
      @stack.push eval_string(read_arg)
    end
    
    def dispatch next_op
      case next_op
      when 'I'
        load_integer
      when 'S'
        load_string
      end
    end
    
    def eval_string string
      string = string.to_s[1..-2]
      
      string.gsub /(\\.)/ do |match|
        eval('"' + match.to_s + '"')
      end
    end
    
    def read
      @pickle.read 1
    end
    
    def read_arg
      @pickle.readline("\n")[0..-2]
    end
    
    def setup_vm
      @stack = []
      @memo = []
    end
  end
end