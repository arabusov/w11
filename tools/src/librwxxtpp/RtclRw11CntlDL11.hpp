// $Id: RtclRw11CntlDL11.hpp 1186 2019-07-12 17:49:59Z mueller $
// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2013-2017 by Walter F.J. Mueller <W.F.J.Mueller@gsi.de>
// 
// Revision History: 
// Date         Rev Version  Comment
// 2017-04-16   878   1.1    derive from RtclRw11CntlTermBase
// 2013-03-06   495   1.0    Initial version
// 2013-02-08   484   0.1    First draft
// ---------------------------------------------------------------------------


/*!
  \brief   Declaration of class RtclRw11CntlDL11.
*/

#ifndef included_Retro_RtclRw11CntlDL11
#define included_Retro_RtclRw11CntlDL11 1

#include "RtclRw11CntlTermBase.hpp"
#include "librw11/Rw11CntlDL11.hpp"

namespace Retro {

  class RtclRw11CntlDL11 : public RtclRw11CntlTermBase<Rw11CntlDL11> {
    public:
                    RtclRw11CntlDL11();
                   ~RtclRw11CntlDL11();

      virtual int   FactoryCmdConfig(RtclArgs& args, RtclRw11Cpu& cpu);
  };
  
} // end namespace Retro

//#include "RtclRw11CntlDL11.ipp"

#endif
