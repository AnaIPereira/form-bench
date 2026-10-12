#:MaxTermSize 4000000
#:WorkSpace 400000000
#:SubSmallSize 200000000
#:SubLargeSize 2000000000
#:SubTermsInSmall 4000000
#:SubFilePatches 256
#:LargeSize 4000000000
#:SmallSize 400000000
#:TermsInSmall 8000000
#define HYPMAXEXTRASYMBOLS "20000"
#include- hyperform.h
#redefine HYPuserParameters "t"
#redefine HYPnumberOfUserParameters "1"
on statistics;
#define HYPMAXEP "0"
#define IntegralExpr "F"
symbols x1,...,x12,t;
symbol ep;
symbols z2,z3,z5,z7,z9,z11,z13;
cfunctions num,den;
local F =
  + (-1/2/ep)*(x1*x4+x2*x4+x1+x2+x4)^(3+5*ep)*(x6*x9+x6*x10+x6*x11+x7*x9+x7*x10+x7*x11+x8*x9+x8*x10+x8*x11+x9*x10+x9*x11+x6+x7+x8+x9)^(3+5*ep)*(t*x1*x4*x6*x9+x1*x4*x6*x10*x11+x1*x4*x7*x8*x9+x1*x4*x7*x8*x10+x1*x4*x7*x8*x11+x1*x4*x7*x9*x11+x1*x4*x7*x10*x11+x1*x4*x8*x9*x10+x1*x4*x8*x10*x11+x1*x4*x9*x10*x11+x2*x3*x4*x6*x9+x2*x3*x4*x6*x10+x2*x3*x4*x6*x11+x2*x3*x4*x7*x9+x2*x3*x4*x7*x10+x2*x3*x4*x7*x11+x2*x3*x4*x8*x9+x2*x3*x4*x8*x10+x2*x3*x4*x8*x11+x2*x3*x4*x9*x10+x2*x3*x4*x9*x11+x2*x4*x5*x6*x9+x2*x4*x5*x6*x10+x2*x4*x5*x6*x11+x2*x4*x5*x7*x9+x2*x4*x5*x7*x10+x2*x4*x5*x7*x11+x2*x4*x5*x8*x9+x2*x4*x5*x8*x10+x2*x4*x5*x8*x11+x2*x4*x5*x9*x10+x2*x4*x5*x9*x11+x2*x4*x6*x8*x9+x2*x4*x6*x8*x10+x2*x4*x6*x8*x11+x2*x4*x6*x9*x11+x2*x4*x6*x10*x11+x2*x4*x7*x8*x9+x2*x4*x7*x8*x10+x2*x4*x7*x8*x11+x2*x4*x7*x9*x11+x2*x4*x7*x10*x11+x2*x4*x8*x9*x10+x2*x4*x8*x10*x11+x2*x4*x9*x10*x11+x1*x4*x7*x8+x1*x5*x6*x9+x1*x5*x6*x10+x1*x5*x6*x11+x1*x5*x7*x9+x1*x5*x7*x10+x1*x5*x7*x11+x1*x5*x8*x9+x1*x5*x8*x10+x1*x5*x8*x11+x1*x5*x9*x10+x1*x5*x9*x11+x1*x6*x8*x9+x1*x6*x8*x10+x1*x6*x8*x11+x1*x6*x9*x11+x1*x6*x10*x11+x1*x7*x8*x9+x1*x7*x8*x10+x1*x7*x8*x11+x1*x7*x9*x11+x1*x7*x10*x11+x1*x8*x9*x10+x1*x8*x10*x11+x1*x9*x10*x11+x2*x3*x4*x6+x2*x3*x4*x7+x2*x3*x4*x8+x2*x3*x4*x9+x2*x3*x6*x9+x2*x3*x6*x10+x2*x3*x6*x11+x2*x3*x7*x9+x2*x3*x7*x10+x2*x3*x7*x11+x2*x3*x8*x9+x2*x3*x8*x10+x2*x3*x8*x11+x2*x3*x9*x10+x2*x3*x9*x11+x2*x4*x5*x6+x2*x4*x5*x7+x2*x4*x5*x8+x2*x4*x5*x9+x2*x4*x6*x8+x2*x4*x7*x8+x2*x5*x6*x9+x2*x5*x6*x10+x2*x5*x6*x11+x2*x5*x7*x9+x2*x5*x7*x10+x2*x5*x7*x11+x2*x5*x8*x9+x2*x5*x8*x10+x2*x5*x8*x11+x2*x5*x9*x10+x2*x5*x9*x11+x2*x6*x8*x9+x2*x6*x8*x10+x2*x6*x8*x11+x2*x6*x9*x11+x2*x6*x10*x11+x2*x7*x8*x9+x2*x7*x8*x10+x2*x7*x8*x11+x2*x7*x9*x11+x2*x7*x10*x11+x2*x8*x9*x10+x2*x8*x10*x11+x2*x9*x10*x11+x3*x4*x6*x9+x3*x4*x6*x10+x3*x4*x6*x11+x3*x4*x7*x9+x3*x4*x7*x10+x3*x4*x7*x11+x3*x4*x8*x9+x3*x4*x8*x10+x3*x4*x8*x11+x3*x4*x9*x10+x3*x4*x9*x11+x4*x5*x6*x9+x4*x5*x6*x10+x4*x5*x6*x11+x4*x5*x7*x9+x4*x5*x7*x10+x4*x5*x7*x11+x4*x5*x8*x9+x4*x5*x8*x10+x4*x5*x8*x11+x4*x5*x9*x10+x4*x5*x9*x11+x4*x6*x8*x9+x4*x6*x8*x10+x4*x6*x8*x11+x4*x6*x9*x11+x4*x6*x10*x11+x4*x7*x8*x9+x4*x7*x8*x10+x4*x7*x8*x11+x4*x7*x9*x11+x4*x7*x10*x11+x4*x8*x9*x10+x4*x8*x10*x11+x4*x9*x10*x11+x1*x5*x6+x1*x5*x7+x1*x5*x8+x1*x5*x9+x1*x6*x8+x1*x7*x8+x2*x3*x6+x2*x3*x7+x2*x3*x8+x2*x3*x9+x2*x5*x6+x2*x5*x7+x2*x5*x8+x2*x5*x9+x2*x6*x8+x2*x7*x8+x3*x4*x6+x3*x4*x7+x3*x4*x8+x3*x4*x9+x4*x5*x6+x4*x5*x7+x4*x5*x8+x4*x5*x9+x4*x6*x8+x4*x7*x8)^(-5-4*ep)
