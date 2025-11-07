#procedure treatgzgz(POWER)
*
*   Treat routine for gravitonic currents.
*	We use five projections.
*	For unpolarized we have UNPOL1, UNPOL2, UNPOL3 for the A1, A2, A3 parts
*	For polarized we have POLD and POLF. The A4 and A6 parts are linear
*	combinations of these (see commentary there). We also have POL for A6.
*
*  #[ Physics :
*--#[ Unpolarized :
#if ( `CURRENT' == UNPOL1 )
*
*	Don't forget the external ghost when using d_(K1,K2).
*	This is to be found in the set hzhz
*
Multiply,-(d_(K1,K2)*(-P.Q^2)-P(K1)*P(K2)*Q.Q
			+P(K1)*Q(K2)*P.Q+P(K2)*Q(K1)*P.Q)/P.Q^2;
Multiply (
*
*   We take out a factor 1/D/(D-2)
*
**********************************************************
      +P(al1)*P(be1)*P(al2)*P(be2)*Q.Q^-2*acc(8960-6144*ep+1024*ep^2)
*      +d_(al1,al2)*P(be1)*P(be2)*P.Q^2*Q.Q^-3*acc(1280-512*ep)
*      +d_(al1,be2)*P(be1)*P(al2)*P.Q^2*Q.Q^-3*acc(1280-512*ep)
*      +d_(be1,al2)*P(al1)*P(be2)*P.Q^2*Q.Q^-3*acc(1280-512*ep)
      +4*d_(be1,be2)*P(al1)*P(al2)*P.Q^2*Q.Q^-3*acc(1280-512*ep)
      +2*d_(al1,al2)*d_(be1,be2)*P.Q^4*Q.Q^-4*acc(256)
*      +d_(al1,be2)*d_(be1,al2)*P.Q^4*Q.Q^-4*acc(256)
**********************************************************
	)/Q.Q;
repeat id D/[D-1] = 1+1/[D-1];
Multiply P.Q^2/Q.Q^2;
#define PROJ "6"
#elseif ( `CURRENT' == UNPOL2 )
*
*	Don't forget the external ghost when using d_(K1,K2).
*	This is to be found in the set hzhz
*
Multiply,-(d_(K1,K2)*(-P.Q^2)-P(K1)*P(K2)*Q.Q
			+P(K1)*Q(K2)*P.Q+P(K2)*Q(K1)*P.Q)/P.Q^2;
Multiply (
*
*   We take out a factor 1/D/(D-2)/(D-3)
*
**********************************************************
      +P(al1)*P(be1)*P(al2)*P(be2)*Q.Q^-2*acc(320-768*ep+256*ep^2)
*      +d_(al1,al2)*P(be1)*P(be2)*P.Q^2*Q.Q^-3*acc(128-224*ep+64*ep^2)
*      +d_(al1,be2)*P(be1)*P(al2)*P.Q^2*Q.Q^-3*acc(128-224*ep+64*ep^2)
*      +d_(be1,al2)*P(al1)*P(be2)*P.Q^2*Q.Q^-3*acc(128-224*ep+64*ep^2)
      +4*d_(be1,be2)*P(al1)*P(al2)*P.Q^2*Q.Q^-3*acc(128-224*ep+64*ep^2)
      +2*d_(al1,al2)*d_(be1,be2)*P.Q^4*Q.Q^-4*acc(64-64*ep)
*      +d_(al1,be2)*d_(be1,al2)*P.Q^4*Q.Q^-4*acc(64-64*ep)
**********************************************************
	)/Q.Q;
repeat id D/[D-1] = 1+1/[D-1];
Multiply P.Q^2/Q.Q^2;
#define PROJ "6"
#elseif ( `CURRENT' == UNPOL3 )
*
*	Don't forget the external ghost when using d_(K1,K2).
*	This is to be found in the set hzhz
*
Multiply,-(d_(K1,K2)*(-P.Q^2)-P(K1)*P(K2)*Q.Q
			+P(K1)*Q(K2)*P.Q+P(K2)*Q(K1)*P.Q)/P.Q^2;
