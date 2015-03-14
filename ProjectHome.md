ASTRAN is a free, open source, physical synthesis framework for Mac and Linux .<br>
<br>
It supports automatic layout generation of CMOS cells from a transistor level netlist description in SPICE format. Area overhead compared to high quality hand-made standard-cells layouts is 3.7% on average.<br>
It's functionality is being extended to produce complete circuit layouts. Features include transistor sizing, floorplanning, cell placement, and routing. The framework is very modular and new contributions to this project are welcome.<br>

<img src='https://gme-ufrgs.googlecode.com/files/screenshot2.png' />

<br>
<b>Features:</b><br>
- ASTRAN 3 is under active development and supports <a href='https://gme-ufrgs.googlecode.com/files/RULES.png'>+45nm technology rules</a> (including freePDK45) like: redundant diff contacts, conditional design rules (metal/poly/diff enclosure of contacts, gate extension on diff, diff to gate spacing, metal/poly spacing on dense line-ends), minimum area,...<br>
- It generates all kinds of logic cells, including non-complementary as PTL;<br>
- Cell instances can be individually sized before generation to reduce delay;<br>
- Cell layout generation flow includes: transistor folding, 1-D transistor placement (using Threshold Accepting metaheuristic), intracell routing (using pathfinder and iterated 1-steiner algorithms) and 2-D layout compaction (using ILP);<br>
- Layouts can be saved in CIF, GDSII and LEF formats. It can also be exported to Cadence ICFB and loaded into Virtuoso;<br>
- Structural Verilog is supported once you provide a Spice description of each cell;<br>
- Includes a command line interpreter for increased flexibility, like the ability to run scripts;<br>
<br>
TIP: Download and install always the latest version directly from the GIT repository. Binary versions will not be updated anymore since google closed this service. It's pretty easy to <a href='https://code.google.com/p/gme-ufrgs/wiki/Install_Instructions'>compile</a>.<br>
<br>
There is a video tutorial that can be usefull in: <a href='https://www.youtube.com/watch?v=IFbCDSMi_FE'>https://www.youtube.com/watch?v=IFbCDSMi_FE</a><br>
Any question about the tool, please, send me an email: <b>amziesemerj (at) inf.ufrgs.br</b><br>
<br>
If you are using my tool, please cite: <a href='https://www.researchgate.net/publication/262876168_Simultaneous_Two-Dimensional_Cell_Layout_Compaction_Using_MILP_with_ASTRAN?ev=prf_pub'>ISVLSI'14</a> or visit my <a href='https://www.researchgate.net/profile/A_Ziesemer_Jr'>profile</a> for other works <br>
<br>
Layout examples of automatically generated cells:<br>
<br>
<img src='https://gme-ufrgs.googlecode.com/files/ADD31.png' />
<img src='https://gme-ufrgs.googlecode.com/files/xor2.png' />
<img src='https://gme-ufrgs.googlecode.com/files/aoi21.png' />
<img src='https://gme-ufrgs.googlecode.com/files/oai2112.png' />