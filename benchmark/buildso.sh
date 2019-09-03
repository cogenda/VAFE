lname=$1
g++ -g -O0   -I"/spice/cygsp/xyceDeps/xyce/"/include/   -fPIC -c vabuild/${lname}.C -o vabuild/${lname}.o
g++ -g -O0 -m64 -Wl,-O1 -shared -o vabuild/lib${lname}.so -L"/spice/cygsp/xyceDeps/xyce/"/lib/ -lxyce   -Wl,-rpath="/spice/cygsp/xyceDeps/xyce/"/lib vabuild/${lname}.o
