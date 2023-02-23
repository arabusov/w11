// $Id: RethBuf.ipp 1378 2023-02-23 10:45:17Z mueller $
// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2017-2023 by Walter F.J. Mueller <W.F.J.Mueller@gsi.de>
// 
// Revision History: 
// Date         Rev Version  Comment
// 2023-02-22  1378   1.1    improved Info/Dump methods
// 2018-12-18  1089   1.0.1  use c++ style casts
// 2017-02-25   856   1.0    Initial version
// 2017-02-12   850   0.1    First draft
// ---------------------------------------------------------------------------

/*!
  \brief   Implemenation (inline) of RethBuf.
*/

#include "RethTools.hpp"

// all method definitions in namespace Retro
namespace Retro {

//------------------------------------------+-----------------------------------
//! FIXME_docs

inline void RethBuf::Clear()
{
  fSize = 0;
  return;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

inline void RethBuf::SetSize(uint16_t size)
{
  fSize = size;
  return;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

inline void RethBuf::SetTime()
{
  fTime.GetClock(CLOCK_REALTIME);
  return;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

inline void RethBuf::SetTime(const Rtime& time)
{
  fTime = time;
  return;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

inline uint16_t RethBuf::Size() const
{
  return fSize;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

inline const Rtime& RethBuf::Time() const
{
  return fTime;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

inline const uint8_t* RethBuf::Buf8() const
{
  return fBuf;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

inline const uint16_t* RethBuf::Buf16() const
{
  return reinterpret_cast<uint16_t*>(const_cast<uint8_t*>(fBuf));
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

inline uint8_t* RethBuf::Buf8()
{
  return fBuf;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

inline uint16_t* RethBuf::Buf16()
{
  return reinterpret_cast<uint16_t*>(fBuf);
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

inline void RethBuf::SetMacDestination(uint64_t mac)
{
  RethTools::Mac2WList(mac, Buf16()+kEOffDstMac/2);
  return;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

inline void RethBuf::SetMacSource(uint64_t mac)
{
  RethTools::Mac2WList(mac, Buf16()+kEOffSrcMac/2);
  return;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

inline uint64_t RethBuf::MacDestination() const
{
  return RethTools::WList2Mac(Buf16()+kEOffDstMac/2);
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

inline uint64_t RethBuf::MacSource() const
{
  return RethTools::WList2Mac(Buf16()+kEOffSrcMac/2);
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

inline bool RethBuf::IsMcast() const
{
  return fBuf[0] & 0x1;                     // odd first byte destination MAC 
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

inline bool RethBuf::IsBcast() const
{
  return Buf16()[0] == 0xffff && Buf16()[1] == 0xffff && Buf16()[2] == 0xffff;
}


} // end namespace Retro
