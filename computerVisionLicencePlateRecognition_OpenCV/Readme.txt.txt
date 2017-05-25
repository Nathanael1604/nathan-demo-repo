Author: Nathanael Park
Initially started as minor project when first gaining exposure to the OpenCV libraryy.
Have added bits and pieces over the years as a side hobby project.

================ Version Function ================

Current version performs: threshold & contour extraction, plate detection, plate localization,
 character segmentation, and Optical Character Recognition (OCR)

================ Version Results ================

Pretty successful at threshold & contour extraction, plate detection, plate localization.

Some ongoing issues with character segmentation and OCR.

Struggles with O/0, 5/S, W/V, 7/Y, 6/b, 1/I etc...

For example, successfully identifies car1.jpg (FSU681) with almost 90% confidence
 and successfully identifies car4.jpg (ERH960) with ~80% confidence,

but struggles with car5.jpg (WW5543), identifying it as IVSS43, or car7.jpg (RY2540), identifying it as R72S40...LOL

================== Development Set-up ====================
OpenCV library, OpenFrameworks, Visual Studios 2012 IDE, Windows 7
.
Will also compile and run on Linux from command line, had it set up on Ubuntu 14.04.1 at one point.