Multiply (
*
*   We take out a factor 1/D/(D-2)/(D-3)
*
**********************************************************
      +P(al1)*P(be1)*P(al2)*P(be2)*Q.Q^-2*acc(16-32*ep)
*      +d_(al1,al2)*P(be1)*P(be2)*P.Q^2*Q.Q^-3*acc(16-16*ep)
*      +d_(al1,be2)*P(be1)*P(al2)*P.Q^2*Q.Q^-3*acc(16-16*ep)
*      +d_(be1,al2)*P(al1)*P(be2)*P.Q^2*Q.Q^-3*acc(16-16*ep)
      +4*d_(be1,be2)*P(al1)*P(al2)*P.Q^2*Q.Q^-3*acc(16-16*ep)
      +2*d_(al1,al2)*d_(be1,be2)*P.Q^4*Q.Q^-4*acc(16-16*ep)
*      +d_(al1,be2)*d_(be1,al2)*P.Q^4*Q.Q^-4*acc(16-16*ep)
**********************************************************
	)/Q.Q;
repeat id D/[D-1] = 1+1/[D-1];
Multiply P.Q^2/Q.Q^2;
#define PROJ "6"
*--#] Unpolarized : 
*--#[ Polarized :
#elseif ( `CURRENT' == POL )
*
*	We dropped a factor den(-2+D)*den(-3+D)*D^-1
*	This is the 6-current
*
Multiply (
         -d_(K1,be1)*d_(K2,be2)*d_(al1,al2)*P.Q^4*Q.Q^-4
         +d_(K1,be2)*d_(K2,be1)*d_(al1,al2)*P.Q^4*Q.Q^-4
         -d_(K1,be1)*d_(K2,be2)*P(al1)*P(al2)*P.Q^2*Q.Q^-3
         +d_(K1,be2)*d_(K2,be1)*P(al1)*P(al2)*P.Q^2*Q.Q^-3
         -d_(K1,be1)*d_(al1,al2)*(P(K2)-Q(K2)*P.Q/Q.Q)*P(be2)*P.Q^2*Q.Q^-3
         +d_(K1,be2)*d_(al1,al2)*(P(K2)-Q(K2)*P.Q/Q.Q)*P(be1)*P.Q^2*Q.Q^-3
         +d_(K2,be1)*d_(al1,al2)*(P(K1)-Q(K1)*P.Q/Q.Q)*P(be2)*P.Q^2*Q.Q^-3
         -d_(K2,be2)*d_(al1,al2)*(P(K1)-Q(K1)*P.Q/Q.Q)*P(be1)*P.Q^2*Q.Q^-3
         -d_(K1,be1)*(P(K2)-Q(K2)*P.Q/Q.Q)*P(al1)*P(al2)*P(be2)*Q.Q^-2
         +d_(K1,be2)*(P(K2)-Q(K2)*P.Q/Q.Q)*P(al1)*P(be1)*P(al2)*Q.Q^-2
         +d_(K2,be1)*(P(K1)-Q(K1)*P.Q/Q.Q)*P(al1)*P(al2)*P(be2)*Q.Q^-2
         -d_(K2,be2)*(P(K1)-Q(K1)*P.Q/Q.Q)*P(al1)*P(be1)*P(al2)*Q.Q^-2
		)*4/Q.Q;
#define PROJ "4"
*
#elseif ( `CURRENT' == POLD )
*
*	Proj4 = num(1+D)*den(-2+D)*den(-3+D)*D^-1*ProjD
*	       +         den(-2+D)*den(-3+D)*D^-1*ProjF
*	Proj6 = den(-2+D)*den(-3+D)*D^-1*ProjD
*	       +den(-2+D)*den(-3+D)*D^-1*ProjF
*	Note that the ProjD and ProjF projectors should give separate gauge
*	invariant results.
*
*id	ProjD =
Multiply (
         -d_(K1,be1)*d_(K2,be2)*P.Q^2*Q.Q^-2
         +d_(K1,be2)*d_(K2,be1)*P.Q^2*Q.Q^-2
         -d_(K1,be1)*(P(K2)-Q(K2)*P.Q/Q.Q)*P(be2)*Q.Q^-1
         +d_(K1,be2)*(P(K2)-Q(K2)*P.Q/Q.Q)*P(be1)*Q.Q^-1
         +d_(K2,be1)*(P(K1)-Q(K1)*P.Q/Q.Q)*P(be2)*Q.Q^-1
         -d_(K2,be2)*(P(K1)-Q(K1)*P.Q/Q.Q)*P(be1)*Q.Q^-1
		)*4*P(al1)*P(al2)/Q.Q^2;
