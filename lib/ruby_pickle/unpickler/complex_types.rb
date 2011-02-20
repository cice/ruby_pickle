require 'ruby-debug'

module RubyPickle
  class Unpickler
    module ComplexTypes
      def load_tuple n
        push pop_n(n).reverse
      end
    
      def set_items
        elements = pop_stack_to_mark
        peek.merge! Hash[*elements]
      end
    
      def appends
        elements = pop_stack_to_mark
        peek.concat elements
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
    
      def build_list
        push [*pop_stack_to_mark]
      end 
    end
  end
end