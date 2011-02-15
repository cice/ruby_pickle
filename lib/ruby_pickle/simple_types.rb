module RubyPickle
  module SimpleTypes
    
    protected
    def load_integer
      arg = read_arg
      
      push case arg
      when '01'
        true
      when '00'
        false
      else
        arg.to_i
      end
    end
    
    def load_string
      push eval_string(read_arg)
    end
    
    def load_float
      push read_arg.to_f
    end
    
    def load_long_int
      push read_arg_skip.to_i
    end
    
    def load_bin_int
      push eval_binint(read_arg)
    end
    
    def load_bin_int1
      push eval_binint(read_arg)
    end
    
    def load_bin_string
      length = eval_binint(read(16))
      push read(length)
    end
    
    def load_short_bin_string
      length = eval_binint(read(4))
      push read(length)
    end
    
    def load_unicode_string
      push eval_unicode_cp(read_arg)
    end
    
    def load_short_unicode_string
      length = eval_binint(read(16))
      push read(length)
    end
  end
end