module RubyPickle
  class Unpickler
    module VirtualMachine
      def push value
        @stack.push value
      end
    
      def pop
        @stack.pop
      end
    
      def read length = 1
        stream.read length
      end
    
      def peek
        stack.last
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
      
        while element != Mark do
          elements.unshift element
          element = pop
        end
      
        elements
      end
    
      def dup
        push peek
      end
    
      def put
        pos = read_arg.to_i
        memo[pos] = peek
      end
    
      def get
        pos = read_arg.to_i
        push memo[pos]
      end

      def setup_vm
        @stack = []
        @memo = []
      end
    end
  end
end