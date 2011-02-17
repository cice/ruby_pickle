module RubyPickle
  class Pickler
    module ComplexTypes
      include Operations
      
      def save_enumerable object
        if object.empty?
          write EMPTY_LIST
          return
        end
        
        write MARK
        write LIST
        save_put
        
        object.each do |item|
          dispatch item
          write APPEND
        end
      end
      
      def save_hash object
        if object.empty?
          write EMPTY_DICT
          return
        end
        
        write MARK
        write DICT
        save_put
        
        object.each do |key, value|
          dispatch key
          dispatch value
          write SETITEM
        end
      end
    end
  end
end