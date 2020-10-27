# HALCON/Qt pattern matching and measure example
#
# (c) 2004-2020 MVTec Software GmbH
#
# README
#

- This example requires Qt version 4.8 or later (for MacOS, only version 4.8
  has been tested to work).

- To generate the application take the following steps:

  1. Check for the correct setting of the environment variables
     'PATH','HALCONROOT', and 'HALCONARCH'.
     How to specify those environment variables is described in the
     Programmer's Guide (Section: Platform-Specific HALCON Versions)

  2a. Generate Makefile:

           qmake -o Makefile matching.pro

      or simply:

           qmake

   b. Alternatively generate Visual Studio project file:

           qmake -tp vc

           or: qmake -t vcapp -o matching.dsp matching.pro

  3a. Generate executable by calling make, nmake, gmake etc.

   b. Alternatively use the generated Visual Studio project file

- Alternatively, Qt Creator can be used to build the application.
  To do that, load the matching.pro file to your Qt Creator then build it
  using the desired Qt version.

- For MacOS X, take the following steps:

  1a. Generate either a Makefile:

          qmake -spec macx-g++ matching.pro

   b. Alternatively generate a Xcode project:

          qmake -spec macx-xcode matching.pro

      Note that up until Qt 5.2.0, qmake has a bug where file paths that
      contain spaces are not quoted properly, so you may need to manually
      fix the 'Qt Qmake' and 'Qt Preprocessors' custom build phases in the
      generated Xcode project by putting the path to the example directory in
      single quotes in the shell command.

  2. Generate executable by calling make or selecting 'Build' in Xcode.

