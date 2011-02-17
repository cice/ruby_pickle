require 'spec_helper'

module RubyPickle
  describe Pickler do
    include SpecHelper
    
    it 'pickles simple one element list' do
      pending
      run_pickle 'one_element_list_simple'
    end

    it 'pickles one element list' do
      run_pickle 'one_element_list'
    end
    # 
    it 'pickles multi int list' do
      run_pickle 'multi_int_list'
    end
    # 
    it 'pickles mixed list' do
      run_pickle 'mixed_list'
    end
    # 
    # it 'pickles simple dict' do
    #   run_pickle 'simple_dict'
    # end
    # 
    it 'pickles dict' do
      run_pickle 'dict1'
    end
    # 
    it 'pickles nested list' do
      run_pickle 'nested_list'
    end
    # 
    it 'pickles nested dict' do
      run_pickle 'nested_dict'
    end
    # 
    it 'pickles empty list' do
      run_pickle 'empty_list'
    end
    # 
    it 'pickles empty dict' do
      run_pickle 'empty_dict'
    end
    # 
    # it 'pickles tuples' do
    #   run_pickle 'simple_tuple'
    # end
    # 
    # it 'pickles empty tuple' do
    #   run_pickle 'empty_tuple'
    # end
    # 
    # it 'pickles appended list items' do
    #   run_pickle 'appends_elements'
    # end
    # 
    # it 'pickles put / get' do
    #   run_pickle 'put_get'
    # end
    # 
    # it 'pickles update dict' do
    #   run_pickle 'update_dict'
    # end
    # 
    # it 'pickles n-tuples' do
    #   run_pickle 'n_tuple'
    # end
  end
end