#define PROJ "4"
#elseif ( `CURRENT' == POLF )
*
*	Proj4 = num(1+D)*den(-2+D)*den(-3+D)*D^-1*ProjD
*	       +         den(-2+D)*den(-3+D)*D^-1*ProjF
*	Proj6 = den(-2+D)*den(-3+D)*D^-1*ProjD
*	       +den(-2+D)*den(-3+D)*D^-1*ProjF
*	Note that the ProjD and ProjF projectors should give separate gauge
*	invariant results.
*
*id	ProjF =
Multiply (
         -d_(K1,be1)*d_(K2,be2)*P.Q^2*Q.Q^-2
         +d_(K1,be2)*d_(K2,be1)*P.Q^2*Q.Q^-2
         -d_(K1,be1)*(P(K2)-Q(K2)*P.Q/Q.Q)*P(be2)*Q.Q^-1
         +d_(K1,be2)*(P(K2)-Q(K2)*P.Q/Q.Q)*P(be1)*Q.Q^-1
         +d_(K2,be1)*(P(K1)-Q(K1)*P.Q/Q.Q)*P(be2)*Q.Q^-1
         -d_(K2,be2)*(P(K1)-Q(K1)*P.Q/Q.Q)*P(be1)*Q.Q^-1
		)*4*d_(al1,al2)*P.Q^2/Q.Q^3;
#define PROJ "4"
*--#] Polarized : 
#else
	#message Improper value for variable CURRENT: `CURRENT'
	#message Should be either UNPOL1, UNPOL2, UNPOL3, POL, POLD or POLF
	#terminate
#endif
*
.sort:Physics;
*
*  #] Physics : 
*  #[ feynman :
sum mu,mu1,nu,nu1,mu2,nu2,mu3,nu3,mu4,nu4,ro,ro1,si,si1,ka,ka1,la,la1;
sum mu5,nu5,mu6,nu6,mu7,nu7,mu8,nu8,mu9,nu9,mu10,nu10;
*
#call intpow(`TOPO')

id  fp(ii1?,p?) = i_*g_(ii1,p)/p.p;
id  vqg(ii1?,mu?) = i_*g_(ii1,mu);
id  vqz(ii1?,al1?,be1?,p1?,p2?) = -i_/8*(g_(ii1,al1)*(p1(be1)-p2(be1))
		+g_(ii1,be1)*(p1(al1)-p2(al1))
		-2*d_(al1,be1)*(g_(ii1,p1)-g_(ii1,p2)));
id  vqgz(ii1?,mu?,al1?,be1?) = i_/4*(
		+d_(al1,mu)*g_(ii1,be1)+d_(be1,mu)*g_(ii1,al1)-2*d_(al1,be1)*g_(ii1,mu));
id  fp(ii1?,p?,1) = i_*g_(ii1,p)/p.p*fermi1*epexp(p,1)*s/ep;
id  fp(ii1?,p?,2) = i_*g_(ii1,p)/p.p*fermi2*epexp(p,2)*s^2/ep^2;
id  fp(ii1?,p?,3) = i_*g_(ii1,p)/p.p*fermi3*epexp(p,3)*s^3/ep^3;
id  vgh(p?,mu?) = i_*p(mu);
id  Dg(ii1?,ii2?,p?,1) = DL(ii1,ii2,p)*gluon1*epexp(p,1)*s/ep;
id  Dg(ii1?,ii2?,p?,2) = DL(ii1,ii2,p)*gluon2*epexp(p,2)*s^2/ep^2;
id  Dg(ii1?,ii2?,p?,3) = DL(ii1,ii2,p)*gluon3*epexp(p,3)*s^3/ep^3;
id  Dgh(p?,1) = Dgh(p)*ghost1*epexp(p,1)*s/ep;
id  Dgh(p?,2) = Dgh(p)*ghost2*epexp(p,2)*s^2/ep^2;
id  Dgh(p?,3) = Dgh(p)*ghost3*epexp(p,3)*s^3/ep^3;
id  Dgh(p?) = i_/p.p;

