TSP Solver and Generator
========================

Copyright (C) 2007-2013 [Oleksii Serdiuk](mailto:contacts@oleksii.name).


About TSPSG
-----------

**TSP Solver and Generator** is intended to generate and solve
**Travelling Salesman Problem** (TSP) tasks. It uses *Branch and Bound*
method for solving. Its input is a number of cities and a matrix of
city-to-city travel costs. The matrix can be populated with random
values in a given range (which is useful for generating tasks). The
result is an optimal route, its price, step-by-step matrices of solving
and a solving graph. The task can be saved in an internal binary format
and opened later. The result can be printed or saved as *PDF*, *HTML*,
or *ODF*.

**TSPSG** may be useful for teachers to generate test tasks or just for
regular users to solve TSPs. Also, it may be used as an example of using
*Branch and Bound* method to solve a particular task.

You can check the [Installation Guide](INSTALL.md) or read the
[ChangeLog](ChangeLog.md) to see what's new (updated only on releases).


License
-------

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.


Credits
-------

I'd like to give credits to the following projects which were used in
the creation of **TSP Solver and Generator**:

  * TSPSG was created using **Qt framework** licensed under the terms of
    the *GNU Lesser General Public License*,
    see <http://qt.nokia.com/>.

  * Most icons used in TSPSG are part of **Oxygen Icons** project
    licensed according to the *GNU Lesser General Public License*,
    see <http://www.oxygen-icons.org/>.

  * Country flag icons used in TSPSG are part of the free **Flag Icons**
    collection created by **IconDrawer**,
    see <http://www.icondrawer.com/>.

  * TSPSG comes with the default "embedded" font **DejaVu LGC Sans
    Mono** from the **DejaVu fonts** licensed under a *Free license*,
    see <http://dejavu-fonts.org/>.


<!--
$Id: $Format:%h %ai %an$ $
$URL: http://tspsg.info/ $
-->
