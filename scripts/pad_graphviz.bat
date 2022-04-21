@echo off

pushd ..
cd exports/pad
pwd
dot -Tpng recipe.txt -o recipe.png
dot -Tpng Resource1.txt -o Resource1.png
dot -Tpng Resource2.txt -o Resource2.png
dot -Tpng Resource3.txt -o Resource3.png
dot -Tpng Resource4.txt -o Resource4.png
dot -Tpng Resource5.txt -o Resource5.png

dot -Tpng highlighted_topology.txt -o highlighted_topology.png
dot -Tpng topology.txt -o topology.png
popd
pause