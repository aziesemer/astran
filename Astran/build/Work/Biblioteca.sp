.subckt INV2 A Q GND VCC
  MN Q A GND GND nmos L=65n W=480n
  MP Q A VCC VCC pmos L=65n W=1040n
.ends

.subckt INV1 A Q GND VCC
  MN Q A GND GND nmos L=65n W=240n
  MP Q A VCC VCC pmos L=65n W=520n
.ends

.subckt INV0 A Q GND VCC
  MN Q A GND GND nmos L=65n W=120n
  MP Q A VCC VCC pmos L=65n W=260n
.ends

.subckt INV0f A Q GND VCC
  MN Q A GND GND nmos L=65n W=120n
  MP Q A VCC VCC pmos L=65n W=140n
.ends

.subckt DDB0 A Q GND VCC
  MN1 nint A GND GND nmos L=65n W=120n
  MP1 nint A VCC VCC pmos L=65n W=120n
  MN2 Q nint GND GND nmos L=65n W=120n
  MP2 Q nint VCC VCC pmos L=65n W=120n 
.ends

.subckt TGATE1 en ep in Q GND VCC
  MP1 Q ep in VCC pmos L=65n W=240n
  MN1 Q en in GND nmos L=65n W=240n
.ends

.subckt NAND21 a b Q GND VCC
  MN2 net13 b GND GND nmos W=480n L=65n
  MN1 Q a net13 GND nmos W=480n L=65n
  MP2 Q b VCC VCC pmos W=520n L=65n
  MP1 Q a VCC VCC pmos W=520n L=65n
.ends

.subckt CLINV1 A C CN Q GND VCC
  MN0 Q A net18 GND nmos W=240n L=65n
  MN1 net18 C GND GND nmos W=240n L=65n
  MP0 Q A net10 VCC pmos W=520n L=65n
  MP1 net10 CN VCC VCC pmos W=520n L=65n
.ends

.subckt DFP1 C D Q QN SN GND VCC
  X49 D CN CI net48 GND VCC CLINV1 
  X57 net40 net57 GND VCC INV1  
  X58 net63 net47 GND VCC INV1 
  X59 net57 Q GND VCC INV1 
  X60 net55 QN GND VCC INV1 
  X55 SN net48 net63 GND VCC NAND21 
  X56 SN net57 net55 GND VCC NAND21 
  X52 CN CI net55 net40 GND VCC TGATE1  
  X53 CI CN net47 net48 GND VCC TGATE1 
  X54 CI CN net63 net40 GND VCC TGATE1 
  X51 C CN GND VCC INV0 
  X50 CN CI GND VCC INV0 
.ends DFP1

******* NUEVOS 29-Abril-09:

.SUBCKT DFC1_G5 C D Q QN RN GND VCC
XI50 D CN CI NET48 GND VCC CLINVA_CORE_1   
XI58 NET48 NET63 GND VCC INV1  
XI59 NET57 Q GND VCC INV1  
XI60 NET55 QN GND VCC INV1  
XI61 NET57 NET55 GND VCC INV1  
XI56 RN NET63 NET47 GND VCC NAND21 
XI57 RN NET40 NET57 GND VCC NAND21  
XI53 CN CI NET55 NET40 GND VCC TGATE1  
XI54 CI CN NET47 NET48 GND VCC TGATE1  
XI55 CI CN NET63 NET40 GND VCC TGATE1  
XI52 C CN GND VCC INVB_CORE_5  
XI51 CN CI GND VCC INV0 
.ENDS DFC1_G5  

.SUBCKT CLINVA_CORE_1 A C CN Q GND VCC
MN0 Q A NET18 GND  nmos  L=65n W=240n
MN1 NET18 C GND GND  nmos  L=65n W=240n
MP0 Q A NET10 VCC  pmos  L=65n W=520n 
MP1 NET10 CN VCC VCC  pmos  L=65n W=520n
.ENDS CLINVA_CORE_1 

