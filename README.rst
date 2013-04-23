Khopper
=======

This is a GUI audio converter, which can parse CUE sheet and convert to another
audio format.

Feature
-------
* Parse CUE sheet
* Support many codecs (e.g.: wav, ape, mp3 ... etc.)
* Converts to mp3, flac, ogg, wav
* Support multi-task convertion
* Query `FreeDB`_ for CUE sheet
* Play audio (by Phonon)
* Extensible by plugins

Usage
-----

Please see `here <https://github.com/legnaleurc/khopper/wiki/Usage>`_.

Supported platform
------------------

* GNU/Linux
* Microsoft Windows
* Mac OS X

Supported Formats
-----------------

The `FFmpeg`_/`Libav`_ plugin provides the most audio codecs, such as wav, ape ... etc.

Please at least enable the `FFmpeg`_/`Libav`_ plugin.

License
-------

* libkhopper: LGPLv2.1 or later
* khopper_gui: GPLv3 or later
* plugins: May vary with dependencies, but basically are GPLv3

Build Dependencies
------------------

Toolchain
~~~~~~~~~

C++11 support is required.

* Linux: `GNU Compiler Collection`_ >= 4.5
* Windows: `Microsoft Visual C++`_ >= 2010
* Mac OS X: `XCode`_ >= 4.5

Required
~~~~~~~~

* `CMake`_
* `Qt`_ >= 4.7 [GPL3, LGPL2.1, Commercial]

  * Phonon: for GUI

Optional
~~~~~~~~

* `FFmpeg`_ or `Libav`_

  * `FFmpeg`_ >= 1.1 [LGPL, GPL]
  * `Libav`_ >= 9 [LGPL, GPL]
  * `Libav`_ == 0.8 [LGPL, GPL]

* `libFLAC`_ >= 1.2.1 [BSD-3]
* `libvorbis`_ and `libogg`_ [BSD-3]
* `LAME`_ [LGPL, patent]
* `TagLib`_ [LGPL, MPL]

How To Build
------------

You should choose `FFmpeg`_ or `Libav`_ by option
``KHOPPER_USE_FFMPEG_OR_LIBAV``. There are three choices: ``ffmpeg``,
``libav``, ``libav_0_8``. Default value is ``ffmpeg``.

GNU/Linux or Mac OS X
~~~~~~~~~~~~~~~~~~~~~

.. code:: bash

  mkdir build
  cd build
  cmake -DCMAKE_BUILD_TYPE=Release ..
  make

For `Libav`_ == 0.8 users (e.g.: Debian, Ubuntu):

.. code:: bash

  mkdir build
  cd build
  cmake -DCMAKE_BUILD_TYPE=Release -DKHOPPER_USE_FFMPEG_OR_LIBAV=libav_0_8 ..
  make

Microsoft Windows
~~~~~~~~~~~~~~~~~

Please use `CMake`_ GUI to generate the building system.


.. _CMake: http://www.cmake.org/
.. _FFmpeg: http://ffmpeg.org/
.. _FreeDB: http://www.freedb.org/
.. _GNU Compiler Collection: http://gcc.gnu.org/
.. _LAME: http://lame.sourceforge.net/
.. _Libav: http://libav.org/
.. _libFLAC: http://flac.sourceforge.net/
.. _libvorbis: http://www.xiph.org/downloads/
.. _libogg: http://www.xiph.org/downloads/
.. _Microsoft Visual C++: http://www.microsoft.com/visualstudio/eng/products/visual-studio-2010-express
.. _Qt: http://qt-project.org/
.. _TagLib: http://taglib.github.com/
.. _XCode: https://developer.apple.com/xcode/
