## Notes on oskit: 2.11BSD system on a RP06 volume - minimal memory system

### General remarks
See notes on
- [I/O emulation setup](../../../doc/w11a_io_emulation.md)
- [FPGA Board setup](../../../doc/w11a_board_connection.md)
- [Rlink and Backend Server setup](../../../doc/w11a_backend_setup.md)
- [SimH simulator setup](../../../doc/w11a_simh_setup.md)
- [Legal terms](../../../doc/w11a_os_guide.md)

### System properties and intended usage
- patch level is #447 plus [fpsim+tcsh patch](https://wfjm.github.io/blogs/211bsd/2017-06-06-kernel-panic-here-doc-tcsh.html)
- for history see [CHANGELOG.md](CHANGELOG.md)
- This system is created for usage on systems with limited memory. The
  `RETRONFPMIN` kernel is configured with
```
    NBUF 40         # only 60 blocks disk cache (usual is 160)
    MAXUSERS 10     # this reduces number of file, process, ... slots
    INET NO         # no networking available
```
- The system will boot on as little as 512 kB memory. Will work reasonably with
  640 kB if only one or two sessions are active. Usage of `tcsh` should be
  avoided because it has a large memory footprint.

### Installation
A disk set is available from
http://www.retro11.de/data/oc_w11/oskits/211bsd_rpminset.tgz

Download, unpack and copy the disk images (*.dsk), e.g.
```bash
       cd $RETROBASE/tools/oskit/211bsd_rpmin/
       211bsd_rpmin_setup
```

### Usage

- Start backend server and boot system (see section Rlink in
  [w11a_os_guide](../../../doc/w11a_os_guide.md#user-content-rlink))
  ```
       boot script:  211bsd_rpmin_boot.tcl
       example:      ti_w11 <opt> @211bsd_rpmin_boot.tcl
                     where <opt> is the proper option set for the board.
  ```
  or use for verification the SimH simulator  (see section SimH in
  [w11a_os_guide](../../../doc/w11a_os_guide.md#user-content-simh))
  ```
  pdp11 211bsd_rpmin_boot.scmd
  ```

- Hit `<ENTER>` in the `xterm` window to connnect to backend server.
  System with as low as 512 kB memory can be used, like in example below.
  The boot dialog in the console `xterm` window will look like
  (required input is in `{..}`, with `{<CR>}` denoting a carriage return:
  ```
       70Boot from xp(0,0,0) at 0176700
       : {<CR>}
       : xp(0,0,0)unix
       Boot: bootdev=05000 bootcsr=0176700

       2.11 BSD UNIX #2: Thu May 30 12:47:59 PDT 2019
           root@w11a:/usr/src/sys/RETRONFPMIN

       phys mem  = 524288
       avail mem = 313536
       user mem  = 307200

       May 30 12:55:30 init: configure system
       
       dz 0 csr 160100 vector 310 attached
       lp 0 csr 177514 vector 200 attached
       rk 0 csr 177400 vector 220 attached
       rl 0 csr 174400 vector 160 attached
       tm 0 csr 172520 vector 224 attached
       xp 0 csr 176700 vector 254 attached
       cn 1 csr 176500 vector 300 attached
       erase, kill ^U, intr ^C
  ```

  In first `'#'` prompt the system is in single-user mode. Just enter a `^D` 
  to continue the system startup to multi-user mode:
  ```
       #^D
       Fast boot ... skipping disk checks
       checking quotas: done.
       Assuming non-networking system ...
       checking for core dump... 
       preserving editor files
       clearing /tmp
       standard daemons: update cron accounting.
       starting lpd
       starting local daemons:Thu May 30 12:55:43 PDT 2019
       
       
       2.11 BSD UNIX (w11a) (console)
       
       login:
  ```

  The login prompt is sometimes mangled with system messages, if its not
  visible just hit `<ENTER>` to get a fresh one.
  ```
       login: {root}
       erase, kill ^U, intr ^C
  ```

  Now the system is in multi-user mode, daemons runnng. You can explore
  the system, e.g. with a `pstat -T` or a `mount` command. The second
  `xterm` can be activated too, it will connect to a second emulated DL11.
  At the end is important to shutdown properly with a `halt`:
  ```
       # {pstat -T}
        16/122 files
        50/128 inodes
        10/ 80 processes
         9/ 36 texts active,  34 used
         6/ 72 swapmap entries,  475 kB used, 3704 kB free, 3673 kB max
        15/ 80 coremap entries,  107 kB free,   43 kB max
         1/ 10  ub_map entries,   25    free,   25    max
       # {mount}
       /dev/xp0a on /
       /dev/xp0c on /tmp
       /dev/xp0d on /home
       /dev/xp0e on /usr
       # {halt}
       syncing disks... done
       halting
  ```

  While the system was running the server process display the
  ```
       cpu0> 
  ```

  prompt. When the w11 has halted after 211bsd shutdown a message like
  ```
       CPU down attention
       Processor registers and status:
         PS: 030350 cm,pm=k,u s,p,t=0,7,0 NZVC=1000  rust: 01 HALTed
         R0: 177560  R1: 010330  R2: 056172  R3: 000010
         R4: 005000  R5: 147510  SP: 147466  PC: 000014
   ```

   will be visible. Now the server process can be stopped with `^D`.
