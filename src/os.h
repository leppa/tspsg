/*
 *  TSPSG - TSP Solver and Generator
 *  Copyright (C) 2007-2009 Lёppa <contacts[at]oleksii[dot]name>
 *
 *  $Id$
 *  $URL$
 *
 *  This file is part of TSPSG.
 *
 *  TSPSG is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TSPSG is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TSPSG.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OS_H
#define OS_H

#ifdef Q_OS_AIX
	#define OS "AIX"
	#define OSID quint8(1)
#elif defined Q_OS_BSD4
	#define OS "BSD 4.4"
	#define OSID quint8(2)
#elif defined Q_OS_BSDI
	#define OS "BSD/OS"
	#define OSID quint8(3)
#elif defined Q_OS_CYGWIN
	#define OS "Cygwin"
	#define OSID quint8(4)
#elif defined Q_OS_DARWIN
	#define OS "Darwin OS"
	#define OSID quint8(5)
#elif defined Q_OS_DGUX
	#define OS "DG/UX"
	#define OSID quint8(6)
#elif defined Q_OS_DYNIX
	#define OS "DYNIX/ptx"
	#define OSID quint8(7)
#elif defined Q_OS_FREEBSD
	#define OS "FreeBSD"
	#define OSID quint8(8)
#elif defined Q_OS_HPUX
	#define OS "HP-UX"
	#define OSID quint8(9)
#elif defined Q_OS_HURD
	#define OS "GNU Hurd"
	#define OSID quint8(10)
#elif defined Q_OS_IRIX
	#define OS "SGI Irix"
	#define OSID quint8(11)
#elif defined Q_OS_LINUX
	#define OS "Linux"
	#define OSID quint8(12)
#elif defined Q_OS_LYNX
	#define OS "LynxOS"
	#define OSID quint8(13)
#elif defined Q_OS_MSDOS
	#define OS "MS-DOS"
	#define OSID quint8(14)
#elif defined Q_OS_NETBSD
	#define OS "NetBSD"
	#define OSID quint8(15)
#elif defined Q_OS_OS2
	#define OS "OS/2"
	#define OSID quint8(16)
#elif defined Q_OS_OPENBSD
	#define OS "OpenBSD"
	#define OSID quint8(17)
#elif defined Q_OS_OS2EMX
	#define OS "OS/2"
	#define OSID quint8(18)
#elif defined Q_OS_OSF
	#define OS "HP Tru64 UNIX"
	#define OSID quint8(19)
#elif defined Q_OS_QNX6
	#define OS "QNX RTP 6.1"
	#define OSID quint8(20)
#elif defined Q_OS_QNX
	#define OS "QNX"
	#define OSID quint8(21)
#elif defined Q_OS_RELIANT
	#define OS "Reliant UNIX"
	#define OSID quint8(22)
#elif defined Q_OS_SCO
	#define OS "SCO OpenServer 5"
	#define OSID quint8(23)
#elif defined Q_OS_SOLARIS
	#define OS "Sun Solaris"
	#define OSID quint8(24)
#elif defined Q_OS_ULTRIX
	#define OS "DEC Ultrix"
	#define OSID quint8(25)
#elif defined Q_OS_UNIX
	#define OS "UNIX BSD/SYSV"
	#define OSID quint8(26)
#elif defined Q_OS_UNIXWARE
	#define OS "UnixWare 7/Open UNIX 8"
	#define OSID quint8(27)
#elif defined Q_OS_WIN32
	#define OS "Windows"
	#define OSID quint8(28)
#elif defined Q_OS_WINCE
	#define OS "Windows CE"
	#define OSID quint8(29)
#else
	#define OS "Unknown"
	#define OSID quint8(255)
#endif

#endif // OS_H
