require 'spec_helper'

module RubyPickle
  describe Pickler do
    include SpecHelper
    
    it 'pickles Integers' do
      run_pickle 'simple_int'
    end
    
    it 'pickles negative Integers' do
      run_pickle 'neg_int'
    end
    
    it 'pickles strings' do
      run_pickle 'simple_string'
    end
    
    it 'pickles multiline strings' do
      run_pickle 'ml_string'
    end
    
    it 'pickles floats' do
      run_pickle 'simple_float'
    end
    
    it 'pickles unprecise floats' do
      pending
      run_pickle 'unprec_float'
    end
    
    # it 'pickles long int' do
    #   run_pickle 'long_int'
    # end
    # 
    # it 'pickles 4byte bin int' do
    #   run_pickle 'binint'
    # end
    # 
    # it 'pickles 1byte bin int' do
    #   run_pickle 'binint1'
    # end
    # 
    # it 'pickles none' do
    #   run_pickle 'none'
    # end
    # 
    it 'pickles boolean types' do
      run_pickle 'true_val'
      run_pickle 'false_val'
    end
    # 
    # it 'pickles binstring' do
    #   run_pickle 'binstring'
    #   run_pickle 'ml_binstring'
    # end
    # 
    # it 'pickles short binstring' do
    #   run_pickle 'short_binstring'
    # end
    # 
    # it 'pickles unicode string' do
    #   # pending "need lib to convert unicode code points to mb chars"
    #   run_pickle 'unicode_string'
    # end
    # 
    # it 'pickles short unicode string' do
    #   run_pickle 'short_unicode_string'
    # end
    # 
    # it 'pickles new boolean' do
    #   run_pickle 'new_true'
    #   run_pickle 'new_false'
    # end
  end
end