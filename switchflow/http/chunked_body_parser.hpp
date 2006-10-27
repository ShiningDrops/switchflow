//
// Copyright 2003-2006 Christopher Baus. http://baus.net/
// Read the LICENSE file for more information.

#ifndef SSD_CHUNKEDBODYPARSER_HPP
#define SSD_CHUNKEDBODYPARSER_HPP

#include <boost/noncopyable.hpp>

#include "http.hpp"


namespace http{

class i_body_receiver;
  
class ChunkedBodyParser: private boost::noncopyable
{
public:
  ChunkedBodyParser(i_body_receiver* pBodyReceiver, unsigned int maxChunksizeLength);
  virtual ~ChunkedBodyParser();
  
  void reset();
  STATUS parseBody(read_write_buffer& buffer);
  
private:
  enum STATE
  {
    PARSE_CHUNKSIZE,
    PARSE_CHUNK,
    PARSE_CHUNKSIZE_LF,
    FORWARD_CHUNKSIZE,
    FORWARD_INCOMPLETE_CHUNK,
    FORWARD_COMPLETE_CHUNK,
    PARSE_TRAILER_CR,
    PARSE_TRAILER_LF,
    FORWARD_TRAILER_CRLF
  };

  STATUS parseChunkSize(read_write_buffer& buffer);
  
  STATE state_;
  
  i_body_receiver* pBodyReceiver_;
  
  unsigned int chunksize_;

  unsigned int currentChunksizeLength_;

  unsigned int maxChunksizeLength_;

  unsigned int currentOffset_;

  unsigned int currentChunkLength_;

  unsigned int numChunkSizeSpaces_;
};

}// namespace httplib
  
#endif // CHUNKEDBODYPARSER_HPP
