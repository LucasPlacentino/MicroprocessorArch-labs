# Run your simulation by using: make -f TestMakefile.mk PROGRAM=yourprogramfile
# Make sure to press enter to continue the simulation. If you don't want to have 
# an interactive session, set INT to "no"


# PROGRAM
PROGRAM = program1
INT= yes

# MICRO-ARCHITECTURE CONFIGURATION
TOM     = no
LREGS   = 8
PREGS   = 24
PSTRUCT = FDPIXWC
UNILSU  = yes
LOADPRI = yes
IOI		= no
IOC		= no
UNIDI	= yes
RSREL	= yes
MEMPIPE = yes

# uARCH FRONT END PARAMETERS
FW   = 4
DW   = 4
PW   = 4
IW   = 4
WW   = 4
CW 	 = 4
WINS = 16
ROBS = 16

# ALU PARAMETERS
AFU   = 4
ALAT  = 0
ARS	  = 100

# MULT UNIT PARAMETERS
MFU	  = 1
MLAT  = 4
MPIPE = yes
MRS	  = 100

# DIV UNIT PARAMETERS
DFU   = 1
DLAT  = 4
DPIPE = yes
DRS	  = 100 		# lately, the "Overtake Mode"

# FLOATING POINT UNIT
FFU	  = 4
XFU	  = 1

# LOAD UNIT
LFU	  = 1
LLAT  = 2
LPIPE = yes

# STORE UNIT
SFU	   = 1
SLAT   = 1
SPIPE  = YES
SWAITS = no

# LOAD-STORE
LSRS   = 100

# BRANCH UNIT
BFU	   = 1
BLAT   = 0
BRS    = 100

# QUEUE SIZE
LQS	   = 1
SQS	   = 1

# SPECULATION
SPEC   = yes
WBLAT  = 0

# Params
PARAMS = INT TOM LREGS PREGS PSTRUCT UNILSU LOADPRI IOI IOC UNIDI RSREL MEMPIPE \
         FW DW PW IW WW CW WINS ROBS \
         AFU ALAT ARS \
         MFU MLAT MPIPE MRS \
         DFU DLAT DPIPE DRS \
         FFU XFU \
         LFU LLAT LPIPE \
         SFU SLAT SPIPE SWAITS \
         LSRS \
         BFU BLAT BRS \
         LQS SQS \
         SPEC WBLAT

lower = $(shell echo $(1) | tr A-Z a-z)
FLAGS = $(foreach v, $(PARAMS), -$(call lower,$(v)) $($(v)))

run:
	./freess $(FLAGS) -exe $(PROGRAM) 2>&1 | tee $(PROGRAM)_log.log