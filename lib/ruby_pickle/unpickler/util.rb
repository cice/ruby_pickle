require 'iconv'

module RubyPickle
  class Unpickler
    module Util
      def eval_unicode_cp codepoints
        utf16 = codepoints.gsub(/\\u..../) { |match|
          match[2,2].to_i(16).chr + match[4,2].to_i(16).chr
        }
      
        Iconv.conv('UTF-8', 'UTF-16', utf16)
      end
    
      def eval_binint string
        val = 0
        i = 0
        string.each_byte do |byte|
          val += byte * (256 ** i)
          i += 1
        end
      
        val
      end
    
      def eval_string string
        string = string.to_s[1..-2]
      
        string.gsub /(\\.)/ do |match|
          eval('"' + match.to_s + '"')
        end
      end
 
      def read_arg
        @pickle.readline("\n")[0..-2]
      end
    
      def read_arg_skip
        read_arg[0...-1]
      end
    end
  end
end