*
* Ares Circuit Synthesis 
* Reginaldo Tavares 
* regi@inf.ufrgs.br
*

*interface n1 N I
*interface n2 N I
*interface n3 N I
*interface n4 S O
*interface n12 S O

.subckt inv in out vcc gnd
MN1 out in gnd gnd NMOS L=0.35U W=2.0U
MP2 out in vcc vcc PMOS L=0.35U W=2.0U
.ends inv

.subckt buf2 in out vcc gnd
X100 in tmp2 vcc gnd inv
X101 tmp2 out vcc gnd inv
.ends buf2

.subckt buf4 in out vcc gnd
X102 in tmp2 vcc gnd buf2
X103 tmp2 out vcc gnd buf2
.ends buf4

.subckt buf in out vcc gnd
MN1 out s1 gnd gnd NMOS L=0.35U W=2.0U
MP2 out s1 vcc vcc PMOS L=0.35U W=2.0U
MN3 s1 in gnd gnd NMOS L=0.35U W=2.0U
MP4 s1 in vcc vcc PMOS L=0.35U W=2.0U
.ends buf

.subckt and2 a b out vcc gnd
MN1 out a b gnd NMOS L=0.35U W=2.0U
MP2 out a gnd vcc PMOS L=0.35U W=2.0U
.ends and2

.subckt nand2 a b out vcc gnd
MN1 out a b0 gnd NMOS L=0.35U W=2.0U
MP2 out a vcc vcc PMOS L=0.35U W=2.0U
MN3 b0 b gnd gnd NMOS L=0.35U W=2.0U
MP4 b0 b vcc vcc PMOS L=0.35U W=2.0U
.ends nand2

.subckt or2 a b out vcc gnd
MN1 out a vcc gnd NMOS L=0.35U W=2.0U
MP2 out a b vcc PMOS L=0.35U W=2.0U
.ends or2

.subckt nor2 a b out vcc gnd
MN1 out a gnd gnd NMOS L=0.35U W=2.0U
MP2 out a b0 vcc PMOS L=0.35U W=2.0U
MN3 b0 b gnd gnd NMOS L=0.35U W=2.0U
MP4 b0 b vcc vcc PMOS L=0.35U W=2.0U
.ends nor2

.subckt mux21 a b c out vcc gnd
MN1 s1 a b gnd NMOS L=0.35U W=2.0U
MP2 s1 a c vcc PMOS L=0.35U W=2.0U
MN3 s2 s1 gnd gnd NMOS L=0.35U W=2.0U
MP4 s2 s1 vcc vcc PMOS L=0.35U W=2.0U
MN5 out s2 gnd gnd NMOS L=0.35U W=2.0U
MP6 out s2 vcc vcc PMOS L=0.35U W=2.0U
.ends mux21

.subckt muxi21 a b c out vcc gnd
MN1 s1 a b gnd NMOS L=0.35U W=2.0U
MP2 s1 a c vcc PMOS L=0.35U W=2.0U
MN5 out s1 gnd gnd NMOS L=0.35U W=2.0U
MP6 out s1 vcc vcc PMOS L=0.35U W=2.0U
.ends muxi21

X1 n13 n4 n12 vcc gnd nand2
X2 n16 n14 n13 vcc gnd nand2
X3 n2 n16 vcc gnd inv
X4 n1 n15 n14 vcc gnd nor2
X5 n0 n15 vcc gnd inv
X6 n3 n5 n8 n4 vcc gnd muxi21
X7 n2 n6 n7 n5 vcc gnd mux21
X8 n1 n6 vcc gnd buf
X9 n0 n7 vcc gnd buf
X10 n2 n9 n11 n8 vcc gnd mux21
X11 n1 n10 n9 vcc gnd nand2
X12 n0 n10 vcc gnd inv
X13 n0 n11 vcc gnd buf

