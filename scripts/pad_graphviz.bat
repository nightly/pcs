@echo off

pushd ..
cd exports/pad
pwd
dot -Tpng highlighted_topology.txt -o highlighted_topology.png
dot -Tpng topology.txt -o topology.png
popd
pause