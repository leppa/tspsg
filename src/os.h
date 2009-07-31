/*
 *  TSPSG: TSP Solver and Generator
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

// Some target arch detection.
// NOTE: Only for GNU C, Visual Studio, Intel C/C++ and MinGW32 compilers
#if defined(__amd64__) || defined(_M_X64)
	#define ARCH " (AMD 64-bit)"
#elif defined(__ia64__) || defined(_M_IA64)
	#define ARCH " (Intel 64-bit)"
#elif defined(__i386__) || defined(_M_IX86) || defined(_X86_)
	#define ARCH " (Intel x86)"
#elif defined(__powerpc__) || defined(_M_PPC)
	#define ARCH " (PowerPC)"
#elif defined(__arm__) || defined(_M_ARM)
	#define ARCH " (ARM)"
#elif defined(__mips__) || defined(_M_MRX000)
	#define ARCH " (MIPS)"
#else
	#define ARCH ""
#endif // ARCH

#ifdef Q_OS_AIX
	#define OS "AIX"ARCH
	#define OSID quint8(1)
#elif defined Q_OS_BSD4
	#define OS "BSD 4.4"ARCH
	#define OSID quint8(2)
#elif defined Q_OS_BSDI
	#define OS "BSD/OS"ARCH
	#define OSID quint8(3)
#elif defined Q_OS_CYGWIN
	#define OS "Cygwin"ARCH
	#define OSID quint8(4)
#elif defined Q_OS_DARWIN
	#define OS "Darwin OS"ARCH
	#define OSID quint8(5)
#elif defined Q_OS_DGUX
	#define OS "DG/UX"ARCH
	#define OSID quint8(6)
#elif defined Q_OS_DYNIX
	#define OS "DYNIX/ptx"ARCH
	#define OSID quint8(7)
#elif defined Q_OS_FREEBSD
	#define OS "FreeBSD"ARCH
	#define OSID quint8(8)
#elif defined Q_OS_HPUX
	#define OS "HP-UX"ARCH
	#define OSID quint8(9)
#elif defined Q_OS_HURD
	#define OS "GNU Hurd"ARCH
	#define OSID quint8(10)
#elif defined Q_OS_IRIX
	#define OS "SGI Irix"ARCH
	#define OSID quint8(11)
#elif defined Q_OS_LINUX
	#define OS "Linux"ARCH
	#define OSID quint8(12)
#elif defined Q_OS_LYNX
	#define OS "LynxOS"ARCH
	#define OSID quint8(13)
#elif defined Q_OS_MSDOS
	#define OS "MS-DOS"ARCH
	#define OSID quint8(14)
#elif defined Q_OS_NETBSD
	#define OS "NetBSD"ARCH
	#define OSID quint8(15)
#elif defined Q_OS_OS2
	#define OS "OS/2"ARCH
	#define OSID quint8(16)
#elif defined Q_OS_OPENBSD
	#define OS "OpenBSD"ARCH
	#define OSID quint8(17)
#elif defined Q_OS_OS2EMX
	#define OS "OS/2"ARCH
	#define OSID quint8(18)
#elif defined Q_OS_OSF
	#define OS "HP Tru64 UNIX"ARCH
	#define OSID quint8(19)
#elif defined Q_OS_QNX6
	#define OS "QNX RTP 6.1"ARCH
	#define OSID quint8(20)
#elif defined Q_OS_QNX
	#define OS "QNX"ARCH
	#define OSID quint8(21)
#elif defined Q_OS_RELIANT
	#define OS "Reliant UNIX"ARCH
	#define OSID quint8(22)
#elif defined Q_OS_SCO
	#define OS "SCO OpenServer 5"ARCH
	#define OSID quint8(23)
#elif defined Q_OS_SOLARIS
	#define OS "Sun Solaris"ARCH
	#define OSID quint8(24)
#elif defined Q_OS_ULTRIX
	#define OS "DEC Ultrix"ARCH
	#define OSID quint8(25)
#elif defined Q_OS_UNIX
	#define OS "UNIX BSD/SYSV"ARCH
	#define OSID quint8(26)
#elif defined Q_OS_UNIXWARE
	#define OS "UnixWare 7/Open UNIX 8"ARCH
	#define OSID quint8(27)
#elif defined Q_OS_WIN32
	#define OS "Windows"ARCH
	#define OSID quint8(28)
#elif defined Q_OS_WINCE
	#define OS "Windows CE"ARCH
	#define OSID quint8(29)
#else
	#define OS "Unknown"ARCH
	#define OSID quint8(255)
#endif // OS

#endif // OS_H
