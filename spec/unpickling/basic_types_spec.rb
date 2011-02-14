require 'spec_helper'

module RubyPickle
  describe VirtualMachine do
    include SpecHelper
    
    it 'unpickles Integers' do
      value = unpickle_sample 'simple_int'
      value.should == sample_value('simple_int')
    end
    
    it 'unpickles negative Integers' do
      value = unpickle_sample 'neg_int'
      value.should == sample_value('neg_int')
    end
    
    it 'unpickles strings' do
      value = unpickle_sample 'simple_string'
      value.should == sample_value('simple_string')
    end
    
    it 'unpickles multiline strings' do
      value = unpickle_sample 'ml_string'
      value.should == sample_value('ml_string')
    end
  end
end