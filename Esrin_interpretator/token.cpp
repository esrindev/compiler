
#include "esrin.h"


void print_tok(int tok){

  switch(tok){

  case  PRG:
    printf("PRG");
    break;
  case  VAR:
    printf("VAR");
    break;
  case  TAM:
    printf("TAM");
    break;
  case  KESR:
    printf("KESR");
    break;
  case  SIMVOL:
    printf("SIMVOL");
    break;
  case  IDT:
    printf("IDT");
    break;
  case  VERGUL:
    printf("VERGUL");
    break;
  case  NOQTEV:
    printf("NOQTEV");
    break;
  case  KOD:
    printf("KOD");
    break;
  case  CAPET:
    printf("CAPET");
    break;
  case  OPNBRK:
    printf("OPNBRK");
    break;
  case  STR:
    printf("STR");
    break;
  case  CHR:
    printf("CHR");
    break;
  case  CLSBRK:
    printf("CLSBRK");
    break;
  case  SON:
    printf("SON");
    break;
  case  DOT:
    printf("DOT");
    break;
  case  LEXEM:
    printf("LEX");
    break;
  case  NUMB:
    printf("NUMB");
    break;
  case  SIGN:
    printf("SIGN");
    break;
  case  DIG:
    printf("DIG");
    break;
  case  POSDIG:
    printf("POSDIG" );
    break;
  case  POSNUMB:
    printf("POSNUMB");
    break;
  case  SNUMB:
    printf("SNUMB");
    break;
  case  POSSIG:
    printf("POSSIG");
    break;
  case  NEGSIG:
    printf("NEGSIG");
    break;
  case  SLC:
    printf("SLC");
    break;
  case  MLC:
    printf("MLC");
    break;
  case  SPS:
    printf("SPS");
    break;
  case  COLON:
    printf("COLON");
    break;
  case  ARYOPN:
    printf("ARYOPN");
    break;
  case  ARYCLS:
    printf("ARYCLS");
    break;
  case  ASGN:
    printf("ASGN");
    break;
  case  MULT:
    printf("MULT");
    break;
  case  DEL:
    printf("DEL");
    break;
  case  PRCT:
    printf("PRCT");
    break;
  case  XAR:
    printf("XAR");
    break;
  case  DAXIL:
    printf("DAXILET");
    break;
  case  EGER:
    printf("EGER");
    break;
  case  NIDA:
    printf("NIDA");
    break;
  case  GRT:
    printf("GRT");
    break;
  case  LES:
    printf("LES");
    break;
  case  VE:
    printf("VE");
    break;
  case  VEYA:
    printf("VEYA");
    break;
  case  FILESON:
    printf("FILESON");
    break;
  case  OPNBLK:
    printf("OPNBLK");
    break;
  case  CLSBLK:
    printf("CLSBLK");
    break;
  case  ONDA:
    printf("ONDA");
    break;
  case  YOXSA:
    printf("YOXSA");
    break;
  case  BERABER:
    printf("BERABER");
    break;
  case  FERQLI:
    printf("FERQLI");
    break;
  case  LESEQU:
    printf("LESEQU");
    break;
  case  GRTEQU:
    printf("GRTEQU");
    break;
  case  EMPTOK:
    printf("EMPTOK");
    break;
  case  SHERT:
    printf("SHERT");
    break;
  case  IFAD:
    printf("IFAD");
    break;

  case  ARTSIN:
    printf("ARTSIN");
    break;
  case  AZALSIN:
    printf("AZALSIN");
    break;
  case  ADDIM:
    printf("ADDIM");
    break;
  case  DOVR:
    printf("DOVR");
    break;
  case  KIMI:
    printf("KIMI");
    break;
  case  CRG1:
    printf("CRG1");
    break;
  case  CRG2:
    printf("CRG2");
    break;
  case  TEKRARET:
    printf("TEKRARET");
    break;
  case  DAN:
    printf("DAN");
    break;
  case  DEN:
    printf("DEN");
    break;

  case  CAPET_OP:
    printf("CAPET_OP");
    break;
  case  DOVR_OP:
    printf("DOVR_OP");
    break;
  case  BLOK:
    printf("BLOK");
    break;
  case  BIRTIP:
    printf("BIRTIP");
    break;
  case  TIPLER:
    printf("TIPLER");
    break;
  case  EGER_OP:
    printf("EGER_OP");
    break;
  case  ASGN_OP:
    printf("ASGN_OP");
    break;
  case  DAXIL_OP:
    printf("DAXIL_OP");
    break;
  case  BLOKSUZ:
    printf("BLOKSUZ");
    break;
  case  BLOKLU:
    printf("BLOKLU");
    break;
  case  KODDATALAR:
    printf("KODDATALAR");
    break;


  case  BLOKLUDATA:
    printf("BLOKLUDATA");
    break;

  case  YA:
    printf("YA");
    break;
  case  YE:
    printf("YE");
    break;
  case  AA:
    printf("A");
    break;
  case  EE:
    printf("E");
    break;
  case  DOVR_EXP:
    printf("DOVR_EXP");
    break;
  case  EGER_EXP:
    printf("EGER_EXP");
    break;

  case  DOVR_BLOKSUZ:
    printf("DOVR_BLOKSUZ");
    break;
  case  EGER_BLOKSUZ:
    printf("EGER_BLOKSUZ");
    break;
  case  YOXSA_BLOKSUZ:
    printf("YOXSA_BLOKSUZ");
    break;

  case  KODDATA:
    printf("KODDATA");
    break;

  case  DOVR_BLOKLU:
    printf("DOVR_BLOKLU");
    break;
  case  EGER_BLOKLU:
    printf("EGER_BLOKLU");
    break;
  case  YOXSA_BLOKLU:
    printf("YOXSA_BLOKLU");
    break;
  case  EGER_TEK_BLOKSUZ:
    printf("EGER_TEK_BLOKSUZ");
    break;
  case  EGER_TAM_BLOKSUZ:
    printf("EGER_TAM_BLOKSUZ");
    break;

  case  TAM1:
    printf("TAM1");
    break;
  case  TAM2:
    printf("TAM2");
    break;
  case  KESR1:
    printf("KESR1");
    break;
  case  KESR2:
    printf("KESR2");
    break;
  case  SIMVOL1:
    printf("SIMVOL1");
    break;
  case  SIMVOL2:
    printf("SIMVOL2");
    break;
 case  VAR1:
    printf("VAR1");
    break;
  case  VAR2:
    printf("VAR2");
    break;

 case  DAXILET1:
    printf("DAXILET1");
    break;
  case  DAXILET2:
    printf("DAXILET2");
    break;
 case  CAPET1:
    printf("CAPET1");
    break;
  case  CAPET2:
    printf("CAPET2");
    break;

 case  EGER_EXP1:
    printf("EGREXP1");
    break;
  case  EGER_EXP2:
    printf("EGREXP2");
    break;

  case  KOD1:
    printf("KOD1");
    break;

  case  PRG1:
    printf("PRG1");
    break;


  case  BADSTR:
    printf("BADSTR");
    break;

 case  BADCHR:
    printf("BADCHR");
    break;

  case  BADIDT:
    printf("BADIDT");
    break;

  case  DAXILET3:
    printf("DAXILET3");
    break;

  case  EMPTYOP:
    printf("EMTYOP");
    break;

  case  CAPET3:
    printf("CAPET3");
    break;

  case  EXPR:
    printf("EXPR");
    break;

  case  BRK:
    printf("BRK");
    break;


 case  FILEBEG:
    printf("FILEBEG");
    break;

 case  FLOAT_HALF:
    printf("FLOAT_HALF");
    break;


 case  FLOAT:
    printf("FLOAT");
    break;

 case  FUNKSIYA:
    printf("FUNKSIYA");
    break;


 case  FUNK_ELAN:
    printf("FUNK_ELAN");
    break;


 case  FUNK_BODY:
    printf("FUNK_BODY");
    break;


 case  EMPBRK:
    printf("EMPBRK");
    break;

 case  FE1:
    printf("FE1");
    break;

 case  FE2:
    printf("FE2");
    break;

 case  FUNK:
    printf("FUNK");
    break;

 case  FUNKS:
    printf("FUNKS");
    break;

 case  FCALL:
    printf("FCALL");
    break;

 case  FCALL1:
    printf("FCALL1");
    break;

 case  VDEC:
    printf("VDEC");
    break;


 case  DUMMY:
   printf(" ");  /* don't print dummy tokens, they play no role during parsing */
    break;

  case  FDECL:
    printf("FDECL");
    break;
	
  case  PRG2:
    printf("PRG2");
    break;

  case  QAYTAR:
    printf("QAYTAR");
    break;

 case  QAYTAR1:
    printf("QAYTAR1");
    break;

 case  NEQEDERKI:
    printf("NEQEDERKI");
    break;

 case  NEQEDERKI1:
    printf("NEQEDERKI1");
    break;

 case  NEQEDERKI2:
    printf("NEQEDERKI2");
    break;

 case  DAVAMET:
    printf("DAVAMET");
    break;

 case  DAYAN:
    printf("DAYAN");
    break;

 case  DAVAMET1:
    printf("DAVAMET1");
    break;

 case  DAYAN1:
    printf("DAYAN1");
    break;

 case  SEC:
    printf("SEC");
    break;

 case  SEC_OP:
    printf("SEC_OP");
    break;

 case  HAL:
    printf("HAL");
    break;

 case  HAL1:
    printf("HAL1");
    break;

 case  HAL2:
    printf("HAL2");
    break;

 case  HAL3:
    printf("HAL3");
    break;

 case  SEC1:
    printf("SEC1");
    break;

 case  SUSMA:
    printf("SUSMA");
    break;

case  START:
    printf("S");
    break;

case  STRUKT:
    printf("STRUKT");
    break;

case  STRUKT1:
    printf("STRUKT1");
    break;

case  STRUKT2:
    printf("STRUKT2");
    break;

case  STRUKT3:
    printf("STRUKT3");
    break;

case  YNT1:
    printf("YNT1");
    break;

case  YNT2:
    printf("YNT2");
    break;

case  STRMEM:
    printf("STRMEM");
    break;


case  YNTS:
    printf("YNTS");
    break;

case  SINIF:
    printf("SINIF");
    break;

case  QORUNAN:
    printf("QORUNAN");
    break;

case  SERBEST:
    printf("SERBEST");
    break;

case  MEXSUSI:
    printf("MEXSUSI");
    break;

case  SINIF_HEAD:
    printf("SINIF_HEAD");
    break;

case  SINIF_HEAD_START:
    printf("SINIF_HEAD_START");
    break;

case  SINIF_BODY:
    printf("SINIF_BODY");
    break;

case  SINIF_DECL:
    printf("SINIF_DECL");
    break;

case  SINIF_TYPE:
    printf("SINIF_TYPE");
    break;

case  YENI_TIP:
    printf("YENI_TIP");
    break;

case  SNMEM:
    printf("SNMEM");
    break;



case  INIT_DOVR:
    printf("INIT_DOVR");
    break;

case  CHECK_DOVR:
    printf("CHECK_DOVR");
    break;

case  STEP_DOVR:
    printf("STEP_DOVR");
    break;

case NEQ_TOP:
    printf("NEQ_TOP");
    break;

case SUS1:
    printf("SUS1");
    break;

case HAL1S:
    printf("HAL1S");
    break;

  case FARGS:
    printf("FARGS");
    break;

  case FPARM:
    printf("FPARM");
    break;

  case FCALL_BEG:
    printf("FCALL_BEG");
    break;


  case PUT_RET_ADDR:
    printf("PUT_RET_ADDR");
    break;


  case IDT_ADRS:
    printf("IDT_ADRS");
    break;

  case IDT_VAL:
    printf("IDT_VAL");
    break;

  case CRG1_ADRS:
    printf("CRG1_ARDS");
    break;

  case CRG2_ADRS:
    printf("CRG2_ADRS");
    break;

  case CRG1_VAL:
    printf("CRG1_VAL");
    break;

  case CRG2_VAL:
    printf("CRG2_VAL");
    break;

  case IDT_OFST:
    printf("IDT_OFST");
    break;

  case CRG1_OFST:
    printf("CRG1_OFST");
    break;

  case CRG2_OFST:
    printf("CRG2_OFST");
    break;

  case STRMEM_VAL:
    printf("STRMEM_VAL");
    break;

  case CAPET_NUMB:
    printf("CAPET_NUMB");
    break;
  case CAPET_FLOAT:
    printf("CAPET_FLOAT");
    break;
  case CAPET_CHR:
    printf("CAPET_CHR");
    break;
  case CAPET_STR:
    printf("CAPET_STR");
    break;
  case CAPET_IDT:
    printf("CAPET_IDT");
    break;
  case CAPET_CRG1:
    printf("CAPET_CRG1");
    break;
  case CAPET_CRG2:
    printf("CAPET_CRG2");
    break;
  case CAPET_STRMEM:
    printf("CAPET_STRMEM");
    break;
  case CHR_ADRS:
    printf("CHR_ADRS");
    break;
  case STR_ADRS:
    printf("STR_ADRS");
    break;

  case CAPET_EXPR:
    printf("CAPET_EXPR");
    break;

  case DAXILET_IDT:
    printf("DAXILET_IDT");
    break;

  case FARGS_NUMB:
    printf("FAGRS_NUMB");
    break;

  case FARGS_FLOAT:
    printf("FAGRS_FLOAT");
    break;

  case FARGS_CHR:
    printf("FAGRS_CHR");
    break;

  case FARGS_STR:
    printf("FAGRS_STR");
    break;

  case FARGS_IDT_VAL:
    printf("FAGRS_IDT_VAL");
    break;

  case FARGS_CRG1_VAL:
    printf("FAGRS_CRG1_VAL");
    break;

  case FARGS_CRG2_VAL:
    printf("FAGRS_CRG2_VAL");
    break;

  case FARGS_STRMEM_VAL:
    printf("FAGRS_STRMEM_VAL");
    break;

  case FARGS_EXPR_VAL:
    printf("FAGRS_EXPR_VAL");
    break;

  case FPARM_IDT_VAL:
    printf("FPARM_IDT_VAL");
    break;

  case POP_OBSTK:
    printf("POP_OBSTK");
    break;
    

case GLBMT:
    printf("GLBMT");
    break;


  case  NOTYET:
    printf("NOTYET");
    break;
  }
}


