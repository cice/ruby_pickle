require 'rubygems'
require 'bundler/setup'

require 'ruby_pickle'

Pickles = YAML.load_file('spec/pickles/samples.yml')['pickles']

module SpecHelper
  def anythings count
    [anything] * count
  end
  
  def sample label
    Pickles[label.to_s].to_a.first
  end
  
  def sample_pickle label
    sample(label).first
  end
  
  def sample_value label
    sample(label).last
  end
  
  def unpickle_sample label
    RubyPickle::Unpickler.new(sample_pickle(label)).to_ruby
  end
  
  def run_sample label
    unpickle_sample(label).should == sample_value(label)
  end
end

RSpec.configure do |config|
  # some (optional) config here
end