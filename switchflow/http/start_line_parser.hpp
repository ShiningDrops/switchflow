//
// Copyright 2003-2006 Christopher Baus. http://baus.net/
// Read the LICENSE file for more information.

#ifndef SF_START_LINE_PARSER_HPP
#define SF_START_LINE_PARSER_HPP

#include <set>
#include <asio.hpp>
#include "http.hpp" 
#include "line_parser.hpp"
#include "i_line_receiver.hpp"

namespace switchflow{
namespace http{

class i_start_line_receiver;  

class start_line_parser: private i_line_receiver
{
 public:
  start_line_parser(i_start_line_receiver& receiver,
                    size_t token1_max_length,
                    size_t token2_max_length,
                    size_t token3_max_length);

  enum PARSE_OPTION
  {
    STRICT,
    LOOSE
  };

  std::pair<STATUS, asio::const_buffer> parse_start_line(asio::const_buffer buffer,
                                                         PARSE_OPTION option=STRICT);

  void reset();
  
private:

  virtual STATUS receive_line(asio::const_buffer buffer, bool b_complete);
    
  enum PARSE_STATE
  {
    STATE_TOKEN_1,            // 0
    STATE_TOKEN_2,            // 1
    STATE_TOKEN_3,            // 2
    STATE_LF                  // 3
  };    

  void next_state(char delimiter);
  
  std::pair<STATUS, asio::const_buffer> parse_token(asio::const_buffer buffer,
                                                    size_t max_length,
                                                    PARSE_OPTION option);

  STATUS set_token(asio::const_buffer buffer, bool b_complete);

  std::set<char>& get_token_delimiters(PARSE_OPTION option);

  PARSE_STATE state_;
  
  size_t current_length_;
    
  i_start_line_receiver& receiver_;

  size_t token1_max_length_;
  size_t token2_max_length_;
  size_t token3_max_length_;
  line_parser line_parser_;
};

} //namespace httplib
} //namespace switchflow

#endif

