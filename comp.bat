PATH=C:\z88dk\bin;%PATH% 
zcc +vz -O3 -subtype=basic  -o main.vz main.c
del zcc_opt.def main.reloc
appmake +vz -b main.vz --audio --fast
pause
