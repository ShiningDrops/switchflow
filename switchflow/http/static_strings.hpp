//
// Copyright 2003-2006 Christopher Baus. http://baus.net/
// Read the LICENSE file for more information.

#ifndef SF_STATIC_STRINGS_HPP
#define SF_STATIC_STRINGS_HPP

#include <util/read_write_buffer.hpp>


namespace switchflow{
namespace http{
  
class static_strings
{
public:
  static_strings();

  raw_buffer space_;
  raw_buffer endline_;
  raw_buffer fieldsep_;
};

extern static_strings strings_;

} //namespace http
} //namespace switchflow

#endif 
