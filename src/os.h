/*!
 * \file os.h
 * \author Copyright &copy; 2007-2014 Oleksii Serdiuk <contacts[at]oleksii[dot]name>
 *
 *  $Id: $Format:%h %ai %an$ $
 *  $URL: http://tspsg.info/ $
 *
 * \warning Arch detection works only for <b>GNU C</b>, <b>Visual Studio</b>, <b>Intel C/C++</b> and <b>MinGW32</b> compilers.
 *  OS detection should work for any compiler.
 *
 * \brief Contains TSPSG target CPU architecture and OS detection.
 *
 *  <b>TSPSG: TSP Solver and Generator</b>
 *
 *  This file is part of TSPSG.
 *
 *  TSPSG is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
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

#include <QtGlobal>

// Some target arch detection.
/*!
 * \def ARCH
 * \brief The target CPU architecture TSPSG was built for.
 * \warning NOTE: Only for <b>GNU C</b>, <b>Visual Studio</b>, <b>Intel C/C++</b> and <b>MinGW32</b> compilers.
 */
#if defined(__amd64__) || defined(_M_X64)
#   define ARCH " (AMD 64-bit)"
#elif defined(__ia64__) || defined(_M_IA64)
#   define ARCH " (Intel 64-bit)"
#elif defined(__i386__) || defined(_M_IX86) || defined(_X86_)
#   define ARCH " (Intel x86)"
#elif defined(__powerpc__) || defined(_M_PPC)
#   define ARCH " (PowerPC)"
#elif defined(__arm__) || defined(_M_ARM)
#   define ARCH " (ARM)"
#elif defined(__mips__) || defined(_M_MRX000)
#   define ARCH " (MIPS)"
#else
#   define ARCH ""
#endif // ARCH

// Target OS detection. Done by Qt, so should work for any compiler.
/*!
 * \def OS
 * \brief The target operating system TSPSG was built for.
 */
/*!
 * \def OSID
 * \brief The target operating system ID.
 *
 * This value will be used in task file metadata.
 */
#ifdef Q_OS_AIX
#   define OS "AIX"
#   define OSID quint8(1)
#elif defined(Q_OS_BSDI)
#   define OS "BSD/OS"
#   define OSID quint8(2)
#elif defined(Q_OS_CYGWIN)
#   define OS "Cygwin"
#   define OSID quint8(3)
#elif defined(Q_OS_DARWIN)
#   define OS "Mac OS (Darwin)"
#   define OSID quint8(4)
#elif defined(Q_OS_DGUX)
#   define OS "DG/UX"
#   define OSID quint8(5)
#elif defined(Q_OS_DYNIX)
#   define OS "DYNIX/ptx"
#   define OSID quint8(6)
#elif defined(Q_OS_FREEBSD)
#   define OS "FreeBSD"
#   define OSID quint8(7)
#elif defined(Q_OS_HPUX)
#   define OS "HP-UX"
#   define OSID quint8(8)
#elif defined(Q_OS_HURD)
#   define OS "GNU Hurd"
#   define OSID quint8(9)
#elif defined(Q_OS_IRIX)
#   define OS "SGI Irix"
#   define OSID quint8(10)
#elif defined(Q_OS_LINUX)
#   define OS "Linux"
#   define OSID quint8(11)
#elif defined(Q_OS_LYNX)
#   define OS "LynxOS"
#   define OSID quint8(12)
#elif defined(Q_OS_NETBSD)
#   define OS "NetBSD"
#   define OSID quint8(13)
#elif defined(Q_OS_OPENBSD)
#   define OS "OpenBSD"
#   define OSID quint8(14)
#elif defined(Q_OS_OS2EMX)
#   define OS "OS/2"
#   define OSID quint8(15)
#elif defined(Q_OS_OSF)
#   define OS "HP Tru64 UNIX"
#   define OSID quint8(16)
#elif defined(Q_OS_BLACKBERRY)
#   define OS "BlackBerry 10"
#   define OSID quint8(17)
#elif defined(Q_OS_QNX)
#   define OS "QNX Neutrino"
#   define OSID quint8(18)
#elif defined(Q_OS_RELIANT)
#   define OS "Reliant UNIX"
#   define OSID quint8(19)
#elif defined(Q_OS_SCO)
#   define OS "SCO OpenServer 5"
#   define OSID quint8(20)
#elif defined(Q_OS_SOLARIS)
#   define OS "Sun Solaris"
#   define OSID quint8(21)
#elif defined(Q_OS_SYMBIAN)
#   define OS "Symbian"
#   define OSID quint8(22)
#elif defined(Q_OS_ULTRIX)
#   define OS "DEC Ultrix"
#   define OSID quint8(23)
#elif defined(Q_OS_UNIXWARE)
#   define OS "UnixWare 7/Open UNIX 8"
#   define OSID quint8(24)
#elif defined(Q_OS_WIN32)
#   define OS "Windows"
#   define OSID quint8(25)
#elif defined(Q_OS_WINCE_WM)
#   define OS "Windows Mobile"
#   define OSID quint8(26)
#elif defined(Q_OS_WINCE)
#   define OS "Windows CE"
#   define OSID quint8(27)
#elif defined(Q_OS_BSD4)
#   define OS "BSD 4.4"
#   define OSID quint8(253)
#elif defined(Q_OS_UNIX)
#   define OS "UNIX BSD/SYSV"
#   define OSID quint8(254)
#else
#   define OS "Unknown"
#   define OSID quint8(255)
#endif // OS

//! The target platform in the form OS ARCH
#define PLATFROM OS ARCH

/*!
 * \def COMPILER
 * \brief The compiler TSPSG was built with.
 */
#ifdef Q_CC_INTEL
#   define COMPILER "Intel C++"
#elif defined(Q_CC_MSVC)
#   define COMPILER "Microsoft Visual C/C++"
#elif defined(Q_CC_MINGW)
#   define COMPILER "MinGW GCC"
#elif defined(Q_CC_GNU)
#   define COMPILER "GNU C++"
#elif defined(Q_CC_GCCE)
#   define COMPILER "GCCE (Symbian GCCE builds)"
#elif defined(Q_CC_RVCT)
#   define COMPILER "ARM Realview Compiler Suite"
#elif defined(Q_CC_NOKIAX86)
#   define COMPILER "Nokia x86 (Symbian WINSCW builds)"
#else
#   define COMPILER "Unknown"
#endif

#endif // OS_H
