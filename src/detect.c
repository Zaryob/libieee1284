/*
 * libieee1284 - IEEE 1284 library
 * Copyright (C) 2001, 2002  Tim Waugh <twaugh@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "config.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "debug.h"
#include "detect.h"

#ifdef HAVE_LINUX
#include <sys/io.h>
#include "ppdev.h"
#elif defined(HAVE_OBSD_I386)
/* for i386_get_ioperm and i386_set_ioperm */
#include <machine/sysarch.h>
#elif defined(HAVE_SOLARIS)
#include <sys/ddi.h>
#include <sys/sunddi.h>
#elif defined(HAVE_CYGWIN_NT)
#include <w32api/windows.h>
#endif


int capabilities;

/* Look for parport entries in /proc.
 * Linux 2.2.x has /proc/parport/.
 * Linux 2.4.x has /proc/sys/dev/parport/. */
static int
check_proc_type (void)
{
  int which = 0;
  struct stat st;
  if (stat ("/proc/sys/dev/parport", &st) == 0 &&
      S_ISDIR (st.st_mode) &&
      st.st_nlink > 2)
    {
      which = PROC_SYS_DEV_PARPORT_CAPABLE;
      dprintf ("This system has /proc/sys/dev/parport\n");
    }
  else if (stat ("/proc/parport", &st) == 0 &&
	   S_ISDIR (st.st_mode) &&
	   st.st_nlink > 2)
    {
      which = PROC_PARPORT_CAPABLE;
      dprintf ("This system has /proc/parport\n");
    }
  capabilities |= which;
  return which;
}

/* Try to find a device node that works. */
static int
check_dev_node (const char *type)
{
  char name[50]; /* only callers are in detect_environment */
  int fd;
  int i;

  for (i = 0; i < 8; i++) {
    sprintf (name, "/dev/%s%d", type, i);
    fd = open (name, O_RDONLY | O_NOCTTY);
    if (fd >= 0) {
#ifdef HAVE_LINUX
      if (!strcmp (type, "parport"))
	{
	  /* Try to claim the device.  This will
	   * force the low-level port driver to get loaded. */
	  if (ioctl (fd, PPCLAIM) == 0)
	    ioctl (fd, PPRELEASE);
	}
#endif

      close (fd);
      dprintf ("%s is accessible\n", name);
      return 1;
    }
  }

  return 0;
}

/* Is /dev/port accessible? */
static int
check_dev_port (void)
{
  int fd = open ("/dev/port", O_RDWR | O_NOCTTY);
  if (fd >= 0) {
    close (fd);
    capabilities |= DEV_PORT_CAPABLE;
    dprintf ("/dev/port is accessible\n");
    return 1;
  }
  return 0;
}

/* Can we use direct I/O with inb and outb? */
static int
check_io (void)
{
  #ifdef HAVE_OBSD_I386
  u_long *iomap;
  if ((iomap = malloc(1024/8)) == NULL) return 0;
  if ((i386_get_ioperm(iomap) == 0) && (i386_set_ioperm(iomap) == 0)) {
    capabilities |= IO_CAPABLE;
    dprintf ("We can use i386_get_ioperm()\n");
    free(iomap);
    return 1;
  }
  free(iomap);
  #elif defined(HAVE_FBSD_I386)
  int fd;
  if ((fd = open("/dev/io", O_RDONLY)) >= 0) {
    capabilities |= IO_CAPABLE;
    dprintf("We can use /dev/io\n");
    close(fd);
    return 1;
  }
  #elif defined(HAVE_LINUX)
  if (ioperm (0x378 /* say */, 3, 1) == 0) {
    ioperm (0x378, 3, 0);
    capabilities |= IO_CAPABLE;
    dprintf ("We can use ioperm()\n");
    return 1;
  }
  #elif defined(HAVE_SOLARIS)
  int fd;
  if (fd=open("/devices/pseudo/iop@0:iop", O_RDWR) > 0) {
    capabilities |= IO_CAPABLE;
    dprintf ("We can use iop\n");
    return 1;
  }
  dprintf ("We can't use IOP, nothing will work\n");
  #elif defined(HAVE_CYGWIN_9X)
  /* note: 95 allows apps direct IO access */
  dprintf ("Taking a guess on port availability (win9x)\n");
  capabilities |= IO_CAPABLE;
  return 1;
  #endif

  return 0;
}

/* Can we use win32 style I/O (cygwin environment) */

static int
check_lpt (void)
{
  #ifdef HAVE_CYGWIN_NT

  HANDLE hf = CreateFile("\\\\.\\$VDMLPT1", GENERIC_READ | GENERIC_WRITE,
      0, NULL, OPEN_EXISTING, 0, NULL);
  if (hf == INVALID_HANDLE_VALUE) return 0;
  CloseHandle(hf);

  capabilities |= LPT_CAPABLE;
  return 1;
  #else
  return 0;
  #endif
}

/* Figure out what we can use to talk to the parallel port.
 * We aren't allowed to use the things set in forbidden though. */
int
detect_environment (int forbidden)
{
#define FORBIDDEN(bit) (forbidden & bit)
  static int detected = 0;
  if (detected && !forbidden) return 0;
  detected = 1;

  capabilities = 0;

  /* Find out what access mechanisms there are. */
  if ((!FORBIDDEN(PPDEV_CAPABLE)) && check_dev_node ("parport"))
    capabilities |= PPDEV_CAPABLE;
  if (!FORBIDDEN (IO_CAPABLE))
    check_io ();
  if (!FORBIDDEN (DEV_PORT_CAPABLE))
    check_dev_port ();
  if (!FORBIDDEN (LPT_CAPABLE))
    check_lpt ();

  /* Find out what kind of /proc structure we have. */
  check_dev_node ("lp"); /* causes low-level port driver to be loaded */
  check_proc_type ();

  return 0;
}

/*
 * Local Variables:
 * eval: (c-set-style "gnu")
 * End:
 */
