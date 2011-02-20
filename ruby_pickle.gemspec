# -*- encoding: utf-8 -*-
$:.push File.expand_path("../lib", __FILE__)
$:.push File.expand_path("../ext", __FILE__)
require "ruby_pickle/version"

Gem::Specification.new do |s|
  s.name        = "ruby_pickle"
  s.version     = RubyPickle::VERSION
  s.platform    = Gem::Platform::RUBY
  s.date        = Time.now.strftime("%Y-%m-%d")
  s.authors     = ["Michael Eickenberg", "Marian Theisen"]
  s.email       = 'marian@cice-online.net'
  s.summary     = "Pickling and Unpickling for Ruby"
  s.homepage    = "http://github.com/cice/kangARoo"
  s.description = "Pickling and Unpickling for Ruby"

  s.files         = `git ls-files`.split("\n")
  s.test_files    = `git ls-files -- {test,spec,features}/*`.split("\n")
  s.executables   = `git ls-files -- bin/*`.split("\n").map{ |f| File.basename(f) }
  s.require_paths = ["lib"]

  s.add_dependency "activesupport", ">= 3.0.0"
  
  s.add_development_dependency 'yard'
  s.add_development_dependency 'rspec'
  s.add_development_dependency 'ruby-debug'
end