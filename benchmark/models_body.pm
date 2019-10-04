**
.subckt nch d g s b w=1u l=1u nf=1 nrd=1 nrs=1 verbose=0
+rgatemod=0 rdsmod=0 rbodymod=0
*.model myvamod bsim4_va
.model myvamod bsim4_body
+paramchk= 1               mobmod  = 0               capmod  = 2
+igcmod  = 0               igbmod  = 0               geomod  = 0
+rgeomod = 0               rdsmod  = 1               rbodymod= 1
+diomod  = 2               rgatemod= 1               permod  = 1
+acnqsmod= 0               trnqsmod= 0               tempmod = 0
+wpemod  = 0
+toxe    = 1.29e-008       dtox    = 0
+epsrox  = 3.9             toxref  = 1.29e-008       wint    = 6.6501e-008
+lint    = -5.0365913e-008  ll      = 3.5982021e-013  wl      = -4.0019e-014
+lln     = 0.85018606      wln     = 1               lw      = -1.3343e-014
+ww      = -1.1942e-014    lwn     = 1               wwn     = 0.99397
+lwl     = -1.0904e-021    wwl     = 2.41e-021       llc     = -5.16e-014
+wlc     = 0               lwc     = 0               wwc     = 0
+lwlc    = 0               wwlc    = 0               xl      = 0
+xw      = -3e-008         dlc     = 2.0366e-007     dwc     = 0
+dlcig   = -5e-008         xpart   = 0
+vth0    = 0.40866         lvth0   = 2.451e-007      wvth0   = -3.0899e-008
+pvth0   = 8.231e-015      k1      = 0.528           k2      = 0.016
+lk2     = -3.929e-008     wk2     = -1.2e-008       pk2     = 6.3072e-015 
+k3      = 47.793          k3b     = 9.2             w0      = 6.3937e-006
+dvt0    = 0.43152         dvt1    = 0.23623         dvt2    = -0.05
+dvt0w   = 1.0065e-013     dvt1w   = 0.098644        dvt2w   = 0
+dsub    = 0.30631         minv    = -0.6            lminv   = -1.0624e-008 
+pminv   = 6.15e-014       voffl   = -1.2e-008       dvtp0   = 1.6712e-006 
+dvtp1   = 0.05            lc      = 5e-009          lambda  = 0
+vtl     = 200000          xn      = 3               lpe0    = -4.6674e-007
+lpeb    = -0              vbm     = -3              xj      = 2.5e-007
+ndep    = 1.7e+017        nsd     = 1e+020          phin    = 0
+cdsc    = 0               ud      = 0               ud1     = 0
+up      = 0               lp      = 1e-008          cdscb   = 0
+cdscd   = 0               cit     = 0               voff    = -0.15464
+lvoff   = -5.0652e-009    wvoff   = -5.0803e-010    pvoff   = 1.7e-015
+nfactor = 1.1968          eta0    = 0.042           leta0   = 1.3624e-007 
+weta0   = 3.1205e-008     peta0   = -1.9595e-015    etab    = -0.07
+vfb     = -1              u0      = 0.020608        lu0     = 9.7257e-009
+pu0     = -1.4161e-015    ua      = -3.4732e-010    lua     = -3.5229e-016
+ub      = 4.894e-019      lub     = 1.3247e-025     wub     = -2.6223e-026 
+pub     = -2.4274e-031    uc      = 7.8797e-011     puc     = -3.0031e-023
+eu      = 1.67            vsat    = 47509           lvsat   = 0.0033788 
+wvsat   = 0.004672        a0      = 2.1292          la0     = -9.5536e-007 
+wa0     = -4.24e-009      pa0     = +2.2106e-015    ags     = 0.061238 
+lags    = 9.9243e-008     wags    = -6.2343e-009    pags    = 6.0095e-014
+a1      = 0               a2      = 1               b0      = 0
+b1      = 0               keta    = -0.0091695      lketa   = -2.3452e-009
+wketa   = -9.5684e-010    pketa   = 1.8420e-015     dwg     = 0
+dwb     = 0               pclm    = 0.04            wpclm   = 1.7e-008
+pdiblc1 = 0.001           pdiblc2 = 4.72e-010       pdiblcb = 0
+drout   = 0.56            pvag    = 0               delta   = 0.01
+ldelta  = 9.8135e-009     pscbe1  = 1.8145e+008     pscbe2  = 1.211e-006
+lpscbe2 = -6.0192e-013    fprout  = 0               pdits   = 0
+pditsd  = 0               pditsl  = 0               rsh     = 4
+rdsw    = 1030            rsw     = 0.2             rdw     = 0.7
+rdswmin = 0               rdwmin  = 0               rswmin  = 0
+prwg    = 0.05384         prwb    = 0               wr      = 1 
+alpha0  = 1.2137e-005     lalpha0 = 2.7412e-011     palpha0 = 4e-018
+alpha1  = 8.4304          lalpha1 = -9.7303e-007    beta0   = 33.14
+lbeta0  = 4.9364e-006     agidl   = 9.1e-008        lagidl  = 4e-014
+wagidl  = 7.2e-013        bgidl   = 2.5576e+009     cgidl   = 0.5
+egidl   = 0.8             aigbacc = 0.0136          bigbacc = 0.00171
+cigbacc = 0.075           nigbacc = 1               aigbinv = 0.0111
+bigbinv = 0.000949        cigbinv = 0.006           eigbinv = 1.1
+nigbinv = 3               aigc    = 0.43            bigc    = 0.054
+cigc    = 0.075           aigsd   = 0.43            bigsd   = 0.054
+cigsd   = 0.075           nigc    = 1               poxedge = 1
+pigcd   = 1               ntox    = 1               vfbsdoff= 0
+cgso    = 6.1e-013        cgdo    = 6.1e-013        cgbo    = 0
+cgdl    = 4.716e-010      cgsl    = 4.716e-010      clc     = 0
+cle     = 0.6             cf      = 6.62e-011       ckappas = 0.6
+ckappad = 0.6             vfbcv   = -1              acde    = 0.38163
+moin    = 15              noff    = 1.982           lnoff   = 1e-006
+voffcv  = -0.08           lvoffcv = -6e-008
+tvoff   = -0.00104        tvfbsdoff= 0               kt1     = -0.40824
+wkt1    = 1e-008          pkt1    = -7.7424e-015    kt1l    = -4.752e-009
+kt2     = -0.045556       ute     = -1.0517         lute    = 1.8e-007
+wute    = 8e-008          pute    = 2.4864e-014     ua1     = 2.37e-009
+lua1    = 5.3697e-016     wua1    = -4.4e-017       pua1    = -3.4479e-023
+ub1     = -1.976e-018     uc1     = -5.6e-011       prt     = 0
+at      = 26389           wat     = -0.0032
+fnoimod = 0               tnoimod = 0               em      = 4.1e+007
+ef      = 0.968           noia    = 1.525e+042      noib    = 1.3125e+024
+noic    = 1.75e+007       ntnoi   = 1               
+jss     = 1.5665e-007     jsws    = 2.784e-014      jswgs   = 4.1397e-014
+njs     = 1.0138          ijthsfwd= 0.1             ijthsrev= 0.1
+bvs     = 9.7             xjbvs   = 1               jtss    = 0
+jtsd    = 0               jtssws  = 0               jtsswd  = 0
+jtsswgs = 4.8411e-012     jtsswgd = 4.8411e-012     njts    = 20
+njtssw  = 20              njtsswg = 20              xtss    = 0.02
+xtsd    = 0.02            xtssws  = 0.02            xtsswd  = 0.02
+xtsswgs = 0.02            xtsswgd = 0.02            tnjts   = 0
+tnjtssw = 0               tnjtsswg= 0               pbs     = 0.74354
+cjs     = 0.0012269       mjs     = 0.32351         pbsws   = 0.64511
+cjsws   = 3.2329e-011     mjsws   = 0.13507         pbswgs  = 0.79417
+cjswgs  = 1.8223e-010     mjswgs  = 0.59796         tpb     = 0.0013647
+tcj     = 0.00076432      tpbsw   = 0.00019547      tcjsw   = 0.00054525
+tpbswg  = 0.00060249      tcjswg  = 0.0023126       xtis    = 3
+dmcg    = 3.7e-007        dmci    = 4.37e-006       dmdg    = 0
+dmcgt   = 0               dwj     = 0               xgw     = 0
+xgl     = 0
+rshg    = 0.1             gbmin   = 1e-012          rbpb    = 50
+rbpd    = 50              rbps    = 50              rbdb    = 50
+rbsb    = 50              ngcon   = 1               xrcrg1  = 12
+xrcrg2  = 1               rbps0   = 50              rbpsl   = 0
+rbpsw   = 0               rbpsnf  = 0               rbpd0   = 50
+rbpdl   = 0               rbpdw   = 0               rbpdnf  = 0
+rbpbx0  = 100             rbpbxl  = 0               rbpbxw  = 0
+rbpbxnf = 0               rbpby0  = 100             rbpbyl  = 0
+rbpbyw  = 0               rbpbynf = 0               rbsbx0  = 100
+rbsby0  = 100             rbdbx0  = 100             rbdby0  = 100
+rbsdbxl = 0               rbsdbxw = 0               rbsdbxnf= 0
+rbsdbyl = 0               rbsdbyw = 0               rbsdbynf= 0
+web     = 0               wec     = 0               scref   = 1e-006
+kvth0we = 0               k2we    = 0               ku0we   = 0
+saref   = 4.74e-006       sbref   = 4.74e-006       wlod    = 0
+kvth0   = -1e-009         lkvth0  = 0               wkvth0  = -8.4e-016
+pkvth0  = 0               llodvth = 1               wlodvth = 1
+stk2    = 0               lodk2   = 1               lodeta0 = 1
+ku0     = -7.748e-008     lku0    = 2.28e-007       wku0    = 4.56e-007
+pku0    = -1.6e-013       llodku0 = 1               wlodku0 = 1
+kvsat   = 0.14            steta0  = 0               tku0    = 0
+tnom    = 27

*xmain d g s b myvamod w=w l=l nf=nf nrd=nrd nrs=nrs verbose=verbose 
*+rgatemod=rgatemod rdsmod=rdsmod rbodymod=rbodymod

*ybsim4_va myvadev d g s b myvamod w=w l=l nf=nf nrd=nrd nrs=nrs verbose=verbose
*+rgatemod=rgatemod rdsmod=rdsmod rbodymod=rbodymod

ybsim4_body myvadev d g s b myvamod w=w l=l nf=nf nrd=nrd nrs=nrs verbose=verbose
+rgatemod=rgatemod rdsmod=rdsmod rbodymod=rbodymod

.ends nch