;
.sort
#call HypParseInputExpr(ep,num,den,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11)
.sort
#call HypEpExpand
.sort
#call HypConvertToExtraSymbolForm(HYPz1,HYPz2,HYPz3,HYPz4,HYPz5,HYPz6,HYPz7,HYPz8,HYPz9,HYPz10,HYPz11)
.sort
#call HypIntegrateVar(HYPz1,`IntegralExpr')
.sort
#call HypIntegrateVar(HYPz2,`IntegralExpr')
.sort
#call HypIntegrateVar(HYPz3,`IntegralExpr')
.sort
#call HypIntegrateVar(HYPz4,`IntegralExpr')
.sort
#call HypIntegrateVar(HYPz5,`IntegralExpr')
.sort
#call HypIntegrateVar(HYPz6,`IntegralExpr')
.sort
#call HypIntegrateVar(HYPz7,`IntegralExpr')
.sort
#call HypIntegrateVar(HYPz8,`IntegralExpr')
.sort
#call HypIntegrateVar(HYPz9,`IntegralExpr')
.sort
#call HypIntegrateVar(HYPz10,`IntegralExpr')
.sort
#call HypIntegrateVar(HYPz11,`IntegralExpr')
.sort
#call HypFinalizeResult(ep,`HYPMAXEP')
.sort
FromPolynomial;
.sort
#do iX=1,`extrasymbols_'
  #$HYPdX = extrasymbol_(`iX');
  #$HYPdW = 0;
  #inside $HYPdX
    if (match(HYPregL?(?a)) > 0) $HYPdW = 1;
  #endinside
  #if `$HYPdW' == 0
  Argument;
    id Z`iX'_ = `$HYPdX';
  EndArgument;
  #endif
#enddo
.sort
#write <F.res> "L F = %E;", F
.end
