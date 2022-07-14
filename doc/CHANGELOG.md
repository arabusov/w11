# Changelog: w11a_V0.74 -> HEAD

### Table of contents
- Current [HEAD](#user-content-head)
- Release [w11a_V0.791](#user-content-w11a_v0.791)
- Release [w11a_V0.79](#user-content-w11a_v0.79)
- Release [w11a_V0.78](#user-content-w11a_v0.78)
- Release [w11a_V0.77](#user-content-w11a_v0.77)
- Release [w11a_V0.76](#user-content-w11a_v0.76)
- Release [w11a_V0.753](#user-content-w11a_v0.753)
- Release [w11a_V0.752](#user-content-w11a_v0.752)
- Release [w11a_V0.751](#user-content-w11a_v0.751)
- Release [w11a_V0.75](#user-content-w11a_v0.75)
- Release [w11a_V0.742](#user-content-w11a_v0.742)
- Release [w11a_V0.741](#user-content-w11a_v0.741)
- [CHANGELOG for w11a_V.70 to w11a_V0.74](CHANGELOG-w11a_V0.70-w11a_V0.74.md)
- [CHANGELOG for w11a_V.60 to w11a_V0.70](CHANGELOG-w11a_V0.60-w11a_V0.70.md)
- [CHANGELOG for w11a_V.50 to w11a_V0.60](CHANGELOG-w11a_V0.50-w11a_V0.60.md)

<!-- --------------------------------------------------------------------- -->
---
## <a id="head">HEAD</a>
### General Proviso
The HEAD version shows the current development. No guarantees that
software or firmware builds or that the documentation is consistent.
The full set of tests is only run for tagged releases.

### Summary
- cleanup tbrun setup, drop nexys4 and add nexys4d ([see blog](https://wfjm.github.io/blogs/w11/2019-07-27-nexys4-obituary.html))
- add fast mac-only verification codes under tool/tcode, integrated with tbrun
- add ostest support for rsx11m-31_rk, rsx11m-40_rk and rsx11mp-30_rp oskits
- all actively used commands have now a man page
- Doxygen support now for V1.9.4; remove discontinued Tcl support
- build flow Vivado 2022.1 ready; handle synth 8-3331 -> 8-7129 transition
- remove Atlys support (only test designs, a w11 design was never done)
### New features
- new verification codes
  - tools/tcode: fast cpu verification codes
### Changes
- tools changes
  - ci.yml: define TBW_GHDL_OPTS and suppress IEEE package warnings at t=0ms
  - **/tbrun.yml: since nexys4 not longer available switch to nexys4d
  - tools/bin
    - njobihtm: add -n and -h options
    - tbrun_tbwrri: fully implement --r(l|b)mon
    - ti_w11: update --help text, add -ar,-n4d,-bn4d; add -w and -to options
    - tmuconv: add DEUNA defs
  - tools/tcl
    - w11/tcodes.tcl: driver for tcode execution
  - tools/dox
    - w11_(cpp|vhd_all).Doxyfile: for Doxygen V1.9.4
    - w11_tcl.Doxyfile: removed, Tcl support removed in Doxygen V1.8.18
- firmware changes
  - vlib/xlib/bufg_unisim: added, encapulate unisim BUFG
  - removed designs (drop Atlys)
    - rtl/sys_gen/tst_rlink_cuff/atlys/sys_tst_rlink_cuff_atlys
    - rtl/sys_gen/tst_snhumanio/atlys/sys_tst_snhumanio_atlys
### Bug Fixes
  - tools/mcode
    - m9312/bootw11.mac: proper init of unit number in getnam
  - src/librwxxtpp
    - RtclRw11Cpu.cpp: quit before mem write if asm-11 error seen

<!-- --------------------------------------------------------------------- -->
---
## <a id="w11a_v0.791">2022-04-24: [w11a_V0.791](https://github.com/wfjm/w11/releases/tag/w11a_V0.791) - rev 1229(wfjm)</a>
### Summary
- drop Travis (now defunct)
- add GitHub Actions support ([see blog](https://wfjm.github.io/blogs/w11/2022-04-24-ci-cd-workflow-now-github-actions-based.html))
- use Vivado 2020.1 as default
- use GHDL 2.0.0 as default
- use std=c++17 (requires gcc 7.3 or later)
- automate oskit download and container file setup
- automate testing of oskits
- get Nexys A7 board working and fully integrated,
  close [issue #16](https://github.com/wfjm/w11/issues/16)

### New features
- new tools
  - bin/oskit_setup: helper for oskit download and container file setup
  - oskit/\*/\*_setup: scripts for oskit download and container file setup
  - ostest: automation of oskit tests

### Changes
- tools changes
  - RlinkPortCuff,/RtclRusb: drop libusb_set_debug (now deprecated)
  - viv_tools_config.tcl: use open_hw_manager
  - vbomconv: ghdl_m: use -Wl,--no-pie (for UB 18.04 gcc)
- firmware changes
  - nexys4d/mig_a.prj: InputClk 100 MHz
  - tst_mig/nexys4d/sys_tst_mig_n4d: use 100 MHz MIG SYS_CLK; add clock monitor
  - tst_sram/nexys4d/sys_tst_sram_n4d: use 100 MHz MIG SYS_CLK
  - w11a/nexys4d/sys_w11a_n4d: use 100 MHz MIG SYS_CLK
  - */*.vhd: fixes for GHDL V0.36 -Whide warnings

### Bug Fixes
  - nexys4d/mig_a.prj: BUGFIX: SysResetPolarity ACTIVE HIGH

<!-- --------------------------------------------------------------------- -->
---
## <a id="w11a_v0.79">2019-07-27: [w11a_V0.79](https://github.com/wfjm/w11/releases/tag/w11a_V0.79) - rev 1197(wfjm)</a>
### Summary
- use Vivado 2019.1 as default
- finalize coverity fixups, now defect free ([see blog](https://wfjm.github.io/blogs/w11/2019-06-14-coverity-fixes-done.html))
- use SPDX license tags, remove boilerplate license disclaimers ([see blog](https://wfjm.github.io/blogs/w11/2019-07-21-spdx.html))
- TM11: support odd record length, close [issue #8](https://github.com/wfjm/w11/issues/8)

### New features
- new tools
  - ibrtools/Rfd: added, encapsulates a raw file descriptor
  - ibrtools/RfileFd: add, encapsulates basic file operations

### Changes
- tools changes
  - add -reset option to stats subcommand
    - librtools/Rstats: add Reset(); drop operator-=() and operator*=()
    - librtcltools/RtclStats: Rename Collect->Exec, not longer const; add -reset
    - libr\*/*\: Stats() not longer const; use RtclStats::Exec()
  - other backend updates
    - RparseUrl: add DirName,FileName,FileStem,FileType
    - derive ReventFd and RtimerFd from Rfd
    - Rw11VirtDiskFile,Rw11VirtTapeTap: use RfileFd
    - Rw11Cpu: MemWriteByte(): use AddMembe()
    - Rw11CntlTM11: support odd record length, close [issue #8](https://github.com/wfjm/w11/issues/8)
    - *: drop superfluous \file Doxygen tag (clang -Weverything fix)
  - generic_so.mk: use -scrUuv for ar to avoid "'u' modifier ignored"
  - tools/oskit/hook/hook_ibmon_*.tcl: check ibmon availability
  - tap2file: add -v option
- firmware changes
  - sys_w11a_arty: down-rate to 72 MHz, viv 2019.1 fails with 75 MHz
  - sys_w11a_*.vmfset: add new rule for Vivado 2019.1
  - pdp11_vmbox: support membe for em cacc access

### Bug Fixes
- tap2file: correct rlmax calculation
- Rw11CntlTM11: AddNormalExit(): get tmds logic right

<!-- --------------------------------------------------------------------- -->
---
## <a id="w11a_v0.78">2019-06-01: [w11a_V0.78](https://github.com/wfjm/w11/releases/tag/w11a_V0.78) - rev 1158(wfjm)</a>
### Summary
- add m9312 boot prom emulation
- add dz11 8 line serial port multiplexer
- update 211bsd system images (oskits)
  - apply fpsim+tcsh patch ([see blog](https://wfjm.github.io/blogs/211bsd/2017-06-06-kernel-panic-here-doc-tcsh.html))
  - support dz11 lines
  - update network setup

### New features
- new components
  - ibd_m9312: new boot prom emulation
  - ibdr_dz11: 8 line serial port multiplexer
  - backend for DZ11
    - Rw11CntlDZ11: controller and almost all logic
    - Rw11UnitDZ11: unit
- new verification codes
  - test_m9312_all.tcl: tbench for m9312
  - test_dz11_*.tcl: tbench for dz11
- new test and demonstration codes under tools/mcode
  - m9312/bootw11.mac: w11 boot for m9312
  - sys/noboot.mac: boot blocker code for block 0 of disks
  - dz11/dz11echo.mac: port multiplexer echo tester and traffic generator

### Changes
- consolidate DL,LP,PC after lessons learned in DZ implementation
  - use 'fuse' for fifo usage instead of 'size'
  - re-organize rlim handling (move to r.ibsel = '0' section)
- tools changes
  - RtraceTools: TraceBuffer: add level 5 (full word dump)
  - Rw11Cntl: UnitSetupAll(): now virtual
  - Rw11CntlBase: add kNUnit constant
  - Rw11UnitTerm: add detailed stats and StatInc{Rx,Tx}
  - Rw11UnitTermBase: add AttachDone(),DetachDone()
  - Rw11VirtDiskRam: add noboot attach option
  - renamed kw11p tbench from w11a_kw11p to kw11p
- oskit updates
  - 211bsd_rp, see [CHANGELOG](../tools/oskit/211bsd_rp/CHANGELOG.md)
  - 211bsd_rpeth, see [CHANGELOG](../tools/oskit/211bsd_rpeth/CHANGELOG.md)
  - 211bsd_rpmin, see [CHANGELOG](../tools/oskit/211bsd_rpmin/CHANGELOG.md)
  - 211bsd_rk, see [CHANGELOG](../tools/oskit/211bsd_rk/CHANGELOG.md)
  - 211bsd_rl, see [CHANGELOG](../tools/oskit/211bsd_rl/CHANGELOG.md)

### Bug Fixes
- ReventLoop: fixed a design error in the `RemovePollHandler` flow, which
  allowed that `DoPoll` returned while poll list updates were still pending
  and the subsequent execution of `DoCall` called removed handlers. This
  lead to `SIGSEGV` crashes when a `detach` command was executed for a
  a `tcp` type virtual terminal or a `tap` type virtual ethernet device, see
  [issue #14](https://github.com/wfjm/w11/issues/14).

### Known issues

<!-- --------------------------------------------------------------------- -->
---
## <a id="w11a_v0.77">2019-04-28: [w11a_V0.77](https://github.com/wfjm/w11/releases/tag/w11a_V0.77) - rev 1141(wfjm)</a>
### Summary
- add support for buffered character interfaces
  - LP11: ibdr_lp11_buf
  - PC11: ibdr_pc11_buf
  - DL11: ibdr_dl11_buf
- add mcodes: new test and demonstration codes

### New features
- new components
  - fifo_simple_dram: simple fifo with CE/WE interface, dram based
  - ibd_ibtst: ibus tester device
  - ib_rlim_{gen,slv}: new modules for implementation of rate limiters
  - ibdr_lp11_buf: new LP11 interface with fifo buffering
  - ibdr_pc11_buf: new PC11 interface with fifo buffering
  - ibdr_dl11_buf: new DL11 interface with fifo buffering
  - simclkv: test bench clock generator with variable period
- new verification codes
  - w11a_ibtst/*: tbench for ibd_ibtst
  - w11a_ibmon/*: tbench for ibd_ibmon
  - w11a/test_w11a_sdreg.tcl: tbench for sdreg
  - test_lp11_all.tcl: tbench for lp11 and lp11_buf
  - test_pc11_*.tcl: tbench for pc11 and pc11_buf
  - test_dl11_*.tcl: tbench for dl11 and dl11_buf
- new test and demonstration codes under tools/mcode
  - dl11/dl11echo.mac: console interface echo tester
  - sys/blink.mac: 'blinking lights' demo (rsx,bsd and other modes)
- new tools
  - librw11/RtraceTools: some helper methods for buffer tracing
  - ldadump: lda file dumper

### Changes
- tools changes
  - RtclGet.ipp: use const& for oper() of string& and Rtime&
  - RtimerFd,ReventFd: move to librtools
  - RlinkCommand: add BlockDoneAll()
  - RlinkServer: trace now with timestamp and selective
  - Rw11Cpu,RtclRw11Cpu:
    - add ibmon setup and HasIbtst()
    - added defs for w11 cpu component addresses
    - added MemSize() and MemWriteByte()
    - LoadAbs(): return start, better odd byte handling
    - add cp -brf and -bwf; add range checks for cp -wa
    - add ExecWibr(),ExecRibr()
  - Rw11VirtStream: added Error(),Eof()
  - ensure that after aborted rblk only BlockDone words are processed
    - RlinkCommand: Print(): use BlockDone() as length for rblk
    - RlinkConnect: DecodeResponse(): rblk expect check over BlockDone
    - RtclRlinkConnect: M_exec(): tranfer BlockDone values after rblk
    - RtclRw11Cpu: M_cp(): tranfer BlockDone values after rblk
  - Rw11Cntl: add UnitSetup(), UnitSetupAll()
  - Rw11Cntl{DEUNA,DL11,RHRP,RK11,RL11,TM11}: call UnitSetupAll() in Start()
  - Rw11CntlLP11: remove SetOnline(), use UnitSetup()
  - Rw11CntlPC11
    - BootCode(): boot loader rewritten
    - remove SetOnline(), use UnitSetup()
  - asm-11:
    - .end directive autocreates '...end' label
    - print lines with errors to stderr unless -lst seen
  - ti_w11: for -e use .end start address when available
- firmware changes
  - rbd_rbmon: more robust ack,err trace when busy
  - rbd_tester: use now fifo_simple_dram
  - ibd_ibtst: rename dly[rw]->bsy[rw]; datto for write; add datab
  - ibd_kw11p:
    - add csr.ir (rem; as intreq monitor)
    - the source of external events (rate=11) is now selectable vai an new rem
      accessible csr.erate field. options: sysclk, 1 Mhz, extevt, none
  - ibdr_dl11: changes for ibdr_dl11_buf compatibility (val in msb, ib_rlim_slv)
  - ibdr_lp11: move valid bit to msb of buf (for ibdr_lp11_buf compatibility)
  - ibdr_pc11: changes for ibdr_pc11_buf compatibility
  - sys_w11a_s3: set BTOWIDTH 7 (was 6, must be > vmbox atowidth (6))
  - pdp11_sys70: instantiate ibd_ibtst (when sys_conf_ibtst = true)
  - ibdr_maxisys,sys_conf ready for buffered LP11,PC11 and DL11
    - use type code instead of boolean for sys_conf_ibd_{dl11,lp11,pc11}
    - add sys_conf_ibtst (enabled in all systems)
    - add sys_conf_ibd_dz11 (enabled in all systems)
    - add ib_rlim_gen to support rate limiters
    - instantiate ibd_ibtst
    - instantiate ibdr_lp11_buf
    - instantiate ibdr_pc11_buf

### Bug Fixes
- backend code: some getters crashed with `SIGSEGV`, see
  [issue #19](https://github.com/wfjm/w11/issues/19). The culprit was the
  automatic return type determination for the getter lambdas. Changed back
  to using bind, now `std::bind`, for method forwarders. This is safer and
  also more compact.
- RlinkContext: use now proper polarity of status mask. Was wrong since
  2015-05-12 release w11a_V0.753 which inverted the mask polarity. Had no
  practical consequences, went therefore undetected for such a long time.
- RtclRw11Cpu: now proper cpu attn test in the server inactive case
- pdp11_irq,ib_intmap24: now proper EI_ACK routing, for all detail see
  [ECO-030](ECO-030-EI_ACK-misroute.md).

### Known issues

<!-- --------------------------------------------------------------------- -->
---
## <a id="w11a_v0.76">2019-02-16: [w11a_V0.76](https://github.com/wfjm/w11/releases/tag/w11a_V0.76) - rev 1108(wfjm)</a>
### Summary
- add support for DDR memory via Vivado MIG cores for
  - Digilent Arty
  - Digilent Nexys4 DDR (or Nexys A7-100)
  - Digilent Arty S7
- add a low level MIG interface test design `sys_tst_mig_*`
- update tool support
  - all designs build under Vivado 2017.2 and 2018.3
  - sys_w11a_as7 must be build under 2018.3 (due to MIG support constraints)
  - sys_w11a_arty runs only with 75 MHz under 2018.3
- 2018.3 is slower and for w11a less efficient, so 2017.2 stays default
- remove iist from Spartan-3,6 designs (will never be used on small FPGAs)

### New features
- new systems
  - for Digilent Arty (classic and A7)
    - sys_tst_mig_arty: low level MIG interface test
    - sys_tst_sram_arty: memory test
    - sys_w11a_arty: w11a with full 3840 MB memory
  - for Digilent Nexys4 DDR
    - sys_tst_mig_n4d: low level MIG interface test
    - sys_tst_sram_n4d: memory test
    - sys_w11a_n4d: w11a with full 3840 MB memory
  - for Digilent Arty S7
    - sys_w11a_as7: w11a with full 3840 MB memory
- new components
  - s7_cmt_sfs_2: dual-channel frequency synthesis MMCM/PLL wrapper
  - s7_cmt_1ce1ce2c: clocking block for 7-Series: 2 clk+CEs + 2 clk
  - cdc_signal_s1_as: clock domain crossing for a signal, 2 stage, asyn input
  - cdc_pulse: clock domain crossing for a slowly changing value
  - migui_core_gsim: highly simplified MIG UI simulation model
  - sramif2migui_core: w11a SRAM to MIG UI interface core
  - sramif_mig_arty: SRAM to DDR via MIG adapter for arty

### Changes
- general
  - travis:
    - use make -j 2 and tbrun -j 2 (VM has 2 cores, one real, one HT)
    - use xenial in matrix
  - Makefile: drop boost includes and libs (boost not used anymore)
- tools changes
  - viv_tools_build.tcl
    - export log and rpt generated in OOC synthesis runs
    - downgrade SSN critical warnings to warnings
  - tbrun: add --list option
  - ti_w11: add add -ar,-n4d (ddr versions)
- firmware changes
  - cdc_vector_s0: add ENA port (now used in cdc_pulse)

### Bug Fixes
- nexys4d_pins.xdc: BUFFIX: Fix faulty IO voltage for I_SWI[8,9]

### Known issues
- Nexys4 DDR and Arty S7 designs only simulation tested
  (see [#16](https://github.com/wfjm/w11/issues/16) and
  [#17](https://github.com/wfjm/w11/issues/17) )
- w11 clock rate limited by CACHE-to-CACHE false path
  (see [#18](https://github.com/wfjm/w11/issues/18) )

<!-- --------------------------------------------------------------------- -->
---
## <a id="w11a_v0.753">2018-12-29: [w11a_V0.753](https://github.com/wfjm/w11/releases/tag/w11a_V0.753) - rev 1096(wfjm)</a>
### Summary
- add continuous integration support via [Travis CI](https://travis-ci.org),
  add [project wfjm/w11](https://travis-ci.org/wfjm/w11), and setup
  a `.travis.yml` ([see blog](https://wfjm.github.io/blogs/w11/2018-09-16-travis-based-ci-cd-workflow.html)).
- use static source code analysis [Coverity Scan](https://scan.coverity.com),
  add [project wfjm/w11](https://scan.coverity.com/projects/wfjm-w11).
  The scans are manually uploaded, not automated via Travis (w11 is a `VHDL`
  project after all, so C++ backend code doesn't change so often). Coverity
  found a modest number of defects,  most uncritical, but some real bugs.
- the Coverity results triggered a general backend code review
  - fix coverity detected defects
  - get backend code `-Wall -Wextra -Wpedantic` clean
  - exploit c++11 language constructs (e.g. emplace,lambda,auto,move,...)
  - completely replace boost with std
- add KW11-P (programmable clock) to all w11 systems. It is usefull in test
  benches (fast interrupt source) and enables on the long run to port the
  2.10BSD kernel profiling code to 2.11BSD ([see blog](https://wfjm.github.io/blogs/w11/2018-10-14-kw11-p-added.html))
- stay with Vivado 2017.2 as default tool, 2017.2 to 2018.2 exhibit much
  longer build times for w11 designs ([see blog](https://wfjm.github.io/blogs/w11/2018-09-01-vivado-2018.2-much-slower.html))
- the first Artix-7 designs for the nexys4 board where done in 2013 with
  ISE 14.5, later with 14.7, simply because the early Vivado versions were
  nice technology demonstrators, but didn't compile the w11 code base. That
  changed with Vivado 2014.3, and since 2015 ISE wasn't used for 7Series. So
  it's time to remove the ISE build support for the nexys4 designs.

### New features
- travis support via `.travis.yml`
  - compiles the C++ backend
  - download the `GHDL` based test benches (can't be build under Travis)
  - execute the test benches with `tbrun`
- add KW11-P support, enable it in all w11a systems
- add performance counters
  - pdp11_dmpcnt: an array of 32 counters of 32 bit width
  - connected to 24 signals from inside pdp11_sys70 and 8 signals from outside
  - dmpcntanal: analysis script
- add s7_cmt_1ce1ce: clock generator block used in many 7-Series designs
- add new disk scheme ram: (with Rw11VirtDiskRam)
  - implements a ram-only-disk
  - generates create_disk compatible test patterns

### Changes
- general
  - remove ISE 14.x build support for 7Series (mostly nexys4 designs)
  - Makefile: `make all_tcl` now quiet, use setup_packages_filt
- tools changes
  - vbomconv: now allows to inject Tcl scripts into the Vivado project setup
    sequence via the `@tcl` directive.
  - xviv_msg_filter
    - display INFO Common 17-14 'further message disabled'
    - add c type rules for 'count-only' filters
    - add support for bitstream generation checking ([bit] section)
  - tbrun:
    - add --all option
    - show correct 'found count' in summary message
  - (all perl scripts): add and use bailout instead of die
  - viv_tools_build.tcl: increase message limits (all 200, some 5000)
  - rw11/shell.tcl: add workaround for tclreadline and `after` interference
- firmware changes
  - sys_w11_n4: reduce cache from 64 to 32 kB to keep timing closure
  - changes for DM_STAT_* signals (debug and monitoring)
    - DM_STAT_SE: add elements cpbusy,idec,pcload,itimer
    - DM_STAT_CA: added, used for cache monitoring
    - DM_STAT_SY: removed, now replaced by DM_STAT_CA
    - DM_STAT_EXP: added, for signals exported by pdp11_sys70
  - pdp11_sys70:
    - instantiate pdp11_dmpcnt, setup performance counter sigs
    - drop ITIMER,DM_STAT_DP, use DM_STAT_EXP, add PERFEXT port
  - pdp11_sequencer: drive DM_STAT_SE.(cpbusy,idec,pcload,itimer), drop ITIMER
  - pdp11_cache: drop CHIT, add DM_STAT_CA port, add detailed monitoring
  - pdp11_tmu(_sb): use DM_STAT_CA instead of DM_STAT_SY
  - ibdr_maxisys: add IDEC port, connect to EXTEVT of KW11P
  - sys_w11a_*.vhd: use DM_STAT_EXP; IDEC to maxisys; setup PERFEXT
  - sfs_gsim_core: new common simulation core
    - use in {dcm,s6_cmt,s7_cmt}_sfs_gsim simulation models
    - use in rtl/bplib/\*/tb/tb_\* test benches
    - remove s7_cmt_sfs_tb
  - tbcore_rlink: wait 40 cycles after CONF_DONE
  - serport_master_tb: add 100 ps RXSD,TXSD delay to allow clock jitter
- tbench changes
  - tst_sram: don't test memory controller reset anymore
- backend changes
  - RtclRw11Unit: fix for clang: M_virt() now public
  - Rw11VirtDisk: keep track of disk geometry
- backend code review
  - use for C++ compiles `-Wpedantic` (in addition to `-Wall` and `-Wextra`)
  - fixes for uninitialized variables (coverity, all uncritical)
  - catch exceptions in dtors (coverity, use Catch2Cerr)
  - use `[[noreturn]]` (clang -Wmissing-noreturn)
  - drop never reached returns (clang -Wunreachable-code-return)
  - drop `throw()` lists, use `noexcept` (clang -Wdeprecated)
  - add `R*_Init` prototypes (clang -Wmissing-prototypes)
  - now -Wunused-parameter clean (comment out unused params)
  - now -Wunused-variable clean (comment out so far unused code)
  - move `using namespace std` after includes (clang warning)
  - some selected clang -Weverything aspects
    - use c++ style casts (gcc/clang -Wold-style-cast)
    - now -Wdocumentation clean (some wrong doxygen trailing comments)
    - rename variables in shadow situations (-Wshadow)
    - add casts (-Wfloat-conversion, -Wdouble-promotion)
    - make Dump non virtual in some cases (-Wnon-virtual-dtor)
  - use c++11 language features
    - use `nullptr` instead of plain '0'
    - use auto, emplace() and range loops
    - use unique_ptr instead of free pointers, avoid explicit `delete`
    - add and use move semantic
    - use constructor delegation
    - use range loops
    - use std::boolalpha; add and use Rprintf(bool);
  - completely replace boost with std
    - use std::unique_ptr instead of boost::scoped_ptr
    - use std::shared_ptr instead of boost
    - use std::function instead of boost
    - use std::bind or in most cases a lambda, instead of boost::bind
    - use std::thread instead of boost
    - use mutex and friends from std:: instead from boost::
      - use std::mutex
      - use std::recursive_mutex
      - use std::condition_variable
      - use std::lock_guard
    - use =delete for noncopyable instead of boost
  - reduce usage of pointers in APIs
    - add HasPort/HasVirt(); Port() and Virt() return reference

### Bug Fixes
- RtclArgs.hpp: BUGFIX: get *_min limits correct (gcc -Wpedantic)
- RtclArgs.cpp: BUGFIX: GetArg(): argument in wrong order (coverity)
- Rw11CntlDEUNA.cpp: BUGFIX: SetMacDefault(): resource leak (coverity)
- Rw11VirtDiskFile.cpp: BUGFIX: Open(): resource leak (coverity)
- Rw11VirtEthTap.cpp: BUGFIX: buffer not null terminated (coverity)
- Rw11VirtTapeTap.cpp:
  - BUGFIX: Open(): resource leak (coverity)
  - BUGFIX: Rewind(): bad constant expression (coverity)

### Known issues

<!-- --------------------------------------------------------------------- -->
---
## <a id="w11a_v0.752">2018-08-26: [w11a_V0.752](https://github.com/wfjm/w11/releases/tag/w11a_V0.752) - rev 1041(wfjm)</a>
### Summary
- the Arty board is now also offered with a Spartan-7 FPGA. To evaluate the
  Spartan vs Artix performance a w11a port to the Arty S7 board was added.
  The design runs with 80 MHz, same clock rate as achieved with Artix-7 FPGAs.
  _Note_: the design is only simulation tested, was _not FPGA tested_ !!
- use Vivado 2017.2 as default (needed for Spartan-7 support). All Vivado
  versions from 2017.3 to 2018.2 were tested. All designs build properly under
  Vivado 2018.2, but the CPU time for a build increased very substantially,
  so they are currently not used as default build tool.

### New features
- Add Digilent Arty A7 (50 die size) support
  - general board support (for rev E board)
  - rgbdrv_3x2mux.vhd : driver for array with 2 RGB LEDs
  - add systems
    - w11a: w11a system with 256 kB memory (from BRAM) (_only sim tested_)
### Changes
- xviv_msg_filter: allow {yyyy.x} tags (in addition to ranges)
- xviv_msg_summary: check also for .bit and .dcp files
- get Vivado 2017.2 ready (needed for Spartan-7 support)
- test Vivado 2017.3 - 2018.2 ready
  - *.vmfset: update rules to cover 2017.4-2018.2
  - all designs build under 2017.2 and 2018.2

<!-- --------------------------------------------------------------------- -->
---
## <a id="w11a_v0.751">2018-08-10: [w11a_V0.751](https://github.com/wfjm/w11/releases/tag/w11a_V0.751) - rev 1037(wfjm)</a>

### Summary
- The license disclaimers in the source files referred so far to GPL V2 or later
  They are now consistent with the License.txt file, which refers to GPL V3.
- Add Digilent Cmod A7 (35 die size) support
- Add [INSTALL_quickstart](INSTALL_quickstart.md)
- get Vivado 2017.1 ready
- Added Unix 7th Edition oskit; rename 5th Edition kit
  - u5ed_rk: renamed from unix-v5_rk
  - u7ed_rp: added, very preliminary, boots on Cmod A7, further testing needed

### New features
- Add Digilent Cmod A7 (35 die size) support
  - general board support
  - c7_sram_memctl: SRAM memory controller (incl tb)
  - is61wv5128bll: simple memory model (incl tb)
  - sn_humanio_emu_rbus: human IO emulator
  - 92-retro-usb-persistent.rules: add more board rules
  - associated changes
    - sn_humanio_rbus: add stat_rbf_emu (=0); single cycle btn pulses
    - rgbdrv_analog(_rbus): add ACTLOW generic to invert output polarity
    - ti_rri: adopt Digilent autodetect for Cmod A7
  - add systems
    - tst_rlink: rlink tester
    - tst_sram: SRAM tester
    - w11a: w11a system with 672 kB memory (512 SRAM + 160 BRAM)
- rtl/vlib/rutil.vhd: added package, with imin helper function

### Changes
- cleanups
  - s3_sram_memctl: drop superfluous idata_cei=1 in s_write2
- 17bit support for tst_sram
  - tst_sram.vhd: allow AWIDTH=17; sstat_rbf_awidth instead of _wide
  - tcl/tst_sram/*.tcl: 17bit support; use sstat(awidth); add isnarrow
- get Vivado 2017.1 ready
  - xviv_msg_filter: add version-range tag support
  - *.vmfset:
    - drop the nonsense 'Synth 8-6014' messages
    - adopt to different path used by 'Synth 8-3332' messages
- Rw11VirtDiskOver: more detailed stats
- add test_w11a_mem70.tcl; retire old tests tb_w11a_mem70*.dat

### Bug Fixes
- shell_egd.tcl: BUGFIX: shell_pspec_map: fix mapping for addr>20000

### Known issues

<!-- --------------------------------------------------------------------- -->
---
## <a id="w11a_v0.75">2017-06-04: [w11a_V0.75](https://github.com/wfjm/w11/releases/tag/w11a_V0.75) - rev 904(wfjm)</a>

### Summary
- the only device class missing so far for the w11 was *network interfaces*.
  This release adds a preliminary and functionally restricted `DEUNA` Ethernet
  interface and thus for the the first time full networking support for 211bsd.
  The provisos for the current implementation are
  - no buffer chaining
  - no loopback
  - no memory access error checking
  - works with 211bsd: ping, telnet, and ftp tested
  - RSX11-M uses buffer chaining, will definitively not work
- move to Ubuntu 16.04 as development platform
  - document urjtag build (jtag in Ubuntu 16.04 is broken)
  - add environment sanity wrappers for acroread,awk,firefox to ensure
    proper operation of Vivado under Ubuntu 16.04
  - use -std=c++11 (gcc 4.7 or later)
  - for all FTDI USB-UART it is essential to set them to `low latency` mode.
    That was default for Linux kernels 2.6.32 to 4.4.52. Since about March
    2017 one gets kernels with 16 ms default latency again, thanks to
    [kernel patch 9589541](https://patchwork.kernel.org/patch/9589541/).
    **For newer systems it is essential to install a udev rule** which
    automatically sets low latency, see [documentation](../tools/sys/README.md).
- the cpu monitor dmcmon was not available in Vivado systems due to synthesis
  issues caused by dmscnt. Is resolved, dmcmon now part of sys_w11a_n4.
- many improvements to the w11 shell in ti_w11: rbmon integrated, usage
  of ibmon and dmcmon streamlined.
- add *overlay* file system (Rw11VirtDiskOver, scheme over:) which keeps all
  write in backend memory. Very convenient for development. The changes can
  be written to the disk image with a tcl level command (cpu0* virt flush).
- add two new 211bsd system images (oskits)
  - 211bsd_rpmin: for small memory systems (512 or 672 kB)
  - 211bsd_rpeth: with DEUNA Ethernet support
- cleanup 211bsd system images (oskits)
  - 211bsd_rp, the master, see [CHANGELOG](../tools/oskit/211bsd_rp/CHANGELOG.md)
  - 211bsd_rk, see [CHANGELOG](../tools/oskit/211bsd_rl/CHANGELOG.md)
  - 211bsd_rl, see [CHANGELOG](../tools/oskit/211bsd_rk/CHANGELOG.md)
- man pages now [available online](http://www.retro11.de/manp/w11/man/cat1/).

### New features
- add DEUNA (Ethernet) support
  - add DEUNA device (xu) for ibus
  - add DEUNA to all sys_w11a systems
    - add ibdr_deuna to maxisys
    - setup sys_conf for all systems
  - backend support classes for networking
    - RethBuf: Ethernet buffer
    - RethTools: some handy tools
    - Rw11VirtEth: new virt base for Ethernet
    - Rw11VirtEthTap: concrete networking via tap devices
  - backend for DEUNA
    - Rw11CntlDEUNA: controller and almost all logic
    - Rw11UnitDEUNA: unit
  - tcl support for DEUNA
  - tbench support for DEUNA
  - some new preinit and preboot hooks
- tools for setting up Ethernet bridge and tap
  - add ip_create_br: create bride and convert default Ethernet interface
  - add ip_create_tap: create use-mode tap device
  - add ip_inspect: helper script
- update USB serial latency handling
  - 91-retro-usb-latency.rules: udev rule to set low latency for FDTI USB UART
  - 92-retro-usb-persistent.rules: udev rule for persistent device names
  - 99-retro-usb-permissions.rules renamed to 90-retro-usb-permissions.rules
- add Rw11VirtDiskOver (simple overlay file container)
  - Rw11VirtDiskBuffer: added, disk buffer representation
  - Rw11VirtDiskOver: added, a 'keep changes in memory' overlay file container
  - Rw11Virt: add fWProt,WProt()
  - Rw11VirtDiskFile: adopt WProt handling
  - RtclRw11Unit: add fpVirt,DetachCleanup(),AttachDone(),M_virt()
  - RtclRw11UnitBase: add AttachDone()
- RtimerFd: first practical version
- Rtime: class for absolute and delta times

### Changes
- sys_w11a_n(2|3): use SWI(7:6) to allow fx2 debug via LEDs
- sys_tst_sram_n4: add sysmon_rbus
- 23 line interrupt mapper for full system configuration
- revise interface for ibd_ibmon and rbd_rbmon
  - use start,stop,suspend,resume functions; improved stop on wrap handling
  - add 'repeat collapse' logic (store only first and last of a sequence)
- refurbish dmcmon
  - has now the sta,sto,sus,res logic as rbmon and ibmon
  - does not depend on full state number generation anymore
  - missed WAIT instructions so far, has been fixed
- dmcmon included in sys_w11a_n4 again
  - full snum generation code gives bad synthesis under Vivado (fine in ISE)
  - the updated dmcmon can life with a simple, category based, snum
- move hook_*.tcl files to tools/oskiit/hook directory
- w11 shell .bs now support ibus register names and ranges
  - rw11/dmhbpt.tcl: hb_set: use imap_range2addr, allow regnam and range
- integrate rbus monitor in w11 shell
  - ti_rri: setup rbus monitor if detected
  - rw11/shell.tcl: add .rme,.rmd,.rmf,.rml
  - ibd_ibmon/util.tcl: move out imap_reg2addr
  - rbmoni/util.tcl: add procs filter,rme,rmf
  - rlink/util.tcl: add amap_reg2addr
  - rw11/util.tcl: move in imap_reg2addr; add imap_range2addr
  - rw11/shell.tcl: integrate rbmon: add .rme,.rmd,.rmf,.rml
- update probe handling
  - probe/setup auxiliary devices: kw11l,kw11p,iist
  - keep probe data, make it tcl gettable
- re-arrange rawio commands for rlc and rlp
  - RtclRlink(Connect|Port): drop M_rawio; add M_rawread,M_rawrblk,M_rawwblk
  - RtclRlinkPort: LogFileName(): returns now const std::string&
- make setup procs idempotent
  - RlinkConnect: add rbus monitor probe, add HasRbmon()
  - RtclRlinkConnect: M_amap: -testname opt addr check; add hasrbmon get
  - RtclRw11Cpu: M_(imap|rmap): -testname optional addr check
  - */util.tcl: setup: now idempotent
- factor out controller class specifics; add useful M_default output
  - RtclRw11Cntl*Base: add classes with Rdma,Disk,Stream.Tape,Term specifics
  - RtclRw11Cntl*: add class in ctor; derive from RtclRw11Cntl*Base
- make list cpus,cntls,units command gettable; make controller class gettable
  - RtclRw11: add CpuCommands() and cpus getter
  - RtclRw11Cntl: add UnitCommands() and uints and class getter
  - RtclRw11Cpu: add ControllerCommands() and cntls getter
  - RtclGet: add Tcl_Obj* getter
- make attach status and attach url gettable for units
  - Rw11Cntl,Rw11CntlBase: NUnit() now pure; add UnitBase()
  - Rw11Unit: add IsAttached(), AttachUrl()
  - Rw11UnitVirt: add VirtBase()
  - Rw11Virt: add Url() const getter
  - RtclRw11UnitBase: add attached,attachutl getters
- ensure that defaulted scheme visible in displayed open urls
  - pass default scheme to RparseUrl in Open()
  - add Open() overloads for Rw11VirtDiskFile and Rw11VirtDiskOver
  - RparseUrl: add Set() with default scheme handling
- remove double inheritance in RtclRw11Unit* stack
  - RtclRw11Unit: drop fpCpu, use added Cpu()=0 instead
  - RtclRw11UnitBase: add TUV,TB; add TUV* ObjUV(); inherit from TB
  - RtclRw11Unit(Disk|Stream|Tape|Term): define ObjUV();inherit from RtclRw11Unit
  - RtclRw11Unit(dev): inherit from RtclRw11UnitBase
- update trace output handling
  - Rw11Cntl*: use controller name as message prefix
  - Rw11CntlPC11,Rw11CntlDL11: trace received chars
- more compact dumps, add Dump(..,detail)
  - RlinkCommand: add CommandInfo()
  - RtclCmdBase: add GetArgsDump()
  - RlinkCommandList: Dump(): add detail arg
  - Rstats: add NameMaxLength(); Print(): add counter name
  - Rw11Cntl: use Dump(detail) for PrimClist ect
  - Rw11(Cntl|Unit|Virt)*,: Dump(): add detail arg
  - many other classes: Dump(): add detail arg
  - RtclRw11(Cntl|Unit|*): M_dump: use GetArgsDump and Dump detail
- update time handling
  - use clock_gettime instead of gettimeofday
  - add Rtime support in RtclGet/RtclSet
  - use Rtime; drop Rtools::TimeOfDayAsDouble()
- tcl command handling update
  - support now sub-command handling
  - support dynamically created commands (like 'virt')
  - support command info (via '?' option)
- Auto-detection of Digilent boards with `FT2232HQ` interface for
  [ti_rri](http://www.retro11.de/manp/w11/man/cat1/ti_rri.0.html) and
  [ti_w11](http://www.retro11.de/manp/w11/man/cat1/ti_w11.0.html).
- miscellaneous fixes and changes
  - svn_set_ignore: check svn:ignore existence before reading it
  - telnet_wrapper: add 'r' --> reset and stty sane handling
  - Makefile: add all_tcl to all; use njobihtm
  - tb_rlink_stim.dat: start section B (error aborts) and C (re-transmit)
  - ticonv_rri: use 'rlc rawwblk' instead of 'rlc rawio -wblk'
  - rbmoni/test_regs.tcl: add data/addr logic tests
  - librw11/Rw11Cpu: add ModLalh()
  - librtools/Rstats: add IncLogHist() and fix + and * operator definition

### Bug Fixes
- rlink_core: BUGFIX: correct re-transmit after nak aborts
- resolve hangup of fx2 USB controller
  - was caused by inconsistent use of rx fifo thresholds
  - adding more lines to monitor output (fsm_* lines for state tracking)
- RlinkPort: BUGFIX: RawRead(): proper irc for exactsize=false
- Rexception: BUGFIX: add fErrtxt for proper what() return
- `rlc get logfile` or `rlc get *` crashed with segfault
  - error was a type mismatch in the getter declaration in RtclRlinkConnect
  - fixed by changing the return type in RlinkConnect

### Known issues

<!-- --------------------------------------------------------------------- -->
---
## <a id="w11a_v0.742">2017-01-07: [w11a_V0.742](https://github.com/wfjm/w11/releases/tag/w11a_V0.742) - rev 841(wfjm)</a>

### Summary
- fixes for Vivado 2016.4; all designs build under Vivado 2016.4
- added **preliminary** support for Nexys4 DDR board (thanks to [Michael Lyle](https://github.com/mlyle) for testing!)
- [w11 shell](../tools/tcl/rw11/shell.tcl) re-organized and expanded,
  now default in [ti_w11](../tools/bin/ti_w11)
- `CPUERR` cleared with cpu reset (see [ECO-029](ECO-029-cpuerr_creset.md))
- `PS` initialized now with `000000` (see [ECO-028](ECO-028-ps_init.md))

<!-- --------------------------------------------------------------------- -->
---
## <a id="w11a_v0.741">2016-12-23: [w11a_V0.741](https://github.com/wfjm/w11/releases/tag/w11a_V0.741) - rev 826(wfjm)</a>
### Summary
- moved w11 repository from OpenCores to GitHub
  [wfjm/w11](https://github.com/wfjm/w11/) ([see blog](https://wfjm.github.io/blogs/w11/2016-12-11-w11-moved-to-github.html))
- moved w11 project pages from OpenCores to GitHub-Pages
  [wfjm.github.io/home/w11](https://wfjm.github.io/home/w11/)
- converted existing documentation from plain text to markdown
- added README.md files in sub-directories
