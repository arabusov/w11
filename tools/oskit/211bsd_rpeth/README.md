## Notes on oskit: 2.11BSD system on a RP06 volume with Ethernet

### General remarks
See notes in [w11a_os_guide.md](../../../doc/w11a_os_guide.md) on
  1. I/O emulation setup
  2. FPGA Board setup
  3. Rlink and Backend Server setup
  4. SimH simulator setup
  5. Legal terms

### System properties and intended usage
This system is build with Ethernet based networking via a DEUNA network
interface. The `RETRONFPETH` kernel is configured with
```
    NETHER 1        # ethernet support
    NDE 1           # DEUNA interface driver
    NSL 0           # no slip available
```

### History
For history see [CHANGELOG.md](CHANGELOG.md).

### Installation
A disk set is available from
http://www.retro11.de/data/oc_w11/oskits/211bsd_rpethset.tgz

Download, unpack and copy the disk images (*.dsk), e.g.
```bash
       cd $RETROBASE/tools/oskit/211bsd_rpeth/
       211bsd_rpeth_setup
```

### Usage

- Setup Ethernet environment, see [211bsd_ethernet.md](../doc/211bsd_ethernet.md)
- Start backend server and boot system (see section Rlink in
  [w11a_os_guide](../../../doc/w11a_os_guide.md#user-content-rlink))
  ```
       boot script:  211bsd_rpeth_boot.tcl
       example:      ti_w11 <opt> @211bsd_rpeth_boot.tcl
                     where <opt> is the proper option set for the board.
  ```

- Hit `<ENTER>` in the `xterm` window to connnect to backend server.
  The boot dialog in the console `xterm` window will look like
  (required input is in `{..}`, with `{<CR>}` denoting a carriage return:
  ```
       70Boot from xp(0,0,0) at 0176700
       : {<CR>}
       : xp(0,0,0)unix
       Boot: bootdev=05000 bootcsr=0176700

       2.11 BSD UNIX #2: Thu May 30 10:29:00 PDT 2019
           root@w11a:/usr/src/sys/RETRONFPETH

       phys mem  = 3932160
       avail mem = 3451968
       user mem  = 307200

       May 30 11:34:36 init: configure system

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
       Assuming NETWORKING system ...
       add host w11a: gateway localhost
       add net default: gateway 192.168.2.1
       starting system logger
       checking for core dump... 
       preserving editor files
       clearing /tmp
       standard daemons: update cron accounting.
       starting network daemons: inetd printer.
       starting local daemons:Thu May 30 11:34:48 PDT 2019
       May 30 11:34:48 w11a init: kernel security level changed from 0 to 1

       
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
        48/186 files
        66/208 inodes
        16/150 processes
        11/ 46 texts active,  38 used
         3/135 swapmap entries,  669 kB used, 3510 kB free, 3504 kB max
        34/150 coremap entries, 2417 kB free, 2339 kB max
         1/ 10  ub_map entries,    7    free,    7    max
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
