What is this?
=============

I started this project as attempt to collect in one single place all my code
which is being highly reusable was copy-n-pasted many times from project to project.
The second goal is to make it easy to reuse in any of my future
projects as an external dependency w/o necessity to copy-n-paste and/or fixing
code style and other bullshit...


Requirements
============

* gcc >= 4.8 (http://gcc.gnu.org)
* boost >= 1.54.0 (http://boost.org)
* cmake >= 2.8.12 (http://cmake.org)

To build documentation:
* doxygen >= 1.8 (http://doxygen.org)

To generate skeleton source files using `make new-class` syntax:
* autogen >= 5.4.6 (http://www.gnu.org/software/autogen)


How to install
==============

... simple! :)

    $ cmake . && make && sudo make install
