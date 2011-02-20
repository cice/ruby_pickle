require 'ruby_cpickle'
require 'stringio'

module RubyPickle
  class Unpickler
    def initialize stream
      @stream = stream
    end
  end
end

p = RubyPickle::Unpickler.new StringIO.new("(lp0\nI1\na.")
# puts p.to_ruby
puts p.to_ruby.inspect
# p.to_ruby