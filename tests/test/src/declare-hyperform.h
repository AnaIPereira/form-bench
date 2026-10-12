#define HYPNPARAMS "0"
#define HYPMAXEP "0"
#define HYPMAXMATCH "20"
#define CURRENTINTVAR "0"
#define INITMZV "1"
#define MAXREGIT "10"
#define HYPMAXZ "5"
#ifndef `MAXMZVWORDLENGTH'
  #define MAXMZVWORDLENGTH "13"
#endif
#ifndef `HYPMAXEXTRASYMBOLS'
  #define HYPMAXEXTRASYMBOLS "1000"
#endif
#define HYPuseNormal "0"
#define HYPcheckForZeroCoefficients "1"
#define CheckPolysInSimplifyDiffLogDiffFuncs "1"
#define KeepLinfZeroInZeroRegularization  "0"
#define FibrationBasisZeroLimitBeforeDiff "1"
#define DoZeroLimit "1"
#define HYPIntegrationSequence ""
#define HYPactiveExpressions ""
#define HYPnumExtraSymbolLetters "0"
#define HYPvarDepVars ""
#define HYPmaxNumPower "1"
#define HYPmaxDenPower "1"
#define HYPDEBUG "0"
#define HYPtempPrefix "TMP"
#define HYPmaxBackSubstitutions "1000"
#define HYPextraSymTblFilled "0"
#define HYPuseParking "1"
#define HYPupperSpecName "HYPupperSpec"
#define HYPlowerSpecName "HYPlowerSpec"
#define HYPdoneSpecName "HYPdoneSpec"
#define HYPparkSpecName "HYPparkSpec"
#define HYPupperSpecFile "hyp-upper.spec"
#define HYPlowerSpecFile "hyp-lower.spec"
#define HYPdoneSpecFile "hyp-done.spec"
#define HYPparkSpecFile "hyp-park.spec"
#define HYPuserParameters ""
#define HYPnumberOfUserParameters "0"
#define HYPuseSubstitutionTables "1"
#define HYPtablesLoaded "0"
#define HYPregLtoMZVtable "HYPregLweightTbl"
#define HYPregLtoMZVtables "regl-tables.h"
#define HYPtempCount "0"
#define HYPzeroedOutTotal "0"
#define HYPdebug "0"
symbols HYPa0,...,HYPa`HYPMAXMATCH';
symbols HYPb0,...,HYPb`HYPMAXMATCH';
symbols HYPn1,...,HYPn`HYPMAXMATCH';
symbol HYPzero;
symbol HYPi;
cfunction HYPaux,HYPaux1,...,HYPaux9;
cfunction HYPtag;
cfunction HYPdrop;
cfunction HYPsep;
cfunctions HYPmatch,HYPmatch1,...,HYPmatch`HYPMAXMATCH';
cfunctions HYPdiv1,HYPdiv2,HYPdiv3,HYPdiv4,HYPgcd,HYPgcd1,HYPgcd2;
cfunction HYPauxSol;
cfunction HYPPhi;
cfunction HYPpow;
cfunction HYPpowExpand;
cfunction HYPacc;
symbol HYPep;
cfunction HYPe;
symbols HYPz1,...,HYPz99;
cfunctions HYPnum,HYPden;
cfunctions HYPnumep,HYPdenep;
cfunction HYPdenx;
cfunctions HYPpoly,HYPinvPoly;
cfunction HYPrat;
cfunction HYPcrat;
cfunction HYPratep;
cfunctions HYPsetJK,HYPsetJ,HYPsetK;
cfunctions HYPomega,HYPord,HYPdeg;
symbol HYPlam;
cfunctions HYPd,HYPdiff;
cfunction HYPtoDiff;
cfunction HYPlogDiff;
symbol HYPdiffMark,HYPdiffep;
cfunction HYPdx;
cfunctions HYPL,HYPw;
cfunction HYPlim;
cfunction HYPcoeff;
cfunction HYPlead;
cfunction HYPL0;
cfunction HYPinverse;
symbol HYPinf;
symbol HYPlog0;
cfunctions HYPlog,HYPlogx
cfunction HYPLx;
cfunction HYPLlimZero;
cfunction HYPtoLlimZero;
cfunction HYPLinfRegInfZero;
cfunction HYPtoLinfRegInfZero;
cfunction HYPregL;
cfunction HYPLone;
cfunction HYPH;
cfunction HYPLconst;
cfunction HYPLinf;
cfunction HYPLzero;
cfunction HYPLconstZeroLimit;
cfunction HYPLphi;
cfunction HYPLlimInf;
cfunction HYPcontainer;
cfunctions HYPweightLx,HYPweightRegL;
ctable sparse,HYPsimpTbl(1);
ctable sparse,HYPsolutionTbl(1);
ctable sparse,HYPsubstTbl(1);
ctable sparse,HYPzeroLimitSubstTbl(1);
ctable zerofill,HYPvarDepTbl(1:`HYPMAXEXTRASYMBOLS');
#Define MyRange "-1:0"
ctable zerofill, HYPregLweightTbl1(`MyRange');
#Do iWeight=2,`MAXMZVWORDLENGTH'
  #Redefine MyRange "`MyRange',-1:0"
  ctable zerofill, `HYPregLtoMZVtable'`iWeight'(`MyRange');
#EndDo
ctable sparse,HYPextraSymTbl(1);
ctable sparse,HYPnormalTbl(1);
ctable sparse,HYPzeroLimitHlogTbl(1);
ctable sparse,HYPregHyperlogTbl(1);
cfunction HYPSol,HYPsol;
symbol HYPEulerGamma;
cfunction HYPGfunc;
cfunction HYPinvGfunc;
cfunctions HYPfact,HYPwrap,HYPmark,HYPvars;
cfunction HYPindex;
cfunction HYPzeroLimitIndex;
cfunction HYPnumEval,HYPdenEval,HYPeval;
cfunctions HYPnumz,HYPdenz,HYPlogz;
symbol HYPdegree;
symbols HYPzeta2,HYPzeta3,HYPzeta5,HYPzeta7,HYPzeta9,HYPzeta11,HYPzeta13;
symbols HYPmzv5z3,HYPmzv7z3,HYPmzv5z3z3,HYPmzv9z3,HYPmzv6z4z1z1,HYPmzv7z3z3,HYPmzv5z5z3;
cfunction HYPGammaExpand, HYPinvGammaExpand;
cfunction HYPslot;
cfunction HYPwRegL;
cfunction HYPwRegL1,...,HYPwRegL`MAXMZVWORDLENGTH';
cfunction HYPsplit;
cfunction HYPminus;
cfunctions HYPcollect,HYPoverflow;
cfunctions HYPnumTerms;
set HYPparams:HYPz1,...,HYPz99;
set HYPratFunExceptions:HYPzeta2,HYPzeta3,HYPzeta5,HYPzeta7,HYPzeta9,HYPzeta11,HYPzeta13,
  HYPmzv5z3,HYPmzv7z3,HYPmzv5z3z3,HYPmzv9z3,HYPmzv6z4z1z1,HYPmzv7z3z3,HYPmzv5z5z3,
  HYPEulerGamma,
  HYPlog0;
