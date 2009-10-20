/*! \mainpage
 *
 *  <b>TSPSG: TSP Solver and Generator</b>
 *  \author Copyright (C) 2007-2009 Lёppa <contacts[at]oleksii[dot]name>
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

#ifndef VERSION_H
#define VERSION_H

/*!
 * \file version.h
 * \brief This file contains TSPSG version information defines.
 */

//! TSPSG version ID
#define VERSIONID "$Id$"
//! Major version of current TSPSG build
#define BUILD_VERSION_MAJOR 0
//! Minor version of current TSPSG build
#define BUILD_VERSION_MINOR 1
//! TSPSG release number
#define BUILD_RELEASE 2

/*!
 * \brief Current TSPSG build number
 *
 * This will only change on releases and will be the same as revision number.
 *
 * Build number meanings:
 *   - 1  --  3: alpha 1 to 3
 *   - 4  --  7: beta 1 to 4
 *   - 8  -- 10: rc 1 to 3
 *   - 11 --...: release 1 to ...
 */
#define BUILD_NUMBER 65535

/*!
 * \def BUILD_STATUS
 * \brief TSPSG build status
 *
 * Determined based on BUILD_NUMBER.
 */
#if BUILD_NUMBER == 65535
	#define BUILD_STATUS (dev build)
#elif BUILD_RELEASE < 4
	#define BUILD_STATUS (alpha)
#elif BUILD_RELEASE < 8
	#define BUILD_STATUS (beta)
#elif BUILD_RELEASE < 11
	#define BUILD_STATUS (rc)
#endif // BUILD_NUMBER == 65535

//! Used for "conversion" of \a x to quoted string
#define QUOTE_X(x) #x
#define QUOTE(x) QUOTE_X(x)

/*!
 * \def BUILD_VERSION
 * \brief Full version of TSPSG in the form: \a major.\a minor.\a release [\a status].
 */
#ifndef BUILD_STATUS
	#define BUILD_VERSION QUOTE(BUILD_VERSION_MAJOR.BUILD_VERSION_MINOR.BUILD_RELEASE)
#else
	#define BUILD_VERSION QUOTE(BUILD_VERSION_MAJOR.BUILD_VERSION_MINOR.BUILD_RELEASE BUILD_STATUS)
#endif

#endif // VERSION_H
