= Anthony's Open Source Contributions =
:stylesheet: dark.css

NOTE: Only authorship is claimed.  Unless explicitly stated in the source files, there is no claim to copyright nor ownership and none should be inferred. 

== Professional ==

=== BlackBerry Accessibility Guidance and Best Practices Sample Application ===
I was tasked with helping internal application developers work our their plan and approach for how they would implement their Accessibility requirements.  Over and over again, I got questions about how to approach the same problems.  So, I decided to write a sample application to show developers the tricks and techniques to make their applications accessible.   The application shows typical cases of coding pitfalls that happened because of a lack of forethought regarding accessibility.  Each case is then followed by a fix and an explanation of what the fix achieves.  I was then asked to do a presentation based on the application for external developers at BBJam Asia 2013.  Please have a look at the following sample:

    BeforeA11yAndAfter

=== Image decoder integration into Webkit for BlackBerry Browser ===
I authored the following C++ code and header file.  This integrates a custom file format developed by RIM/SlipStream into Webkit allowing the BlackBerry browser to decode these files.  This is not the most efficient nor correct implementation that I could have done.  For example, I dereference pointers and then increment them.  I could have done that in a single line of code or found other faster ways to do that.  I was depending on the compiler to help me optimize while allowing my code to be more readable. One regret I have about this particular piece of code is that I use malloc() instead of new and don't check for NULL as its return value.  

    RxIImageDecoder.cpp
    RxIImageDecoder.h

=== Samples for the BlackBerry PlayBook ===

I help maintain and add features to the NDK (Native Development Kit) samples. 
I authored some and maintain all of the samples at  http://blackberry.github.com/ndk/samples.html
SDL Audio Driver and Autoconf Integration for PlayBook
There is a port of the SDL library to the BlackBerry PlayBook.  I wrote an audio driver for that port, but the default one is the original QNX Neutrino driver.  

    https://github.com/anhu/SDL/tree/emulate/src/audio/playbook

=== SDL Autoconf Integration for Playbook ===
The SDL library port was originally built with the IDE.  I added in some scripts to build the library and tests for the PlayBook using configure and the autoconf tools.  I also added scripts that deploy the test apps from the build machine to the playbook.  This is primarily useful as an example of how to use autoconf to build other projects for the PlayBook. Please see the GitHub BlackBerry SDL Project page and look for commits by anhu. 

    https://github.com/blackberry/SDL

== Personal ==

=== Minor Bug Fix for Eqonomize ===
Eqonomize! is personal accounting system that focuses on simplicity and ease of use.  There was a problem where the .eqz file extension filter was not appearing in the open file and save file dialogs.  I submitted a minor patch that fixed this problem. 

    The patch: patch.fileFilterEqz 
    The submission on SourceForge.net is here

=== String Substitution With File ===
This is a rudimentary text processing programming.  It probably would have been simpler in just about any other language, but I chose to do it in C for the fun of it.  It searches for a line containing a match of the substitution string in the input file.  Upon finding the match, it replaces the line with the contents of the substitution file and writes out the results to the output file.
