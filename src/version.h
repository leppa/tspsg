/*!
 * \file version.h
 * \author Copyright &copy; 2007-2010 Lёppa <contacts[at]oleksii[dot]name>
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
 * \author Copyright &copy; 2007-2010 Lёppa <contacts[at]oleksii[dot]name>
 *
 *  \b Homepage: <a href="http://tspsg.sourceforge.net/">tspsg.sourceforge.net</a>
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

#ifndef BUILD_VERSION_MAJOR
	//! Major version of current TSPSG build
	#define BUILD_VERSION_MAJOR 0
#endif // BUILD_VERSION_MAJOR
#ifndef BUILD_VERSION_MINOR
	//! Minor version of current TSPSG build
	#define BUILD_VERSION_MINOR 0
#endif // BUILD_VERSION_MINOR
/*!
 * \brief TSPSG release number
 *
 *  Release number meanings:
 *    - 1  --  3: <b>alpha</b> 1 to 3
 *    - 4  --  7: <b>beta</b> 1 to 4
 *    - 8  -- 10: <b>rc</b> 1 to 3
 *    - 11 --  x: <b>release</b> 1 to x-10
 */
#ifndef BUILD_RELEASE
	#define BUILD_RELEASE 0
#endif // BUILD_RELEASE

/*!
 * \brief Current TSPSG build number
 *
 *  This will only change on releases and will be the same as revision number.
 */
#ifndef BUILD_NUMBER
	#define BUILD_NUMBER 0
#endif // BUILD_NUMBER

/*!
 * \def BUILD_STATUS
 * \brief TSPSG build status
 *
 * Determined based on BUILD_NUMBER and BUILD_RELEASE.
 */
#ifndef TSPSG_RELEASE_BUILD
	#define BUILD_STATUS (dev build)
#elif BUILD_RELEASE < 4
	#define BUILD_STATUS (alpha)
#elif BUILD_RELEASE < 8
	#define BUILD_STATUS (beta)
#elif BUILD_RELEASE < 11
	#define BUILD_STATUS (rc)
#endif // TSPSG_RELEASE_BUILD

//! \internal \brief A helper for QUOTE(x).
#define QUOTE_X(x) #x
//! Used for "conversion" of \a x to quoted string
#define QUOTE(x) QUOTE_X(x)

/*!
 * \def BUILD_VERSION
 * \brief Full version of TSPSG in the form: \c major.minor.release.build [\c status].
 */
#ifndef BUILD_STATUS
	#define BUILD_VERSION QUOTE(BUILD_VERSION_MAJOR.BUILD_VERSION_MINOR.BUILD_RELEASE.BUILD_NUMBER)
#else
	#define BUILD_VERSION QUOTE(BUILD_VERSION_MAJOR.BUILD_VERSION_MINOR.BUILD_RELEASE.BUILD_NUMBER BUILD_STATUS)
#endif

#endif // VERSION_H
