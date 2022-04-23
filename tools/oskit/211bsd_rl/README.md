## Notes on oskit: 2.11BSD system on RL02 volumes

### General remarks
See notes in [w11a_os_guide.md](../../../doc/w11a_os_guide.md) on
  1. I/O emulation setup
  2. FPGA Board setup
  3. Rlink and Backend Server setup
  4. SimH simulator setup
  5. Legal terms
  
For history see [CHANGELOG.md](CHANGELOG.md).

### Installation
A disk set is available from
http://www.retro11.de/data/oc_w11/oskits/211bsd_rlset.tgz

Download, unpack and copy the disk images (*.dsk), e.g.
```bash
       cd $RETROBASE/tools/oskit/211bsd_rl/
       211bsd_rl_setup
```

### System properties and intended usage
- **minimal** system !! Maybe useful as recovery system.
- was useful at a time when w11a had only RK11 and RL11 type disk support
- `/tmp` stays on '/'
- `/home` is not mounted
- suitable for a 'root' user, other accounts not supported

### Usage

- Start backend server and boot system (see section Rlink in
  [w11a_os_guide](../../../doc/w11a_os_guide.md#user-content-rlink))
  ```
       boot script:  211bsd_rl_boot.tcl
       example:      ti_w11 <opt> @211bsd_rl_boot.tcl
                      where <opt> is the proper option set for the board.
  ```
  or use for verification the SimH simulator  (see section SimH in
  [w11a_os_guide](../../../doc/w11a_os_guide.md#user-content-simh))
  ```
  pdp11 211bsd_rl_boot.scmd
  ```

- Hit `<ENTER>` in the `xterm` window to connnect to backend server.
  The boot dialog in the console `xterm` window will look like
  (required input is in `{..}`, with `{<CR>}` denoting a carriage return:
  ```
       70Boot from rl(0,0,0) at 0174400
       : {<CR>}
       : rl(0,0,0)unix
       Boot: bootdev=03400 bootcsr=0174400

       2.11 BSD UNIX #3: Wed May 29 22:28:05 PDT 2019
           root@w11a:/usr/src/sys/RETRONFPRL

       phys mem  = 3932160
       avail mem = 3577344
       user mem  = 307200

       May 29 22:29:16 init: configure system

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
       preserving editor files
       clearing /tmp
       standard daemons: update cron accounting.
       starting lpd
       starting local daemons:Wed May 29 22:29:26 PDT 2019
       May 29 22:29:26 init: kernel security level changed from 0 to 1
       
       
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
        13/186 files
        49/208 inodes
        11/150 processes
         8/ 46 texts active,  35 used
         3/135 swapmap entries,  655 kB used, 1904 kB free, 1897 kB max
        33/150 coremap entries, 2659 kB free, 2556 kB max
         1/ 10  ub_map entries,   10    free,   10    max
       # {mount}
       /dev/rl0a on /
       /dev/rl1h on /usr
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
         R0: 177560  R1: 161322  R2: 053770  R3: 000010
         R4: 003400  R5: 147510  SP: 147466  PC: 000014
   ```

   will be visible. Now the server process can be stopped with `^D`.
