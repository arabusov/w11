// $Id: RlinkPortCuff.cpp 1209 2021-08-22 13:17:33Z mueller $
// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2012-2021 by Walter F.J. Mueller <W.F.J.Mueller@gsi.de>
// 
// Revision History: 
// Date         Rev Version  Comment
// 2021-08-17  1209   1.1.11 drop libusb_set_debug (now deprecated)
// 2018-12-18  1089   1.1.10 use c++ style casts
// 2018-12-17  1088   1.1.9  use std::thread instead of boost
// 2018-12-14  1081   1.1.8  use std::bind instead of boost
// 2018-11-09  1066   1.1.7  use auto
// 2018-10-27  1059   1.1.6  coverity fixup (uncaught exception in dtor)
// 2017-04-15   875   1.1.5  Open(): set default scheme
// 2017-03-04   858   1.1.4  use clock_gettime instead of gettimeofday
// 2015-04-12   666   1.1.3  add noinit attribute
// 2014-08-22   584   1.1.2  use nullptr
// 2013-05-17   521   1.1.1  use Rtools::String2Long
// 2013-02-23   492   1.1    use RparseUrl
// 2013-02-10   485   1.0.3  add static const defs
// 2013-02-03   481   1.0.2  use Rexception
// 2013-01-02   467   1.0.1  get cleanup code right; add USBErrorName()
// 2012-12-26   465   1.0    Initial version
// ---------------------------------------------------------------------------

/*!
  \brief   Implemenation of RlinkPortCuff.
*/

#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

#include <iostream>
#include <sstream>

#include "RlinkPortCuff.hpp"

#include "librtools/Rexception.hpp"
#include "librtools/Rtools.hpp"

using namespace std;

/*!
  \class Retro::RlinkPortCuff
  \brief FIXME_docs
*/