.SUBCKT INVB_CORE_5 A Q GND VCC
MP2 Q A VCC VCC  pmos L=65n W=240n
MN2 Q A GND GND  nmos L=65n W=120n
.ENDS INVB_CORE_5 

.SUBCKT XOR20_G6 A B Q GND VCC
XI5 B A NET8 GND VCC NOR2_CORE_1  
XI6 A B NET8 Q GND VCC AOI21_CORE_2  
.ENDS XOR20_G6 

.SUBCKT NOR2_CORE_1 A B OUT GND VCC
MN2 OUT A GND GND    nmos  L=65n W=120n
MN1 OUT B GND GND    nmos  L=65n W=120n
MP2 NET17 B VCC VCC  pmos  L=65n W=416n
MP1 OUT A NET17 VCC   pmos  L=65n W=416n
.ENDS NOR2_CORE_1 

.SUBCKT AOI21_CORE_2 A B C OUT GND VCC
MN3 OUT C GND GND  nmos  L=65n W=120n
MN1 OUT A NET14 GND nmos  L=65n W=240n 
MN2 NET14 B GND GND  nmos  L=65n W=240n
MP3 OUT C NET18 VCC   pmos  L=65n W=520n
MP2 NET18 B VCC VCC  pmos  L=65n W=520n
MP1 NET18 A VCC VCC  pmos  L=65n W=520n
.ENDS AOI21_CORE_2 

.SUBCKT XTRAN_G7 Q X0 X1 X2 X3 X4 X5 X6 GND VCC
XI13 X4 X5 X6 NET11 GND VCC NAND30_G1  
XI11 NET18 NET26 NET24 NET22 Q GND VCC OAI2112_G2  
XI6 NET30 NET28 NET18 GND VCC NOR20_G3  
XI5 NET11 NET22 GND VCC INV0  
XI4 X3 NET24 GND VCC INV0  
XI3 X2 NET26 GND VCC INV0 
XI2 X1 NET28 GND VCC INV0  
XI1 X0 NET30 GND VCC INV0 
.ENDS XTRAN_G7 

.SUBCKT NAND30_G1 A B C OUT GND VCC
MN2 NET6 B NET10 GND  nmos  L=65n W=360n
MN3 NET10 C GND GND  nmos  L=65n W=360n
MN1 OUT A NET6 GND    nmos  L=65n W=360n
MP2 OUT B VCC VCC  pmos  L=65n W=260n
MP3 OUT C VCC VCC  pmos  L=65n W=260n 
MP1 OUT A VCC VCC  pmos  L=65n W=260n 
.ENDS NAND30_G1 

.SUBCKT OAI2112_G2 A B C D Q GND VCC
MN4 NET7 D GND GND  nmos  L=65n W=1440n 
MN3 NET11 C NET7 GND  nmos  L=65n W=1440n 
MN1 Q A NET11 GND  nmos  L=65n W=1440n 
MN2 Q B NET11 GND  nmos  L=65n W=1440n
MP3 Q C VCC VCC  pmos  L=65n W=1040n 
MP1 Q A NET27 VCC  pmos  L=65n W=2080n 
MP2 NET27 B VCC VCC  pmos  L=65n W=2080n 
MP4 Q D VCC VCC  pmos  L=65n W=1040n
.ENDS OAI2112_G2

.SUBCKT NOR20_G3f A B OUT GND VCC
MN2 OUT A GND GND    nmos  L=65n W=120n 
MN1 OUT B GND GND    nmos  L=65n W=120n 
MP2 NET17 B VCC VCC  pmos  L=65n W=520n
MP1 OUT A NET17 VCC   pmos  L=65n W=520n
.ENDS NOR20_G3f 

.SUBCKT NOR20_G3 A B OUT GND VCC
MN2 OUT A GND GND    nmos  L=65n W=120n
MN1 OUT B GND GND    nmos  L=65n W=120n
MP2 NET17 B VCC VCC  pmos  L=65n W=520n
MP1 OUT A NET17 VCC   pmos  L=65n W=520n
.ENDS NOR20_G3 
