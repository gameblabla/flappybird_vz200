PATH=C:\z88dk\bin;%PATH% 
zcc +vz -subtype=basic -compiler=sdcc -SO3 -opt-code-size -o main.vz main.c
del zcc_opt.def main.reloc
appmake +vz -b main.vz --audio --fast
pause
