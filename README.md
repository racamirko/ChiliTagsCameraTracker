ChiliTagsCameraTracker
======================

Camera orientation/position tracker based on ChiliTags QR-codes.

Overview
--------

Goal of the project is to extract to rotation and translation matrices of the camera. It relies on the known positions of points in 3D space, specifically the corners of the [chiliTag markers](https://github.com/racamirko/chilitags).

Setup description
-----------------

A video stream/recording from a camera perspective of a scene containing chiliTags. A file provided with the tag id's and locations should be provided as a separate resource. The format will be described as the project is being developed.

Dependencies
------------

 * **ChiliTags** - the open/free replacement for QR tags link to my cloned repository is [here](https://github.com/racamirko/chilitags)
 * **OpenCV** - for the actual computation algorithms for R/T matrices from point correspondance, link to the library [here](http://opencv.org/)
 * **glog** - library for log files [link](https://code.google.com/p/google-glog/)
 * **boost** - [link](http://www.boost.org/)


Licence
-------

Tracker is being released under GPL3.

The coding is being done as a part of my research for the PhD thesis done at [CHILI lab](http://chili.epfl.ch/) at [EPFL](http://www.epfl.ch).

Current contact e-mail is ( mirko (dot) raca (at) epfl (dot) ch ).
