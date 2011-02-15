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
        @pickle.read length
      end
       
      def setup_vm
        @stack = []
        @memo = []
      end
    end
  end
end