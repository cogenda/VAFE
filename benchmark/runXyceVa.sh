cmd=$1
#sp='./testva.sp'
#sp='./t1b4_tran.sp'
#sp='./t1b4.sp'
sp='./testnodecllap.sp'
#sp='./testx1_b4.sp'
hsp=`basename $sp .sp`_hsp.sp
sim=/spice/cygsp/xyceDeps/xyce/bin/Xyce
simtop=/spice/cygsp/Xyce-6.8
pluginpath=$simtop/build/user_plugin
plugin=$pluginpath/.libs/libADMSrlc.so
PATH=.:$PATH 
LD_LIBRARY_PATH=.:/tools/adms/lib:/spice/cygsp/VAFE/3rdparty/lib:${LD_LIBRARY_PATH}

if [ "$cmd" = "vams" ]; then
 $sim $sp
elif [ "$cmd" = "plugin" ];then
 $sim -plugin $plugin $sp
elif [ "$cmd" = "dbg" ];then
 gdb $sim 
elif [ "$cmd" = "dbggui" ];then
 clewn -ga $sim -va $simtop/src/Xyce.C
elif [ "$cmd" = "hspice" ];then
 hspice $hsp  -o out/`basename $hsp .sp`
elif [ "$cmd" = "mkplugin" ];then
 cd $pluginpath; make clean ; make plugin 
elif [ "$cmd" = "mkxyce" ];then
 cd $pluginpath/../; make install 
else
  echo "usage: $0 vams|plugin|dbg|dbggui|mkplugin|mkxyce"
fi



