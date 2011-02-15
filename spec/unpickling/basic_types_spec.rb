require 'spec_helper'

module RubyPickle
  describe Unpickler do
    include SpecHelper
    
    it 'unpickles Integers' do
      run_sample 'simple_int'
    end
    
    it 'unpickles negative Integers' do
      run_sample 'neg_int'
    end
    
    it 'unpickles strings' do
      run_sample 'simple_string'
    end
    
    it 'unpickles multiline strings' do
      run_sample 'ml_string'
    end
    
    it 'unpickles floats' do
      run_sample 'simple_float'
    end
    
    it 'unpickles unprecise floats' do
      run_sample 'unprec_float'
    end
    
    it 'unpickles long int' do
      run_sample 'long_int'
    end
    
    it 'unpickles 4byte bin int' do
      run_sample 'binint'
    end
    
    it 'unpickles 1byte bin int' do
      run_sample 'binint1'
    end
    
    it 'unpickles none' do
      run_sample 'none'
    end
    
    it 'unpickles boolean types' do
      run_sample 'true_val'
      run_sample 'false_val'
    end
    
    it 'unpickles binstring' do
      run_sample 'binstring'
      run_sample 'ml_binstring'
    end
    
    it 'unpickles short binstring' do
      run_sample 'short_binstring'
    end
    
    it 'unpickles unicode string' do
      # pending "need lib to convert unicode code points to mb chars"
      run_sample 'unicode_string'
    end
    
    it 'unpickles short unicode string' do
      run_sample 'short_unicode_string'
    end
  end
end