
#ASTRAN

ASTRAN is an acronym for __Automatic Synthesis of Transistor Networks__. It is a free, open source, physical synthesis framework for Mac and Linux.

It supports automatic layout generation of CMOS cells from a transistor level netlist description in SPICE format. Area overhead compared to high quality hand-made standard-cells layouts is 3.7% on average.
Its functionality is being extended to produce complete circuit layouts. Features include transistor sizing, floorplanning, cell placement, and routing. The framework is very modular and new contributions to this project are welcome.

![Astran](https://gme-ufrgs.googlecode.com/files/screenshot2.png)

###Features:
- ASTRAN 3 is under active development and supports 45+nm technology rules (including freePDK45) like: redundant diff contacts, conditional design rules (metal/poly/diff enclosure of contacts, gate extension on diff, diff to gate spacing, metal/poly spacing on dense line-ends), minimum area,...
- It generates all kinds of logic cells, including non-complementary as PTL;
- Cell instances can be individually sized before generation to reduce delay;
- Cell layout generation flow includes: transistor folding, 1-D transistor placement (using Threshold Accepting metaheuristic), intracell routing (using pathfinder and iterated 1-steiner algorithms) and 2-D layout compaction (using ILP);
- Layouts can be saved in CIF, GDSII and LEF formats. It can also be exported to Cadence ICFB and loaded into Virtuoso;
- Structural Verilog is supported once you provide a Spice description of each cell;
- Includes a command line interpreter for increased flexibility, like the ability to run scripts;

TIP: Download and install always the latest version directly from the GIT repository. Binary versions will not be updated anymore since google closed this service. It's pretty easy to compile.

There is a <a href="https://www.youtube.com/watch?v=IFbCDSMi_FE">video</a> tutorial that can be usefull.
Any question about the tool, please, send me an email: amziesemerj (at) inf.ufrgs.br

If you are using ASTRAN, please cite: <a href="https://www.researchgate.net/publication/262876168_Simultaneous_Two-Dimensional_Cell_Layout_Compaction_Using_MILP_with_ASTRAN?ev=prf_pub">ISVLSI'14</a> or visit my <a href="https://www.researchgate.net/profile/Adriel_Ziesemer_Jr">profile</a>  for other works.

##Building

- On Linux 

The following procedure was tested in Centos 5.11 and Ubuntu 14.04
```
git clone https://github.com/aziesemer/astran.git astran
cd astran/Astran
make
```

- On Mac OSX

Read the INSTALL.txt file.

## Environment (optional)

```ASTRAN_PATH``` needs to point to the astran path.

##Using

- On Command Line

```$Astran --shell```

- On GUI 

![Astran GUI](https://github.com/aziesemer/astran/blob/master/Astran/misc/astran.jpg?raw=true)
