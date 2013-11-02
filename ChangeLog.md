TSP Solver and Generator ChangeLog
==================================


TSPSG v0.1.4 beta 2 (build 170)
-------------------------------

### WHAT'S NEW ###

  - Support for embedded base64 encoded images when saving solution as
    HTML (Ticket #4).
  - The ability to setup page-related options for printing (Ticket #5).
  - Solution graph size can now be controlled using Output/GraphWidth
    setting in tspsg.ini. Value is in centimeters. (Ticket #8)


### CHANGES & IMPROVEMENTS ###

  - Any value that is entered into task table and doesn't contain
    numbers is considered an infinity.
  - Some error messages are more verbose.
  - Removed a prompt about turning off language autodetection when
    selecting a language.
  - Handheld version includes larger icons (48x48 instead of 32x32).
  - If after running Save As dialog file name doesn't have an extension
    .tspt will be automatically appended.
  - Translucency effect is available on all desktop platforms. No check
    for its support by window system is made at this moment. Blur is
    only available in Windows Vista or higher.


### BUGFIXES ###

  - Bug #2: Solution graph is too small on high resolution screens.
  - Bug #3: Solution graph font size should be fixed.
  - Bug #6: Bug: Actions in Options Menu.
  - Bug #7: No Cancel option in Unsaved Changes dialog under Symbian.
  - Bug #9: Solution graph is still exported to HTML when graph
    generation is disabled.
  - Language name was not loaded in its menu item's status tip which
    caused status tip to be "Set application language to ".
  - Icons in toolbar configuration dialog didn't follow style and one
    of them wasn't shown.


TSPSG v0.1.3 beta 1 (build 145)
-------------------------------

### WHAT'S NEW ###

  + Solution graph generation.
  + Support for switching between available Qt styles.
  + Toolbar customization support (only on desktop platforms).
  + An option to remember last used directory when saving and opening.
  + Automatically check for updates at the given interval option (only
    on Windows platform at this moment).


### IMPROVEMENTS ###

  + Improved the solution output generation algorithm. It is now about
    1.65 times faster.
  + Dragging and dropping task file on the main window now opens it.
  + The deafult output font is now DejaVu LGC Sans Mono and it comes
    "embedded" in the executable.
  + The current desktop icon theme in Linux is now used if it follows
    freedesktop.org icon theme and icon naming spesifications.
  + Translations are now "embedded" in the executable but may be
    overrided by placing files with the same names in l10n folder.
  + Support for Windows 7 Taskbar Extensions (namely, Progress Bars).


### CHANGES ###

  * About dialog was "tabified". Added GPL License and Credits tabs.
  * Platform dependent settings (e.g. paths) are saved per-platform.
  * 32x32 icons are used on handheld platforms. This slightly decreases
    the size of handheld versions. Desktop icons are still 128x128.
  * Windows binary builds were moved from MSVC to Mingw-w64: no more
    Visual C++ Runtime dependency.


### BUGFIXES ###

  - Solution related actions not disabled when solution is canceled.
  - "Autosize" setting was never saved and was always on.



TSPSG v0.1.2 alpha 2 (build 100)
--------------------------------

### WHAT'S NEW ###

  + New TSPSG icon.
  + Symmetric mode: in this mode the cost of travel from city 1 to
    city 2 and vice versa is the same.
  + Printing of solution results (if the platform supports it).
  + Saving solution to PDF (if the platform supports printing).
  + An ability to reset all settings to their defaults: hold Shift
    while clicking Save button in the Settings Dialog.
  + An ability to generate fractional random numbers.
  + An ability to set the fractional accuracy (in decimail places).
  + An ability to select between using native and Qt's file dialogs
    when opening and saving tasks and solutions.
  + An ability to disable showing of the solution steps' matrices in
    solution output.
  + An ability to enable scroll to the end of the solution output after
    solving.
  + An ability ot enable automatic resize of columns and rows to their
    contents on its change.
  + Basic support for Symbian.
  + Check for updates functionality (only in Windows version at this
    moment).


### IMPROVEMENTS ###

  + Improved the solution algorithm.
  + The maximum number of cities in task was increased to 50.
    NOTE: Showing the solution steps' matrices is disabled by default
    for tasks with more than 15 cities to considerably speed up the
    generation of the solution output process.
  + List of alternate candidates for branching is now displayed.
  + Warning about possible non-optimal result.
  + Open a file, passed as command-line argument.
  + Currently opened file name is displayed in the Main Window title.
  + "Save Solution" and "Back to Task" buttons in Solution tab for
    better usability.
  + The size of the toolbar buttons is now determined based on the
    device DPI in Windows Mobile and Symbian.
  + SIP show/hide handling in Windows Mobile.
  + Toolbar state and position are now saved and restored with the Main
    Window state and position.
  + Window translucency support in Windows Vista and Windows 7.


### CHANGES ###

  * The resulting path is now sorted, always starts from City 1 and has
    "City 1 -> City n -> ... -> City 1" format.
  * Translations were moved from i18n to l10n folder to follow common
    standards.
  * Reworked the Settings Dialog layout.
  * Reworked the About dialog.


### BUGFIXES ###

  - A solution couldn't be found for some tasks while the task had at
    least one solution (mostly, tasks with a lot of restrictions).
  - An error when loading .zkt file with less than 5 cities.
  - Generated random numbers were not in the range defined by Minimum
    and Maximum settings.



TSPSG v0.1.1 alpha 1 (build 42)
-------------------------------

This is initial TSPSG release.

### FEATURES ###

  * Solving taks with up to 5 cities.
  * Generating tasks.
  * Saving solution results to HTML and ODF.
  * Importing tasks from ZKomModRd.
  * Multilinguality: English, Russian and Ukrainian languages.
  * Linux/UNIX, Windows and Windows Mobile support.


$Id: $Format:%h %ai %an$ $
$URL: http://tspsg.info/ $
