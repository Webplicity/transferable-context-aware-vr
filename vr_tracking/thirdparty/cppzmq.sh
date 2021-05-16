git clone https://github.com/zeromq/libzmq.git
cd libzmq
mkdir build
cd build
cmake ..
sudo make -j4 install

cd ../../

git clone https://github.com/zeromq/cppzmq.git
cd cppzmq
mkdir build
cd build
cmake ..
sudo make -j4 install