// $Id: RlinkPacketBufRcv.ipp 1186 2019-07-12 17:49:59Z mueller $
// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2014- by Walter F.J. Mueller <W.F.J.Mueller@gsi.de>
// 
// Revision History: 
// Date         Rev Version  Comment
// 2014-11-23   606   1.0    Initial version
// 2014-11-02   600   0.1    First draft (re-organize PacketBuf for rlink v4)
// ---------------------------------------------------------------------------

/*!
  \brief   Implemenation (inline) of class RlinkPacketBuf.
*/

// all method definitions in namespace Retro
namespace Retro {

//------------------------------------------+-----------------------------------
//! FIXME_docs

inline bool RlinkPacketBufRcv::CheckSize(size_t nbyte) const
{
  return fPktBuf.size()-fNDone >= nbyte;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

inline void RlinkPacketBufRcv::GetWithCrc(uint8_t& data)
{
  data = fPktBuf[fNDone++];
  fCrc.AddData(data);
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

inline void RlinkPacketBufRcv::GetWithCrc(uint16_t& data)
{
  uint8_t datl = fPktBuf[fNDone++];
  uint8_t dath = fPktBuf[fNDone++];
  fCrc.AddData(datl);
  fCrc.AddData(dath);
  data = uint16_t(datl) | (uint16_t(dath) << 8);
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

inline bool RlinkPacketBufRcv::CheckCrc()
{
  uint8_t  datl = fPktBuf[fNDone++];
  uint8_t  dath = fPktBuf[fNDone++];
  uint16_t data = uint16_t(datl) | (uint16_t(dath) << 8);
  return data == fCrc.Crc();
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

inline int RlinkPacketBufRcv::NakIndex() const
{
  return fNakIndex;
}

} // end namespace Retro
