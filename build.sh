DIR=`pwd`

if [ -d "build" ]
then
  rm -rf build
fi
mkdir build
cd build
mkdir bin
cmake ..
make -j4

cd ${DIR}/test
if [ -d "build" ]
then
  rm -rf build
fi
mkdir build
cd build
mkdir bin
cmake ..
make -j4
