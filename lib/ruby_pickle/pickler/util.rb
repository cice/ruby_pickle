module RubyPickle
  class Pickler
    module Util
      def escape string
        "'" + string.to_s.inspect[1...-1] + "'"
      end
    
      def write str
        @pickle.write str
      end
    end
  end
end