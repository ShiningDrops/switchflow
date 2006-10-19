// 
// Copyright (C) Christopher Baus.  All rights reserved.
#ifndef SSD_STATUS_H
#define SSD_STATUS_H

namespace socketlib
{

  enum STATUS{
    COMPLETE,
    INCOMPLETE,
    READ_INCOMPLETE,
    WRITE_INCOMPLETE,
    CONNECTION_FAILED,
    SRC_CLOSED,
    DEST_CLOSED,
    DENY
  };

} // namespace proxylib 

#endif // STATUS_H
