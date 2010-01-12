/*!
 * \file version.h
 * \author Copyright (C) 2007-2010 Lёppa <contacts[at]oleksii[dot]name>
 *
 *  $Id$
 *  $URL$
 *
 * \brief Contains TSPSG version information defines.
 *
 *  <b>TSPSG: TSP Solver and Generator</b>
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
/*!
 * \mainpage
 * \image html tspsg.png
 *  <b>TSPSG: TSP Solver and Generator</b>
 * \author Copyright (C) 2007-2010 Lёppa <contacts[at]oleksii[dot]name>
 *
 *  $Id$
 *  $URL$
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

#ifndef VERSION_H
#define VERSION_H

//! TSPSG version ID
#define VERSIONID "$Id$"
//! Major version of current TSPSG build
#define BUILD_VERSION_MAJOR 0
//! Minor version of current TSPSG build
#define BUILD_VERSION_MINOR 1
/*!
 * \brief TSPSG release number
 *
 *  Release number meanings:
 *    - 1  --  3: <b>alpha</b> 1 to 3
 *    - 4  --  7: <b>beta</b> 1 to 4
 *    - 8  -- 10: <b>rc</b> 1 to 3
 *    - 11 --  x: <b>release</b> 1 to x-10
 */
#define BUILD_RELEASE 2

/*!
 * \brief Current TSPSG build number
 *
 *  This will only change on releases and will be the same as revision number.
 */
#define BUILD_NUMBER 65535

/*!
 * \def BUILD_STATUS
 * \brief TSPSG build status
 *
 * Determined based on BUILD_NUMBER and BUILD_RELEASE.
 */
#if BUILD_NUMBER == 65535
	#define BUILD_STATUS (dev build)
#else
	#if BUILD_RELEASE < 4
		#define BUILD_STATUS (alpha)
	#elif BUILD_RELEASE < 8
		#define BUILD_STATUS (beta)
	#elif BUILD_RELEASE < 11
		#define BUILD_STATUS (rc)
	#else
		#define BUILD_STATUS
	#endif
#endif // BUILD_NUMBER == 65535

//! \internal \brief A helper for QUOTE(x).
#define QUOTE_X(x) #x
//! Used for "conversion" of \a x to quoted string
#define QUOTE(x) QUOTE_X(x)

/*!
 * \def BUILD_VERSION
 * \brief Full version of TSPSG in the form: \c major.\c minor.\c release[ \c status].
 */
#ifndef BUILD_STATUS
	#define BUILD_VERSION QUOTE(BUILD_VERSION_MAJOR.BUILD_VERSION_MINOR.BUILD_RELEASE)
#else
	#define BUILD_VERSION QUOTE(BUILD_VERSION_MAJOR.BUILD_VERSION_MINOR.BUILD_RELEASE BUILD_STATUS)
#endif

#endif // VERSION_H