void print_tok_str(int tok){

  switch(tok){

  case  PRG:
    printf("PRG");
    break;
  case  VAR:
    printf("VAR");
    break;
  case  TAM:
    printf("TAM");
    break;
  case  KESR:
    printf("KESR");
    break;
  case  SIMVOL:
    printf("SIMVOL");
    break;
  case  IDT:
    printf("IDT");
    break;
  case  VERGUL:
    printf("VERGUL");
    break;
  case  NOQTEV:
    printf("`;`");
    break;
  case  KOD:
    printf("KOD");
    break;
  case  CAPET:
    printf("CAPET");
    break;
  case  OPNBRK:
    printf("OPNBRK");
    break;
  case  STR:
    printf("STR");
    break;
  case  CHR:
    printf("CHR");
    break;
  case  CLSBRK:
    printf("CLSBRK");
    break;
  case  SON:
    printf("SON");
    break;
  case  DOT:
    printf("DOT");
    break;
  case  LEXEM:
    printf("LEX");
    break;
  case  NUMB:
    printf("NUMB");
    break;
  case  SIGN:
    printf("SIGN");
    break;
  case  DIG:
    printf("DIG");
    break;
  case  POSDIG:
    printf("POSDIG" );
    break;
  case  POSNUMB:
    printf("POSNUMB");
    break;
  case  SNUMB:
    printf("SNUMB");
    break;
  case  POSSIG:
    printf("POSSIG");
    break;
  case  NEGSIG:
    printf("NEGSIG");
    break;
  case  SLC:
    printf("SLC");
    break;
  case  MLC:
    printf("MLC");
    break;
  case  SPS:
    printf("SPS");
    break;
  case  COLON:
    printf("COLON");
    break;
  case  ARYOPN:
    printf("ARYOPN");
    break;
  case  ARYCLS:
    printf("ARYCLS");
    break;
  case  ASGN:
    printf("ASGN");
    break;
  case  MULT:
    printf("MULT");
    break;
  case  DEL:
    printf("DEL");
    break;
  case  PRCT:
    printf("PRCT");
    break;
  case  XAR:
    printf("XAR");
    break;
  case  DAXIL:
    printf("DAXILET");
    break;
  case  EGER:
    printf("EGER");
    break;
  case  NIDA:
    printf("NIDA");
    break;
  case  GRT:
    printf("GRT");
    break;
  case  LES:
    printf("LES");
    break;
  case  VE:
    printf("VE");
    break;
  case  VEYA:
    printf("VEYA");
    break;
  case  FILESON:
    printf("FILESON");
    break;
  case  OPNBLK:
    printf("OPNBLK");
    break;
  case  CLSBLK:
    printf("CLSBLK");
    break;
  case  ONDA:
    printf("ONDA");
    break;
  case  YOXSA:
    printf("YOXSA");
    break;
  case  BERABER:
    printf("BERABER");
    break;
  case  FERQLI:
    printf("FERQLI");
    break;
  case  LESEQU:
    printf("LESEQU");
    break;
  case  GRTEQU:
    printf("GRTEQU");
    break;
  case  EMPTOK:
    printf("EMPTOK");
    break;
  case  SHERT:
    printf("SHERT");
    break;
  case  IFAD:
    printf("IFAD");
    break;
  case  ARTSIN:
    printf("ARTSIN");
    break;
  case  AZALSIN:
    printf("AZALSIN");
    break;
  case  ADDIM:
    printf("ADDIM");
    break;
  case  DOVR:
    printf("DOVR");
    break;
  case  KIMI:
    printf("KIMI");
    break;
  case  CRG1:
    printf("CRG1");
    break;
  case  CRG2:
    printf("CRG2");
    break;
  case  TEKRARET:
    printf("TEKRARET");
    break;
  case  DAN:
    printf("DAN");
    break;
  case  DEN:
    printf("DEN");
    break;

  case  CAPET_OP:
    printf("CAPET_OP");
    break;
  case  DOVR_OP:
    printf("DOVR_OP");
    break;
  case  BLOK:
    printf("BLOK");
    break;
  case  BIRTIP:
    printf("BIRTIP");
    break;
  case  TIPLER:
    printf("TIPLER");
    break;
  case  EGER_OP:
    printf("EGER_OP");
    break;
  case  ASGN_OP:
    printf("ASGN_OP");
    break;
  case  DAXIL_OP:
    printf("DAXIL_OP");
    break;
  case  BLOKSUZ:
    printf("BLOKSUZ");
    break;
  case  BLOKLU:
    printf("BLOKLU");
    break;
  case  KODDATALAR:
    printf("KODDATALAR");
    break;


  case  BLOKLUDATA:
    printf("BLOKLUDATA");
    break;

  case  YA:
    printf("YA");
    break;
  case  YE:
    printf("YE");
    break;
  case  AA:
    printf("A");
    break;
  case  EE:
    printf("E");
    break;
  case  DOVR_EXP:
    printf("DOVR_EXP");
    break;
  case  EGER_EXP:
    printf("EGER_EXP");
    break;

  case  DOVR_BLOKSUZ:
    printf("DOVR_BLOKSUZ");
    break;
  case  EGER_BLOKSUZ:
    printf("EGER_BLOKSUZ");
    break;
  case  YOXSA_BLOKSUZ:
    printf("YOXSA_BLOKSUZ");
    break;

  case  KODDATA:
    printf("KODDATA");
    break;

  case  DOVR_BLOKLU:
    printf("DOVR_BLOKLU");
    break;
  case  EGER_BLOKLU:
    printf("EGER_BLOKLU");
    break;
  case  YOXSA_BLOKLU:
    printf("YOXSA_BLOKLU");
    break;
  case  EGER_TEK_BLOKSUZ:
    printf("EGER_TEK_BLOKSUZ");
    break;
  case  EGER_TAM_BLOKSUZ:
    printf("EGER_TAM_BLOKSUZ");
    break;

  case  TAM1:
    printf("TAM1");
    break;
  case  TAM2:
    printf("TAM2");
    break;
  case  KESR1:
    printf("KESR1");
    break;
  case  KESR2:
    printf("KESR2");
    break;
  case  SIMVOL1:
    printf("SIMVOL1");
    break;
  case  SIMVOL2:
    printf("SIMVOL2");
    break;
 case  VAR1:
    printf("VAR1");
    break;
  case  VAR2:
    printf("VAR2");
    break;

 case  DAXILET1:
    printf("DAXILET1");
    break;
  case  DAXILET2:
    printf("DAXILET2");
    break;
 case  CAPET1:
    printf("CAPET1");
    break;
  case  CAPET2:
    printf("CAPET2");
    break;

 case  EGER_EXP1:
    printf("EGREXP1");
    break;
  case  EGER_EXP2:
    printf("EGREXP2");
    break;

  case  KOD1:
    printf("KOD1");
    break;

  case  PRG1:
    printf("PRG1");
    break;


  case  BADSTR:
    printf("BADSTR");
    break;

 case  BADCHR:
    printf("BADCHR");
    break;

  case  BADIDT:
    printf("BADIDT");
    break;

  case  DAXILET3:
    printf("DAXILET3");
    break;

  case  EMPTYOP:
    printf("EMTYOP");
    break;

  case  CAPET3:
    printf("CAPET3");
    break;

  case  EXPR:
    printf("ifade");
    break;

  case  BRK:
    printf("BRK");
    break;


 case  FILEBEG:
    printf("FILEBEG");
    break;

 case  FLOAT_HALF:
    printf("FLOAT_HALF");
    break;


 case  FLOAT:
    printf("FLOAT");
    break;

 case  FUNKSIYA:
    printf("FUNKSIYA");
    break;


 case  FUNK_ELAN:
    printf("FUNK_ELAN");
    break;


 case  FUNK_BODY:
    printf("FUNK_BODY");
    break;


 case  EMPBRK:
    printf("EMPBRK");
    break;

 case  FE1:
    printf("FE1");
    break;

 case  FE2:
    printf("FE2");
    break;

 case  FUNK:
    printf("FUNK");
    break;

 case  FUNKS:
    printf("FUNKS");
    break;

 case  FCALL:
    printf("FCALL");
    break;

 case  FCALL1:
    printf("FCALL1");
    break;

 case  VDEC:
    printf("VDEC");
    break;


 case  DUMMY:
   printf(" ");  /* don't print dummy tokens, they play no role during parsing */
    break;

  case  FDECL:
    printf("FDECL");
    break;
	
  case  PRG2:
    printf("PRG2");
    break;

  case  QAYTAR:
    printf("QAYTAR");
    break;

 case  QAYTAR1:
    printf("QAYTAR1");
    break;

 case  NEQEDERKI:
    printf("NEQEDERKI");
    break;

 case  NEQEDERKI1:
    printf("NEQEDERKI1");
    break;

 case  NEQEDERKI2:
    printf("NEQEDERKI2");
    break;

 case  DAVAMET:
    printf("DAVAMET");
    break;

 case  DAYAN:
    printf("DAYAN");
    break;

 case  DAVAMET1:
    printf("DAVAMET1");
    break;

 case  DAYAN1:
    printf("DAYAN1");
    break;

 case  SEC:
    printf("SEC");
    break;

 case  SEC_OP:
    printf("SEC_OP");
    break;

 case  HAL:
    printf("HAL");
    break;

 case  HAL1:
    printf("HAL1");
    break;

 case  HAL2:
    printf("HAL2");
    break;

 case  HAL3:
    printf("HAL3");
    break;

 case  SEC1:
    printf("SEC1");
    break;

 case  SUSMA:
    printf("SUSMA");
    break;

case  START:
    printf("S");
    break;

case  STRUKT:
    printf("STRUKT");
    break;

case  STRUKT1:
    printf("STRUKT1");
    break;

case  STRUKT2:
    printf("STRUKT2");
    break;

case  STRUKT3:
    printf("STRUKT3");
    break;

case  YNT1:
    printf("YNT1");
    break;

case  YNT2:
    printf("YNT2");
    break;

case  STRMEM:
    printf("STRMEM");
    break;


case  YNTS:
    printf("YNTS");
    break;

case  SINIF:
    printf("SINIF");
    break;

case  QORUNAN:
    printf("QORUNAN");
    break;

case  SERBEST:
    printf("SERBEST");
    break;

case  MEXSUSI:
    printf("MEXSUSI");
    break;

case  SINIF_HEAD:
    printf("SINIF_HEAD");
    break;

case  SINIF_HEAD_START:
    printf("SINIF_HEAD_START");
    break;

case  SINIF_BODY:
    printf("SINIF_BODY");
    break;

case  SINIF_DECL:
    printf("SINIF_DECL");
    break;

case  SINIF_TYPE:
    printf("SINIF_TYPE");
    break;

case  YENI_TIP:
    printf("YENI_TIP");
    break;

case  INIT_DOVR:
    printf("INIT_DOVR");
    break;

case  CHECK_DOVR:
    printf("CHECK_DOVR");
    break;

case  STEP_DOVR:
    printf("STEP_DOVR");
    break;

case NEQ_TOP:
    printf("NEQ_TOP");
    break;

case SUS1:
    printf("SUS1");
    break;

case HAL1S:
    printf("HAL1S");
    break;

case GLBMT:
    printf("GLBMT");
    break;

  case  NOTYET:
    printf("NOTYET");
    break;
  }
}