if ( count(s,1) == 3 ) redefine TRIM "3";
if ( count(s,1) == 2 ) redefine TRIM "2";
if ( count(s,1) == 1 ) redefine TRIM "1";
id  s = 1;

id  epexp(p?mncpp18[D],x?) = mncee18[D]^x/mnceq^x;
id  epexp(-p?mncpp18[D],x?) = mncee18[D]^x/mnceq^x;
id  epexp(-p?[mncPp],x?) = epexp(p,x);
id  epexp(Q,x?) = 1;
id  epexp(-Q,x?) = 1;

id  DL(ii1?,ii2?,p?) = -i_*(d_(ii1,ii2)-p(ii1)*p(ii2)/p.p)/p.p;
.sort: loop propagators;
#call vertsubm
#ifdef `GAUGE'
#if ( `GAUGE' > 0 )
if ( count(fermi1,1,fermi2,1,fermi3,1,gluon1,1,gluon2,1,gluon3,1
    ,ghost1,1,ghost2,1,ghost3,1) > 0 );
    if ( count(xi,1) > `GAUGE' ) discard;
else;
    if ( count(xi,1) > `GAUGE' ) discard;
endif;  
#endif
#endif

#call ACCU(Vertex substitutions)

*#call reduce(reduce`TOPO')
id  g_(ii1?,P,Q,[P+Q]) = [P+Q].[P+Q]*g_(ii1,P);
id	g_(ii1?,[P+Q],Q,P) = [P+Q].[P+Q]*g_(ii1,P);
id  P.P = 0;

#call ACCU(Vertex identities)

tracen,3;
tracen,2;
tracen,1;
id  P.P = 0;
id  [(D-3)/(D-2)] = acc((1/2-ep)*(1+ep+ep^2+ep^3+ep^4+ep^5+ep^6));
id	1/[D-1] = acc(1/3*(1+2/3*ep+4/9*ep^2+8/27*ep^3+16/81*ep^4+32/243*ep^5+64/729*ep^6));
id D = acc(4-2*ep);
id [D-4] = acc(-2*ep);
id [1-ep/2] = acc(1-ep/2);

#call ACCU(Traces)
*  #] feynman : 
*  #[ Psub :
#do i = 1,8
id  [P+p`i'] = P+p`i';
id  [P-p`i'] = P-p`i';
#enddo
id  [P+Q] = P+Q;
id  [P-Q] = P-Q;
id  P.P = 0;
#call ACCU(moms0)
#call sym1(`TOPO')
id	sgn4^2 = 1;
#call ACCU(sym1)
#call momsubs(`TOPO')
#call ACCU(moms)
id  P.P = 0;
#call ACCU(moms)
#call scalars(`TOPO')
#call rewire(`TOPO')
* B <[P+p1].[P+p1]>,...,<[P+p8].[P+p8]>,<[P-p1].[P-p1]>,...,<[P-p8].[P-p8]>
*  ,<p1.P>,...,<p8.P>
* ;
* Print[] +f +s;
* .sort
* #switch `TOPO'
* #case o4
* 	#switch `SUBTOPO'
*  	#case 13
* 		#include o4v13rew.h
*     	#break
* 	#case Q5
* 	#case 56
* 		#include o4v56rew.h
*     	#break
* 	#case 57
* 		#include o4v57rew.h
*     	#break
* 	#endswitch
* #break
* #case y1
* 	#switch `SUBTOPO'
* 	#case 35
* 		#include y1v35rew.h
*     	#break
* 	#endswitch
* #break
* #endswitch
*
*  #include la78rew.h
B <[P+p1].[P+p1]>,...,<[P+p8].[P+p8]>,<[P-p1].[P-p1]>,...,<[P-p8].[P-p8]>
*  <p1.P>,...,<p8.P>
;
Print[] +f +s;
.sort
id  P = proexp*P;
if ( count(proexp,1) > {`POWER'+`PROJ'} ) discard;
id  epexp([P+Q],x?) = epexp( proexp*2*P.Q/Q.Q,x);
id  epexp([P-Q],x?) = epexp(-proexp*2*P.Q/Q.Q,x);
id  epexp(p?[mncpp18][D],x?)  = mncee18[D]^x/mnceq^x
    *epexp( 2*proexp*P.mncpp18[D]/mncpp18[D].mncpp18[D],x);
id  epexp(p?[-mncpp18][D],x?) = mncee18[D]^x/mnceq^x
    *epexp(-2*proexp*P.mncpp18[D]/mncpp18[D].mncpp18[D],x);
#do i = 1,8
if ( count([P+p`i'].[P+p`i'],1) < 0 );
    repeat;
      id,once,proexp^x?/[P+p`i'].[P+p`i'] = (
        sum_(k,0,{`POWER'+`PROJ'}-x,2^k*proexp^k*P.p`i'^k/p`i'.p`i'^k*sign_(k))
      )*proexp^x/p`i'.p`i';
    endrepeat;
endif;
if ( count([P-p`i'].[P-p`i'],1) < 0 );
    repeat;
      id,once,proexp^x?/[P-p`i'].[P-p`i'] = (
        sum_(k,0,{`POWER'+`PROJ'}-x,2^k*proexp^k*P.p`i'^k/p`i'.p`i'^k)
      )*proexp^x/p`i'.p`i';
    endrepeat;
endif;
#enddo
if ( count([P+Q].[P+Q],1) < 0 );
    repeat;
      id,once,proexp^x?/[P+Q].[P+Q] =
        (sum_(k,0,{`POWER'+`PROJ'}-x,2^k*proexp^k*P.Q^k/Q.Q^k*sign_(k)))*proexp^x/Q.Q;
    endrepeat;
endif;
if ( count([P-Q].[P-Q],1) < 0 );
	id	proexp^x?/[P-Q].[P-Q]^x1?pos_ =
        (sum_(k,0,{`POWER'+`PROJ'}-x,2^k*binom_(x1-1+k,k)
					*proexp^k*P.Q^k/Q.Q^k))*proexp^x/Q.Q^x1;
*    repeat;
*      id,once,proexp^x?/[P-Q].[P-Q] =
*        (sum_(k,0,{`POWER'+`PROJ'}-x,2^k*proexp^k*P.Q^k/Q.Q^k))*proexp^x/Q.Q;
*    endrepeat;
endif;

repeat;
    id,once,proexp^x?*epexp(y?,D?) =
    sump_(k,0,{`POWER'+`PROJ'}-x, y*acc((-D*ep-k+1)/k))*proexp^x;
    if ( count(P.P,1) > 0 ) discard;
    if ( count(proexp,1) > {`POWER'+`PROJ'} ) discard;
endrepeat;
if ( count(proexp,1) != {`POWER'+`PROJ'} ) discard;
id  proexp^{`POWER'+`PROJ'} = 1;
#call ACCU(Expand in P)
#call ObviouslyZero(`TOPO')
*
*   Now convert the metric:
*
id,many,P?.Q?^D?=sign_(D)*P.Q^D;
*
#call momsubs(`TOPO')
#call sym2(`TOPO')
if ( count(ep,1) );
	if ( count(acc,1) == 1 );
		id	ep^mncx?*acc(mncy?) = acc(ep^mncx*mncy);
	else;
		id	ep^mncx? = acc(ep^mncx);
	endif;
endif;
`HARMBRACKET'
.sort(PolyFun = acc):moms;
on,compress,gzip,6;
Multiply replace_(fermi1,mncfermi1,fermi2,mncfermi2,fermi3,mncfermi3,
	gluon1,mncgluon1,gluon2,mncgluon2,gluon3,mncgluon3,
	ghost1,mncghost1,ghost2,mncghost2);
#call harmo(P,Q,mncFFPP)
#switch `TOPO'
	#case la
	#case be
	#case no
	#case o1
	#case o2
	#case o3
	#break
	#default
		#call ACCU(Harmonics)
	#break
#endswitch
*  #] Psub :
#endprocedure
