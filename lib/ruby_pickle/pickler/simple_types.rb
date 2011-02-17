module RubyPickle
  class Pickler
    module SimpleTypes
      include Operations
                
      def save_boolean object
        if object
          write TRUE
        else
          write FALSE
        end
      end
    
      def save_nil
        write NONE
      end
    
      def save_float object
        write FLOAT + object.to_f.to_s + NEWLINE
      end
    
      def save_string object
        write STRING + escape(object) + NEWLINE
        save_put
      end
    
      def save_put n = @memo_counter
        write PUT + n.to_i.to_s + NEWLINE
        @memo_counter += 1
      end
    
      def save_int object
        write INT + object.to_s + NEWLINE
      end
    end
  end
end