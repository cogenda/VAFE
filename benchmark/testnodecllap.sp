* 
* Non Salicide N+ diffusion resistor subcircuit 
* netlist for typical case              

.xyceinstallpath "/spice/cygsp/xyceDeps/xyce/"
.hdlheaderpath "/spice/cygsp/xyceDeps/vaheader"
*.opt post=fsdb
.opt post ingold=2
*.hdl va04.va
.hdl nodecollap.va
*.hdl va008chg.va

.subckt rnnd_spl130e  n2 n1
.model myvamod nodecollap r=1e-3
ynodecollap myvadev n2 n1 myvamod 
*xva01g n2 n1 myvamod 
.ends rnnd_spl130e
 
x1 1 0 rnnd_spl130e  

v1 1 0 pwl 0 0 10n 1
.print tran i(v1) 
.tran 10p 10n 
.end

