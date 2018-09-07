# 要安装python-dev
rm -fr TestBuff.so build/
python setup.py build
cp  build/lib.linux-x86_64-2.7/TestBuff.so ./
