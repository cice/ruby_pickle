require 'ruby_pickle'
require 'benchmark'

p = RubyPickle::Unpickler.new StringIO.new("(lp0\nI1\na(lp1\nI2\na(lp2\nI3\naI3\naI3\naI3\naI3\naI3\naI3\naI3\naI3\naI4\naaa.")

puts Benchmark.measure {
  10000.times do 
    p.to_ruby
  end
}
puts p.to_ruby.inspect