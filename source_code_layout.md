The core of ProviewR consists of three modules:
1. The WorkBench (WB) used to create PLC programs and graphical interfaces for operators
2. The command-line RunTime (RT) used to start and stop PLC programs
3. The Runtime Viewer (XTT) used to monitor and operate running PLC programs

The source code for ProviewR is organised in layers, where each layers is built on top of, and adds additional functionality to, the layer below. These layers are:
0. src/exp/inc: Basic type definitions and macros.
1. src/lib/co: Library of common (CO) functionality used by all other parts of ProviewR.
2. src/lib/rt: Library of functionality used by the ProviewR RunTime (RT).
3. xtt/lib/cow: Library of common widgets (COW) used by all graphical parts of ProviewR.
4. xtt/lib/flow and xtt/lib/glow: These are small GUI widget libraries. Flow is used by ProviewR itself to draw the WorkBench and Runtime Viewer, while Glow is used to draw the user-created graphical interfaces for operators.
5. xtt/lib/ge: GE is used to create graphical interfaces for operators.
6. xtt/lib/xtt and wb/lib/wb: These libraries contain the graphical widgets used by the xtt/exe and wb/exe executables respectively.
7. src/exe, xtt/exe and wb/exe: These folders contain the code for the actual executables. src/exe contain the command-line RunTime executables, wb/exe contain the executables for the WorkBench (WB), and xtt/exe contain the executables for the Runtime Viewer.

In addition to these, ProviewR comes with a number of plugins that adds support for various hardware, for example: the profibus folder adds support for profibus, etc.
