#ifndef STATES
#define STATES

extern void print_tok(int);

/* TOKEN ids

DOn'T forget to update print_tok funct
 */
/* WARNING!!!!!!!!!!!!!!!!
  HARD CODED. Never change anything, new items add only to end!
   */
#define  PRG     0
#define  VAR     1
#define  TAM     2
#define  KESR    3
#define  SIMVOL  4
#define  IDT     5
#define  VERGUL   6
#define  NOQTEV  7
#define  KOD     8
#define  CAPET   9     /* capet, print */
#define  OPNBRK  10   /* (  */
#define  STR     11
#define  CHR     12
#define  CLSBRK  13
#define  SON     14
#define  DOT     15
#define  LEXEM   16
#define  NUMB    17
#define  SIGN    18
#define  DIG     19
#define  POSDIG  20
#define  POSNUMB 21
#define  SNUMB   22
#define  POSSIG  23
#define  NEGSIG  24
#define  SLC     25
#define  MLC     26
#define  SPS     27
#define  COLON   28
#define  ARYOPN  29
#define  ARYCLS  30
#define  ASGN     31
#define  MULT     32
#define  DEL      33
#define  PRCT     34
#define  XAR      35
#define  DAXIL    36
#define  EGER     37
#define  NIDA     38
#define  GRT      39
#define  LES      40
#define  VE       41
#define  VEYA     42
#define  FILESON  43
#define  OPNBLK   44
#define  CLSBLK   45
#define  ONDA     46
#define  YOXSA    47
#define  BERABER  48
#define  FERQLI   49
#define  LESEQU   50
#define  GRTEQU   51
#define  EMPTOK   52
#define  SHERT    53
#define  IFAD     54
#define  DOVR     55
#define  ARTSIN   56
#define  AZALSIN  57
#define  ADDIM    58
#define  KIMI     59
#define  CRG1     60
#define  CRG2     61
#define  TEKRARET 62
#define  DAN      63
#define  DEN      64


/* not token  */

#define  CAPET_OP  65
#define  DOVR_OP   66
#define  BLOK     67
#define  BIRTIP    68
#define  TIPLER    69
#define  EGER_OP   70
#define  ASGN_OP    71
#define  DAXIL_OP  72
#define  BLOKSUZ   73
#define  BLOKLU    74
#define  KODDATALAR 75
#define  BLOKLUDATA 76

#define YE       77
#define YA       78
#define EE        79
#define AA        80

#define DOVR_EXP   81
#define EGER_EXP   82

#define DOVR_BLOKSUZ 83
#define EGER_BLOKSUZ 84
#define YOXSA_BLOKSUZ 85

#define KODDATA    86

#define DOVR_BLOKLU 87
#define EGER_BLOKLU 88
#define YOXSA_BLOKLU 89

#define EGER_TEK_BLOKSUZ 90
#define EGER_TAM_BLOKSUZ 91

#define TAM1  92
#define TAM2  93

#define KESR1  94
#define KESR2  95

#define SIMVOL1  96
#define SIMVOL2  97

#define VAR1  99
#define VAR2  100

#define DAXILET1 101
#define DAXILET2 102
#define CAPET1 103
#define CAPET2 104

#define EGER_EXP1 105
#define EGER_EXP2 106

#define KOD1  107

#define PRG1  108

#define BADSTR 109
#define BADIDT 110
#define BADCHR 111
#define DAXILET3 112
#define EMPTYOP 113
#define CAPET3 114

#define EXPR  115

#define OPR 116
#define BRK 117

#define FILEBEG 118
#define FLOAT_HALF 119
#define FLOAT 120


/* funksiyalar */

#define FUNKSIYA 121 /* ancaq "funksiya" sozu  */
#define FUNK_ELAN 122
#define FUNK_BODY 123

#define EMPBRK 124 /* icibosh moterize , ()  */
#define FE1  125  /* funskya deyishenleri elani , moterize ici  */
#define FE2  126  /* moterize ile birlikte  */

#define FUNK 127
#define FUNKS 128

#define FCALL 129
#define FCALL1 130
#define VDEC   131

#define DUMMY 132

#define FDECL 133

#define PRG2 134

#define QAYTAR 135
#define QAYTAR1 136

#define NEQEDERKI 137
#define NEQEDERKI1 138
#define NEQEDERKI2 139

#define DAVAMET 140
#define DAYAN 141

#define DAVAMET1 142
#define DAYAN1 143

#define SEC 144
#define SEC_OP 145

#define HAL 146
#define HAL1 147
#define HAL2 148
#define HAL3 149

#define SEC1 150
#define SUSMA 151

#define START 152
#define STRUKT 153
#define STRUKT1 154
#define STRUKT2 155
#define STRUKT3 156


#define YNT1 157
#define YNT2 158

#define STRMEM 159 /* strukt member  */

#define YNTS 160

#define SINIF 161
#define MEXSUSI 162
#define SERBEST 163
#define QORUNAN 164
#define SINIF_HEAD 165
#define SINIF_HEAD_START 166
#define SINIF_BODY 167
#define SINIF_DECL 168
#define SINIF_TYPE 169

#define YENI_TIP 170

#define SNMEM 171

#define INIT_DOVR 172
#define CHECK_DOVR 173
#define STEP_DOVR 174

#define NEQ_TOP 175
#define SUS1 176
#define HAL1S 177

#define FARGS 178
#define FPARM 179

#define FCALL_BEG 180
#define PUT_RET_ADDR 181

#define IDT_ADRS 182
#define CRG1_ADRS 183
#define CRG2_ADRS 184
#define CRG1_VAL 185
#define CRG2_VAL 186
#define IDT_VAL 187

#define IDT_OFST 188
#define CRG1_OFST 189
#define CRG2_OFST 190
#define STRMEM_VAL 191

#define CAPET_NUMB 192
#define CAPET_FLOAT 193
#define CAPET_CHR 194
#define CAPET_STR 195
#define CAPET_IDT 196
#define CAPET_CRG1 197
#define CAPET_CRG2 198
#define CAPET_STRMEM 199
#define CHR_ADRS 200
#define STR_ADRS 201
#define CAPET_EXPR 202
#define DAXILET_IDT 203

#define FARGS_NUMB 204
#define FARGS_FLOAT 205
#define FARGS_CHR 206
#define FARGS_STR 207
#define FARGS_IDT_VAL 208
#define FARGS_CRG1_VAL 209
#define FARGS_CRG2_VAL 210
#define FARGS_STRMEM_VAL 211
#define FARGS_EXPR_VAL 212

#define FPARM_IDT_VAL 213
#define POP_OBSTK 214
#define GLBMT 215
#define FREE_SEC_DYN_STK 216

#define KVK 217
#define KVK1 218

#define  NOTYET   10001
#endif
