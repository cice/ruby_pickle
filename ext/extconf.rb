require 'mkmf'


extension_name = 'ruby_cpickle'
dir_config extension_name

LIBDIR = Config::CONFIG['libdir']
INCLUDEDIR = Config::CONFIG['includedir']

HEADER_DIRS = [
  # First search /opt/local for macports
  '/opt/local/include',

  # Then search /usr/local for people that installed from source
  '/usr/local/include',

  # Check the ruby install locations
  INCLUDEDIR,

  # Finally fall back to /usr
  '/usr/include',
  '/usr/include/libxml2',
]

LIB_DIRS = [
  # First search /opt/local for macports
  '/opt/local/lib',

  # Then search /usr/local for people that installed from source
  '/usr/local/lib',

  # Check the ruby install locations
  LIBDIR,

  # Finally fall back to /usr
  '/usr/lib',
]

XML2_HEADER_DIRS = [
  '/opt/local/include/libxml2',
  '/usr/local/include/libxml2',
  File.join(INCLUDEDIR, "libxml2")
] + HEADER_DIRS

dir_config('iconv', HEADER_DIRS, LIB_DIRS)
have_func('iconv_open', 'iconv.h') or have_library('iconv', 'iconv_open', 'iconv.h')

create_makefile extension_name

