# AVX lifter torture suite — flush out INTERRs / decompilation failures.
#
#   make torture            # default sweep (10 seeds x 400 funcs, C + raw-asm)
#   make torture SEEDS=50 FUNCS=800
#   make seed SEED=7 FUNCS=800   # one reproducible seed, keeps _t7.* artifacts
#   make clean
#
# Exits non-zero if any INTERR / decompile failure is found.

IDUMP ?= idump
SEEDS ?= 10
FUNCS ?= 400
SEED  ?= 1

.PHONY: torture
torture:
	@python3 run_torture.py --seeds $(SEEDS) --funcs $(FUNCS) --idump $(IDUMP)

# FULL matrix over ALL gen_*.py generators x {gcc,clang,mingw-w64} x
# {ELF64,ELF32,PE64,PE32} x opt levels, incl. Windows PE binaries and the
# EVEX-fringe / illegal-encoding asm. This reproduces the wild INTERRs.
.PHONY: matrix
matrix:
	@python3 torture_matrix.py --seeds $(SEEDS) --funcs $(FUNCS) --idump $(IDUMP)

.PHONY: seed
seed:
	@python3 run_torture.py --seed $(SEED) --funcs $(FUNCS) --idump $(IDUMP) --keep

.PHONY: clean
clean:
	@rm -f _* *.o tg.* at.* matrix_run*.log 2>/dev/null; echo cleaned
