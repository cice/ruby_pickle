require 'spec_helper'

module RubyPickle
  describe Unpickler do
    include SpecHelper
    
    it 'unpickles simple one element list' do
      run_sample 'one_element_list_simple'
    end
    
    it 'unpickles one element list' do
      run_sample 'one_element_list'
    end
    
    it 'unpickles multi int list' do
      run_sample 'multi_int_list'
    end
    
    it 'unpickles mixed list' do
      run_sample 'mixed_list'
    end
    # 
    it 'unpickles simple dict' do
      run_sample 'simple_dict'
    end
    
    it 'unpickles dict' do
      run_sample 'dict1'
    end
    
    it 'unpickles nested list' do
      run_sample 'nested_list'
    end
    
    it 'unpickles nested dict' do
      run_sample 'nested_dict'
    end
    
    it 'unpickles empty list' do
      run_sample 'empty_list'
    end
    
    it 'unpickles empty dict' do
      run_sample 'empty_dict'
    end
    
    it 'unpickles tuples' do
      run_sample 'simple_tuple'
    end
    # 
    it 'unpickles empty tuple' do
      run_sample 'empty_tuple'
    end
    # 
    it 'unpickles appended list items' do
      run_sample 'appends_elements'
    end
    # 
    it 'unpickles put / get' do
      run_sample 'put_get'
    end
    # 
    it 'unpickles update dict' do
      run_sample 'update_dict'
    end
    
    it 'unpickles n-tuples' do
      run_sample 'n_tuple'
    end
  end
end