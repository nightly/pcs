@echo off

pushd ..
cd exports/merge
pwd
dot -Tpng lts1.txt -o lts1.png
dot -Tpng lts2.txt -o lts2.png
dot -Tpng combined-lts.txt -o combined-lts.png
popd
pause