// all method definitions in namespace Retro
namespace Retro {

//------------------------------------------+-----------------------------------
// constants definitions

const size_t RlinkPortCuff::kUSBBufferSize;
const int    RlinkPortCuff::kUSBWriteEP;
const int    RlinkPortCuff::kUSBReadEP;
const size_t RlinkPortCuff::kUSBReadQueue;

//------------------------------------------+-----------------------------------
//! Default constructor

RlinkPortCuff::RlinkPortCuff()
  : RlinkPort(),
    fFdReadDriver(-1),
    fFdWriteDriver(-1),
    fpUsbContext(nullptr),
    fpUsbDevList(nullptr),
    fUsbDevCount(0),
    fpUsbDevHdl(nullptr),
    fLoopState(kLoopStateStopped)
{
  fStats.Define(kStatNPollAddCB,    "kStatNPollAddCB",    "USB poll add cb");
  fStats.Define(kStatNPollRemoveCB, "kStatNPollRemoveCB", "USB poll remove cb");
  fStats.Define(kStatNUSBWrite,     "kStatNUSBWrite",     "USB write done");
  fStats.Define(kStatNUSBRead,      "kStatNUSBRead",      "USB read done");
}

//------------------------------------------+-----------------------------------
//! Destructor

RlinkPortCuff::~RlinkPortCuff()
{
  if (IsOpen())  Rtools::Catch2Cerr(__func__,
                                    [this](){ RlinkPortCuff::Close(); } );
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

bool RlinkPortCuff::Open(const std::string& url, RerrMsg& emsg)
{
  int irc;

  if (IsOpen()) Close();

  if (!fUrl.Set(url, "|trace|noinit|", "cuff", emsg)) return false;

  // initialize USB context
  irc = libusb_init(&fpUsbContext);
  if (irc != 0) {
    emsg.Init("RlinkPortCuff::Open()", 
              string("libusb_init() failed: ") + 
              string(USBErrorName(irc)));
    Cleanup();
    return false;
  }
  //libusb_set_debug is deprecated, libusb_set_option has different features
  //libusb_set_debug(fpUsbContext, 3);        // info->stdout, warn+err->stderr

  // check for internal timeout handling support
  if (libusb_pollfds_handle_timeouts(fpUsbContext) == 0) {
    emsg.Init("RlinkPortCuff::Open()", 
              string("libusb_pollfds_handle_timeouts == 0 : "
                     "this program will not run on this legacy system"));
    Cleanup();
    return false;
  }

  // get USB device list
  fUsbDevCount = libusb_get_device_list(fpUsbContext, &fpUsbDevList);  

  // determine USB path name
  if (fUrl.Path().length() == 0) {
    char* env_vid = ::getenv("RETRO_FX2_VID");
    char* env_pid = ::getenv("RETRO_FX2_PID");
    if (env_vid && ::strlen(env_vid) == 4 &&
        env_pid && ::strlen(env_pid) == 4) {
      fUrl.SetPath(string(env_vid) + ":" + string(env_pid));
    } else {
      emsg.Init("RlinkPortCuff::Open()", 
                "RETRO_FX2_VID/PID not or ill defined");
      Cleanup();
      return false;
    }
  }

  // connect to USB device
  libusb_device* mydev = nullptr;
  // path syntax: /bus/dev
  if (fUrl.Path().length()==8 && fUrl.Path()[0]=='/' && fUrl.Path()[4]=='/') {
    string busnam = fUrl.Path().substr(1,3);
    string devnam = fUrl.Path().substr(5,3);
    unsigned long busnum;
    unsigned long devnum;
    if (!Rtools::String2Long(busnam, busnum, emsg) ||
        !Rtools::String2Long(devnam, devnum, emsg)) {
      Cleanup();
      return false;
    }
    for (ssize_t idev=0; idev<fUsbDevCount; idev++) {
      libusb_device* udev = fpUsbDevList[idev];
      if (libusb_get_bus_number(udev) == busnum &&
          libusb_get_device_address(udev) == devnum) {
        mydev = udev;
      }
    }
  // path syntax: vend:prod
  } else if (fUrl.Path().length()==9 && fUrl.Path()[4]==':') {
    string vennam = fUrl.Path().substr(0,4);
    string pronam = fUrl.Path().substr(5,4);
    unsigned long vennum;
    unsigned long pronum;
    if (!Rtools::String2Long(vennam, vennum, emsg, 16) ||
        !Rtools::String2Long(pronam, pronum, emsg, 16)) {
      Cleanup();
      return false;
    }
    for (ssize_t idev=0; idev<fUsbDevCount; idev++) {
      libusb_device* udev = fpUsbDevList[idev];
      libusb_device_descriptor devdsc;
      libusb_get_device_descriptor(udev, &devdsc);
      if (devdsc.idVendor==vennum && devdsc.idProduct==pronum) {
        mydev = udev;
      }
    }
  } else {
    emsg.Init("RlinkPortCuff::Open()", 
              string("invalid usb path '") + fUrl.Path() +
              "', not '/bus/dev' or 'vend:prod'");
    Cleanup();
    return false;
  }

  if (mydev == nullptr) {
    emsg.Init("RlinkPortCuff::Open()", 
              string("no usb device '") + fUrl.Path() + "', found'");
    Cleanup();
    return false;
  }

  irc = libusb_open(mydev, &fpUsbDevHdl);
  if (irc) {
    fpUsbDevHdl = nullptr;
    emsg.Init("RlinkPortCuff::Open()", 
              string("opening usb device '") + fUrl.Path() + "', failed: " +
              string(USBErrorName(irc)));
    Cleanup();
    return false;
  }
  if (TraceOn()) cout << "libusb_open ok for '" << fUrl.Path() << "'" << endl;

  // claim USB device
  irc = libusb_claim_interface(fpUsbDevHdl, 0);
  if (irc) {
    emsg.Init("RlinkPortCuff::Open()", 
              string("failed to claim '") + fUrl.Path() + "': " +
              string(USBErrorName(irc)));
    Cleanup();
    return false;
  }

  // setup write pipe
  if (!OpenPipe(fFdWriteDriver, fFdWrite, emsg)) {
    Cleanup();
    return false;
  }
  // setup read pipe
  if (!OpenPipe(fFdRead, fFdReadDriver, emsg)) {
    Cleanup();
    return false;
  }

  // setup pollfd list
  fPollFds.clear();

  // 1. write pipe allert (is always 1st in list)
  PollfdAdd(fFdWriteDriver, POLLIN);
  
  // 2. libusb callbacks
  const libusb_pollfd** plist = libusb_get_pollfds(fpUsbContext);
  for (auto p = plist; *p !=0; p++) {
    PollfdAdd((*p)->fd, (*p)->events);
  }
  ::free(plist);
  libusb_set_pollfd_notifiers(fpUsbContext, ThunkPollfdAdd,
                              ThunkPollfdRemove, this);

  fDriverThread =  thread([this](){ Driver(); });

  fIsOpen  = true;

  return true;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

void RlinkPortCuff::Close()
{
  if (!IsOpen()) return;

  if (TraceOn()) cout << "Close() started" << endl;
  Cleanup();
  RlinkPort::Close();

  if (TraceOn()) cout << "Close() ended" << endl;
  return;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

void RlinkPortCuff::Cleanup()
{
  // close write pipe from user side -> causes event in driver and driver stop
  CloseFd(fFdWrite);
  
  // wait till driver thread terminates
  if (fDriverThread.joinable()) fDriverThread.join();

  // cleanup pipes
  CloseFd(fFdRead);
  CloseFd(fFdReadDriver);
  CloseFd(fFdWriteDriver);

  // cleanup USB context
  if (fpUsbContext) {
    if (fpUsbDevHdl) {
      libusb_release_interface(fpUsbDevHdl, 0);
      libusb_close(fpUsbDevHdl);
      fpUsbDevHdl = nullptr;
    }
    if (fpUsbDevList) {
      libusb_free_device_list(fpUsbDevList, 1);
      fpUsbDevList = nullptr;
    }
    libusb_set_pollfd_notifiers(fpUsbContext, nullptr, nullptr, nullptr);
    libusb_exit(fpUsbContext);
    fpUsbContext = nullptr;
  }

  fPollFds.clear();
  
  return;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

bool RlinkPortCuff::OpenPipe(int& fdread, int& fdwrite, RerrMsg& emsg)
{
  int irc;
  int pipefd[2];

  irc = ::pipe(pipefd);
  if (irc < 0) {
    emsg.InitErrno("RlinkPortCuff::OpenPipe()", "pipe() failed: ", errno);
    return false;
  }
  
  fdread  = pipefd[0];
  fdwrite = pipefd[1];

  return true;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

// executed in separate thread !!
void RlinkPortCuff::Driver()
{
  try {

    // setup USB read-ahead queue
    for (size_t nr=0; nr<kUSBReadQueue; nr++) {
      libusb_transfer* t = libusb_alloc_transfer(0);
      
      t->dev_handle = fpUsbDevHdl;
      t->flags      = LIBUSB_TRANSFER_FREE_BUFFER;
      t->endpoint   = static_cast<unsigned char>(kUSBReadEP|0x80);
      t->type       = LIBUSB_TRANSFER_TYPE_BULK;
      t->timeout    = 0;
      t->status     = LIBUSB_TRANSFER_COMPLETED;
      t->buffer     = reinterpret_cast<unsigned char*>(::malloc(kUSBBufferSize));
      t->length     = kUSBBufferSize;
      t->actual_length = 0;
      t->callback   = ThunkUSBReadDone;
      t->user_data  = this;
      
      int irc = libusb_submit_transfer(t);
      if (irc) BadUSBCall("RlinkPortCuff::Driver()", 
                        "libusb_submit_transfer()", irc);
      fReadQueuePending.push_back(t);
    }

    // event loop
    if (TraceOn()) cout << "event loop started" << endl;
    fLoopState = kLoopStateRunning;
    while(fLoopState == kLoopStateRunning) {
      int irc = ::poll(fPollFds.data(), fPollFds.size(), 1000);
      if (irc==-1 && errno==EINTR) continue;
      if (irc!=0 && TraceOn()) {
        cout << "poll() -> " << irc << " :";
        for (size_t i=0; i<fPollFds.size(); i++)
          if (fPollFds[i].revents) cout << " (" << fPollFds[i].fd << "," 
                                        << fPollFds[i].events << ","
                                        << fPollFds[i].revents << ")";
        cout << endl;
      }
      
      if (irc < 0) BadSysCall("RlinkPortCuff::Driver()", "poll()", irc);
      
      if (fPollFds[0].revents & POLLHUP) {        // write pipe close event
        fLoopState = kLoopStateStopping;
      } else if (fPollFds[0].revents & POLLIN) {  // write pipe data event
        DriverEventWritePipe();
      } else {                              // assume USB timeout events
        DriverEventUSB();
      }
    } 
 
    if (TraceOn()) cout << "event loop ended, cleanup started" << endl;

    for (size_t i=0; i<fWriteQueuePending.size(); i++) {
      libusb_cancel_transfer(fWriteQueuePending[i]);
    }
    for (size_t i=0; i<fReadQueuePending.size(); i++) {
      libusb_cancel_transfer(fReadQueuePending[i]);
    }

    while(fLoopState == kLoopStateStopping &&
          fWriteQueuePending.size() + fReadQueuePending.size() > 0) {
      int irc = ::poll(fPollFds.data()+1, fPollFds.size()-1, 1000);
      if (irc==-1 && errno==EINTR) continue;
      if (irc==0) break;
      if (irc < 0) BadSysCall("RlinkPortCuff::Driver()", "poll()", irc);
      DriverEventUSB();
    }
    if (fWriteQueuePending.size() + fReadQueuePending.size())
      throw Rexception("RlinkPortCuff::Driver()", "cleanup timeout");

    fLoopState = kLoopStateStopped;
    if (TraceOn()) cout << "cleanup ended" << endl;

  } catch (exception& e) {
    cout << "exception caught in RlinkPortCuff::Driver(): '" << e.what() 
         << "'" << endl;
    // close read pipe at driver end -> that causes main thread to respond
    ::close(fFdReadDriver);
    fFdReadDriver = -1;
  }

  return;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

void RlinkPortCuff::DriverEventWritePipe()
{
  libusb_transfer* t = NewWriteTransfer();

  ssize_t ircs = ::read(fFdWriteDriver, t->buffer, kUSBBufferSize);
  if (TraceOn()) cout << "write pipe read() -> " << ircs << endl;
  if (ircs < 0) BadSysCall("RlinkPortCuff::DriverEventWritePipe()",
                           "read()", ircs);

  // pipe closed... end driver event loop
  if (ircs == 0) {
    fLoopState = kLoopStateStopping;
    return;
  }

  t->length = int(ircs);
  int irc = libusb_submit_transfer(t);
  if (irc) BadUSBCall("RlinkPortCuff::DriverEventWritePipe()", 
                      "libusb_submit_transfer()", irc);
  fWriteQueuePending.push_back(t);

  return;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

void RlinkPortCuff::DriverEventUSB()
{
  struct timeval tv;
  tv.tv_sec  = 0;
  tv.tv_usec = 0;
  int irc = libusb_handle_events_timeout(fpUsbContext, &tv);
  //setting the timeval pointer to nullptr should work, but doesn't (in 1.0.6)
  //rc = libusb_handle_events_timeout(pUsbContext, 0);
  if (irc) BadUSBCall("RlinkPortCuff::DriverEventUSB()", 
                      "libusb_handle_events_timeout()", irc);
  return;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

libusb_transfer* RlinkPortCuff::NewWriteTransfer()
{
  libusb_transfer* t = nullptr;
  if (!fWriteQueueFree.empty()) {
    t = fWriteQueueFree.front();
    fWriteQueueFree.pop_front();
  } else {
    t = libusb_alloc_transfer(0);
    t->dev_handle = fpUsbDevHdl;
    t->flags      = LIBUSB_TRANSFER_FREE_BUFFER;
    t->endpoint   = static_cast<unsigned char>(kUSBWriteEP);
    t->type       = LIBUSB_TRANSFER_TYPE_BULK;
    t->timeout    = 1000;
    t->buffer     = reinterpret_cast<unsigned char*>(::malloc(kUSBBufferSize));
    t->callback   = ThunkUSBWriteDone;
    t->user_data  = this;
  }

  t->status        = LIBUSB_TRANSFER_COMPLETED;
  t->length        = 0;
  t->actual_length = 0;

  return t;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

bool RlinkPortCuff::TraceOn()
{
  if (!fUrl.FindOpt("trace")) return false;
  struct timespec ts;
  struct tm tmval;

  ::clock_gettime(CLOCK_REALTIME, &ts);
  ::localtime_r(&ts.tv_sec, &tmval);
  char buf[20];
  ::snprintf(buf, 20, "%02d:%02d:%02d.%06d: ", 
             tmval.tm_hour, tmval.tm_min, tmval.tm_sec, int(ts.tv_nsec)/1000);
  cout << buf;
  return true;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

[[noreturn]]
void RlinkPortCuff::BadSysCall(const char* meth, const char* text, int rc)
{
  stringstream ss;
  ss << rc;
  throw Rexception(meth, string(text) + " failed with rc=" + ss.str(),
                   errno);
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

[[noreturn]]
void RlinkPortCuff::BadUSBCall(const char* meth, const char* text, int rc)
{
  stringstream ss;
  ss << rc;
  throw Rexception(meth, string(text) + " failed with rc=" + ss.str() +
                   " : " + string(USBErrorName(rc)));
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

void RlinkPortCuff::CheckUSBTransfer(const char* meth, libusb_transfer *t)
{
  const char* etext = nullptr;
  
  if (t->status == LIBUSB_TRANSFER_ERROR)     etext = "ERROR";
  if (t->status == LIBUSB_TRANSFER_STALL)     etext = "STALL";
  if (t->status == LIBUSB_TRANSFER_NO_DEVICE) etext = "NO_DEVICE";
  if (t->status == LIBUSB_TRANSFER_OVERFLOW)  etext = "OVERFLOW";

  if (etext == 0) return;

  char buf[1024];
  ::snprintf(buf, 1024, "%s : transfer failure on ep=%d: %s",
             meth, int(t->endpoint&(~0x80)), etext);
  throw Rexception(meth, buf);
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

const char* RlinkPortCuff::USBErrorName(int rc)
{
  // Code taken verbatim from libusb-1.0.9.tar.bz2 function libusb_error_name
  // The libusb_error_name() function was added rather late in V1.0.9.
  // To allow usage with V1.0.8 and earlier this function is include here
  
  switch (rc) {
  case LIBUSB_SUCCESS:
    return "LIBUSB_SUCCESS";
  case LIBUSB_ERROR_IO:
    return "LIBUSB_ERROR_IO";
  case LIBUSB_ERROR_INVALID_PARAM:
    return "LIBUSB_ERROR_INVALID_PARAM";
  case LIBUSB_ERROR_ACCESS:
    return "LIBUSB_ERROR_ACCESS";
  case LIBUSB_ERROR_NO_DEVICE:
    return "LIBUSB_ERROR_NO_DEVICE";
  case LIBUSB_ERROR_NOT_FOUND:
    return "LIBUSB_ERROR_NOT_FOUND";
  case LIBUSB_ERROR_BUSY:
    return "LIBUSB_ERROR_BUSY";
  case LIBUSB_ERROR_TIMEOUT:
    return "LIBUSB_ERROR_TIMEOUT";
  case LIBUSB_ERROR_OVERFLOW:
    return "LIBUSB_ERROR_OVERFLOW";
  case LIBUSB_ERROR_PIPE:
    return "LIBUSB_ERROR_PIPE";
  case LIBUSB_ERROR_INTERRUPTED:
    return "LIBUSB_ERROR_INTERRUPTED";
  case LIBUSB_ERROR_NO_MEM:
    return "LIBUSB_ERROR_NO_MEM";
  case LIBUSB_ERROR_NOT_SUPPORTED:
    return "LIBUSB_ERROR_NOT_SUPPORTED";
  case LIBUSB_ERROR_OTHER:
    return "LIBUSB_ERROR_OTHER";
  }
  return "**UNKNOWN**";
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

void RlinkPortCuff::PollfdAdd(int fd, short events)
{
  fStats.Inc(kStatNPollAddCB);
  pollfd pfd;
  pfd.fd      = fd;
  pfd.events  = events;
  pfd.revents = 0;
  fPollFds.push_back(pfd);
  return;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

void RlinkPortCuff::PollfdRemove(int fd)
{
  fStats.Inc(kStatNPollRemoveCB);
  for (size_t i=0; i<fPollFds.size(); ) {
    if (fPollFds[i].fd == fd) {
      fPollFds.erase(fPollFds.begin()+i);
    } else {
      i++;
    }
  }
  return;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

void RlinkPortCuff::USBWriteDone(libusb_transfer* t)
{
  if (TraceOn()) cout << "USB write done -> " << t->actual_length << endl;

  if (fWriteQueuePending.size() && t == fWriteQueuePending.front()) 
    fWriteQueuePending.pop_front();
  else 
    throw Rexception("RlinkPortCuff::USBWriteDone()",
                     "BugCheck: fWriteQueuePending disordered");

  if (fLoopState == kLoopStateRunning) {
    CheckUSBTransfer("RlinkPortCuff::USBWriteDone()", t);
    fStats.Inc(kStatNUSBWrite);
    fWriteQueueFree.push_back(t);

  } else {
    libusb_free_transfer(t);
  }  
  return;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

void RlinkPortCuff::USBReadDone(libusb_transfer* t)
{
  if (TraceOn()) cout << "USB read done -> " << t->actual_length << endl;

  if (fReadQueuePending.size() && t == fReadQueuePending.front())
    fReadQueuePending.pop_front();
  else 
    throw Rexception("RlinkPortCuff::USBReadDone()",
                     "BugCheck: fReadQueuePending disordered");

  if (fLoopState == kLoopStateRunning) {
    CheckUSBTransfer("RlinkPortCuff::USBReadDone()", t);
    fStats.Inc(kStatNUSBRead);
    if (t->actual_length>0) {
      ssize_t ircs = ::write(fFdReadDriver, t->buffer,
                             size_t(t->actual_length));
      if (ircs < 0) BadSysCall("RlinkPortCuff::USBReadDone()",
                               "write()", ircs);
    }
    
    t->status        = LIBUSB_TRANSFER_COMPLETED;
    t->actual_length = 0;
    int irc = libusb_submit_transfer(t);
    if (irc) BadUSBCall("RlinkPortCuff::USBReadDone()", 
                        "libusb_submit_transfer()", irc);
    fReadQueuePending.push_back(t);

  } else {
    libusb_free_transfer(t);
  }
  
  return;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

void RlinkPortCuff::ThunkPollfdAdd(int fd, short events, void* udata)
{
  RlinkPortCuff* pcntx = reinterpret_cast<RlinkPortCuff*>(udata);
  pcntx->PollfdAdd(fd, events);
  return;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

void RlinkPortCuff::ThunkPollfdRemove(int fd, void* udata)
{
  RlinkPortCuff* pcntx = reinterpret_cast<RlinkPortCuff*>(udata);
  pcntx->PollfdRemove(fd);
  return;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

void RlinkPortCuff::ThunkUSBWriteDone(libusb_transfer* t)
{
  RlinkPortCuff* pcntx = reinterpret_cast<RlinkPortCuff*>(t->user_data);
  pcntx->USBWriteDone(t);
  return;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

void RlinkPortCuff::ThunkUSBReadDone(libusb_transfer* t)
{
  RlinkPortCuff* pcntx = reinterpret_cast<RlinkPortCuff*>(t->user_data);
  pcntx->USBReadDone(t);
  return;
}

} // end namespace Retro
