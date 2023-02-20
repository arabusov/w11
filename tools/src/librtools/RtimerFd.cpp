// $Id: RtimerFd.cpp 1376 2023-02-20 15:05:03Z mueller $
// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2013-2023 by Walter F.J. Mueller <W.F.J.Mueller@gsi.de>
// 
// Revision History: 
// Date         Rev Version  Comment
// 2023-02-20  1376   1.1.1  BUGFIX: SetRelative(): correct is-positive check
// 2019-06-08  1161   1.1    derive from Rfd, inherit IsOpen,Close,Fd
// 2017-02-18   852   1.0    Initial version
// 2013-01-11   473   0.1    First draft
// ---------------------------------------------------------------------------

/*!
  \brief   Implemenation of class RtimerFd.
*/

#include <errno.h>
#include <unistd.h>
#include <sys/timerfd.h>

#include "RtimerFd.hpp"

#include "Rexception.hpp"

using namespace std;

/*!
  \class Retro::RtimerFd
  \brief FIXME_docs
*/

// all method definitions in namespace Retro
namespace Retro {

//------------------------------------------+-----------------------------------
//! FIXME_docs

RtimerFd::RtimerFd()
  : RtimerFd("RtimerFd::")
{}

//------------------------------------------+-----------------------------------
//! FIXME_docs

RtimerFd::RtimerFd(const char* cnam)
  : Rfd(cnam)
{}

//------------------------------------------+-----------------------------------
//! FIXME_docs

void RtimerFd::Open(clockid_t clkid)
{
  if (IsOpen())
    throw Rexception(fCnam+"Open()", "bad state: already open");

  fFd = ::timerfd_create(clkid, TFD_NONBLOCK);
  if (!IsOpen()) 
    throw Rexception(fCnam+"Open()", "timerfd_create() failed: ", errno);
  return;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

void RtimerFd::SetRelative(const Rtime& dt)
{
  if (!IsOpen())
    throw Rexception(fCnam+"SetRelative()", "bad state: not open");

  if (!dt.IsPositive())
    throw Rexception(fCnam+"SetRelative()", "bad value: dt zero or negative ");

  struct itimerspec itspec;
  itspec.it_interval.tv_sec   = 0;
  itspec.it_interval.tv_nsec  = 0;
  itspec.it_value             = dt.Timespec();

  if (::timerfd_settime(fFd, 0, &itspec, nullptr) < 0)
    throw Rexception(fCnam+"SetRelative()", 
                     "timerfd_settime() failed: ", errno);
  return;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

void RtimerFd::Cancel()
{
  if (!IsOpen())
    throw Rexception(fCnam+"Cancel()", "bad state: not open");

  struct itimerspec itspec;
  itspec.it_interval.tv_sec   = 0;
  itspec.it_interval.tv_nsec  = 0;
  itspec.it_value.tv_sec      = 0;
  itspec.it_value.tv_nsec     = 0;

  // cancel running timers
  if (::timerfd_settime(fFd, 0, &itspec, nullptr) < 0)
    throw Rexception(fCnam+"Cancel()", "timerfd_settime() failed: ", errno);

  // clear aready experied timers
  uint64_t cnt;
  int irc = ::read(fFd, &cnt, sizeof(cnt));
  if (irc < 0 && errno != EAGAIN)
    throw Rexception(fCnam+"Cancel()", "read() failed: ", errno);

  return;
}
  
//------------------------------------------+-----------------------------------
//! FIXME_docs

uint64_t RtimerFd::Read()
{
  if (!IsOpen())
    throw Rexception(fCnam+"Read()", "bad state: not open");

  uint64_t cnt;
  int irc = ::read(fFd, &cnt, sizeof(cnt));
  if (irc < 0) {
    if (errno == EAGAIN) return 0;
    throw Rexception(fCnam+"Read()", "read() failed: ", errno);
  }
  return cnt;
}

} // end namespace Retro
