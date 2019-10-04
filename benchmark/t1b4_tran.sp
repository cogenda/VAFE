* 1T device to test DC with VA bsim
.xyceinstallpath "/home/abc/spice/cygsp/xyceDeps/xyce/"
.hdlheaderpath "/home/abc/spice/cygsp/xyceDeps/vaheader"
.hdl "./bsim4_body.va"
.inc models_body.pm
.option ingold=2 numdgt=7
.step temp list 27
.temp 27

vd d 0 pwl 0 0 10n 1.2
vg g 0 1.2
vs s 0 0
vb b 0 0
*vb b 0 0

xm1 d g s b nch l=5u w=1u nrd=3 nrs=7 rdsmod=1 rgatemod=1 rbodymod=1 verbose=0

.print tran i(vd)
.tran 0.1n 10n
.end
