//
// Copyright 2003-2006 Christopher Baus. http://baus.net/
// Read the LICENSE file for more information.

#include "event.hpp"
#include "i_event_handler.hpp"

namespace eventlib{

  event::event():fd_(-1), p_event_handler_(0)
{
}

int event::set(int fd, i_event_handler* p_event_handler)
{
  fd_ = fd;
  p_event_handler_ = p_event_handler;
  event_set(&event_, fd_, 0, 0, this);
}


event::~event()
{

}

}
