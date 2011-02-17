require 'bigdecimal'
require 'ruby_pickle/operations'
require 'set'

module RubyPickle
  class Pickler
    autoload :SimpleTypes, 'ruby_pickle/pickler/simple_types'
    autoload :Util, 'ruby_pickle/pickler/util'
    autoload :ComplexTypes, 'ruby_pickle/pickler/complex_types'
    include Operations
    include Util
    include SimpleTypes
    include ComplexTypes
    
    attr_accessor :object
    
    def initialize object
      @object = object
      @pickle = StringIO.new ""
      @memo_counter = 0
    end
    
    def dispatch object
      case object
      when Float, BigDecimal
        save_float object
      when Numeric
        save_int object
      when String
        save_string object
      when TrueClass, FalseClass
        save_boolean object
      when nil
        save_nil
      when Hash
        save_hash object
      when Enumerable
        save_enumerable object
      else
        raise "Can not pickle #{object.inspect}"
      end
    end
    
    def to_pickle
      dispatch object
      write STOP
      @pickle.string
    end
  end
end