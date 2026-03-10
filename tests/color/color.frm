#-
#: ScratchSize 1G

Off Statistics;

#define SIZE "8"
* Extend the runtime without increasing SIZE:
#ifndef `DIFFICULTY'
	#define DIFFICULTY "1"
#endif
#if `DIFFICULTY' == 1
	#define LONGER "2"
#elseif `DIFFICULTY' == 2
	#define LONGER "4"
#else
	#define LONGER "8"
#endif
*#define ALTERNATEMETHOD "1"
*
*	Test program for the color traces in the paper and a few more.
*	The paper: "Color traces for arbitrary groups"
*	by T. van Ritbergen, A.N. Schellekens and J.A.M. Vermaseren.
*
*	The object is to express color traces in terms of group invariants
*	in such a way that the group has not been specified yet. The few
*	remaining invariants can be substituted afterwards. The expressions
*	in terms of invariants are better for publications. One sees more
*	about the structure of the problem this way.
*
*	All declarations are made in the file cfactors.h
*	One needs version 3 or later of FORM to run these programs.
*	The programs will handle all color structures with up to 14 vertices.
*	One vertex is one generator in any representation.
*	Currently the program handles only one type of non-adjoint representaion.
*	If there are two different representations of such type one has to
*	try to run the problem in stages.
*
*	Examples are:
*	One quark-loop with SIZE gluons in a maximally non-planar configuration
*		(TYPE = qloop)
*	Same but qluon loop (everything in the adjoint representation)
*		(TYPE = gloop)
*	Two quark loops with SIZE connecting gluons like a circular ladder.
*		(TYPE = qqloop)
*	Same with one quark loop and one gluon loop.
*		(TYPE = qgloop)
*	Same with two gluon loops.
*		(TYPE = ggloop)
*	The Coxeter graph (diagram with 14 vertices in the adjoint representation
*	in which there are no loops with fewer than 6 lines) (TYPE = g14)
*
*	One should not choose SIZE larger than 7 or most likely the simplification
*	routine cannot simplify the final result completely (in some rare cases
*	it can though for SIZE = 8).
*	For execution times, see the paper.
*	It does get very slow for quarks and SIZE > 7.
*
*	Program by J.Vermaseren, 24-may-1997
*
#include color.h
AutoDeclare Index i,j,k;
CFunction acc;
Symbol x,y;
.global
G	Q{2*`SIZE'} = <T(i1,i2,j1)>*...*<T(i`SIZE',i{`SIZE'+1},j`SIZE')>
	*<T(i{`SIZE'+1},i{`SIZE'+2},j1)>*...*<T(i{`SIZE'*2},i{2*`SIZE'+1},j`SIZE')>
	*replace_(i{`SIZE'*2+1},i1);

G	G{2*`SIZE'} = <f(i1,i2,j1)>*...*<f(i`SIZE',i{`SIZE'+1},j`SIZE')>
	*<f(i{`SIZE'+1},i{`SIZE'+2},j1)>*...*<f(i{`SIZE'*2},i{2*`SIZE'+1},j`SIZE')>
	*replace_(i{`SIZE'*2+1},i1);

G	QQ`SIZE' = <T(i1,i2,j1)>*...*<T(i`SIZE',i{`SIZE'+1},j`SIZE')>
	*<T(k1,k2,j1)>*...*<T(k`SIZE',k{`SIZE'+1},j`SIZE')>
	*replace_(i{`SIZE'+1},i1,k{`SIZE'+1},k1);

G	QG`SIZE' = <T(i1,i2,j1)>*...*<T(i`SIZE',i{`SIZE'+1},j`SIZE')>
	*<f(k1,k2,j1)>*...*<f(k`SIZE',k{`SIZE'+1},j`SIZE')>
	*replace_(i{`SIZE'+1},i1,k{`SIZE'+1},k1);

G	GG`SIZE' = <f(i1,i2,j1)>*...*<f(i`SIZE',i{`SIZE'+1},j`SIZE')>
	*<f(k1,k2,j1)>*...*<f(k`SIZE',k{`SIZE'+1},j`SIZE')>
	*replace_(i{`SIZE'+1},i1,k{`SIZE'+1},k1);

G	girth14 =
		f(i1,i2,i3)*f(i1,i4,i5)*f(i2,i6,i7)*f(i3,i8,i9)
		*f(i4,i10,i11)*f(i5,i12,i13)*f(i6,i14,i15)*f(i7,i16,i17)
		*f(i8,i18,i19)*f(i9,i20,i21)
		*f(i10,i21,i15)*f(i13,i19,i14)*f(i17,i11,i18)*f(i12,i16,i20);

* Takes too long
*G	girth24 =
*		f(i36,i1,i2)*f(i36,i3,i4)*f(i1,i5,i6)*f(i2,i7,i8)
*		*f(i3,i9,i10)*f(i4,i11,i12)*f(i5,i13,i14)*f(i6,i15,i16)
*		*f(i7,i17,i18)*f(i8,i19,i20)*f(i9,i21,i22)*f(i10,i23,i24)
*		*f(i11,i25,i26)*f(i12,i27,i28)*f(i13,i23,i29)*f(i14,i27,i30)
*		*f(i15,i25,i31)*f(i16,i21,i32)*f(i17,i26,i29)*f(i18,i32,i33)
*		*f(i19,i31,i34)*f(i20,i22,i30)*f(i24,i34,i35)*f(i28,i33,i35);

G	fiveq = T(i1,i2,j1)*T(i2,i3,j2)*T(i3,i1,j3)*
		T(i4,i5,j2)*T(i5,i6,j4)*T(i6,i4,j5)*
		T(i7,i8,j4)*T(i8,i9,j6)*T(i9,i7,j7)*
		T(i10,i11,j6)*T(i11,i12,j1)*T(i12,i10,j8)*
		T(i13,i14,j3)*T(i14,i15,j5)*T(i15,i16,j7)*T(i16,i13,j8);

Sum i1,...,i{`SIZE'*2},j1,...,j`SIZE',k1,...,k`SIZE';
Sum i{`SIZE'*2+1},...,i36,j{`SIZE'+1},...,j8;
Multiply <y^1>+...+<y^`LONGER'>;
.sort

#message call color
#call color
#call SORT(tloop-1)
#message call adjoint
#call adjoint
#call SORT(tloop-2)
#call simpli
id	acc(x?) = x;
id y^x? = 1/`LONGER';
.sort

*--#[ Check results
Local Q16 = -Q16
       + NA*I2R*cR^7
       - 14*NA*I2R*cA*cR^6
       + 161/2*NA*I2R*cA^2*cR^5
       - 2975/12*NA*I2R*cA^3*cR^4
       + 64085/144*NA*I2R*cA^4*cR^3
       - 67655/144*NA*I2R*cA^5*cR^2
       + 1413487/5184*NA*I2R*cA^6*cR
       - 14740249/217728*NA*I2R*cA^7
       + 70*d44(cOlpR1,cOlpA1)*cR^4
       - 546*d44(cOlpR1,cOlpA1)*cA*cR^3
       + 9191/6*d44(cOlpR1,cOlpA1)*cA^2*cR^2
       - 66199/36*d44(cOlpR1,cOlpA1)*cA^3*cR
       + 4463815489/5598720*d44(cOlpR1,cOlpA1)*cA^4
       + 18763481/3110400*d44(cOlpR1,cOlpA1)*d44(cOlpA2,cOlpA3)*NA^-1
       + 56/3*d44(cOlpA1,cOlpA2)*I2R*cR^3
       - 1813/15*d44(cOlpA1,cOlpA2)*I2R*cA*cR^2
       + 7637/30*d44(cOlpA1,cOlpA2)*I2R*cA^2*cR
       - 21073/120*d44(cOlpA1,cOlpA2)*I2R*cA^3
       - 224*d66(cOlpR1,cOlpA1)*cR^2
       + 2704/3*d66(cOlpR1,cOlpA1)*cA*cR
       - 7912/9*d66(cOlpR1,cOlpA1)*cA^2
       + 168*d444(cOlpR1,cOlpA1,cOlpA2)*cR^2
       - 3416/5*d444(cOlpR1,cOlpA1,cOlpA2)*cA*cR
       + 4380543991/6220800*d444(cOlpR1,cOlpA1,cOlpA2)*cA^2
       + 536/15*d444(cOlpA1,cOlpA2,cOlpA3)*I2R*cR
       - 125711/1575*d444(cOlpA1,cOlpA2,cOlpA3)*I2R*cA
       + 896/3*d644(cOlpR1,cOlpA1,cOlpA2)*cR
       - 9296/15*d644(cOlpR1,cOlpA1,cOlpA2)*cA
       - 2624/9*d644(cOlpA1,cOlpR1,cOlpA2)*cR
       + 3902758067/6531840*d644(cOlpA1,cOlpR1,cOlpA2)*cA
       - 4544/135*d644(cOlpA1,cOlpA2,cOlpA3)*I2R
       + 136*d88(cOlpR1,cOlpA1)
       + 35*d844(cOlpR1,cOlpA1,cOlpA2)
       - 31931/1440*d844(cOlpA1,cOlpR1,cOlpA2)
       - 1312/3*d664(cOlpR1,cOlpA1,cOlpA2)
       + 659/12*d664(cOlpA1,cOlpA2,cOlpR1)
       - 14215999/1036800*d4444a(cOlpR1,cOlpA1,cOlpA2,cOlpA3)
       + 213768787/1036800*d4444b(cOlpA1,cOlpA2,cOlpR1,cOlpA3)
      ;

Local G16 = -G16
       - 121/31104*NA*cA^8
       - 13/432*d44(cOlpA1,cOlpA2)*cA^4
       + 91/90*d444(cOlpA1,cOlpA2,cOlpA3)*cA^2
       - 16/27*d644(cOlpA1,cOlpA2,cOlpA3)*cA
       + d4444a(cOlpA1,cOlpA2,cOlpA3,cOlpA4)
      ;

Local QQ8 = -QQ8
       + 367/54432*NA*I2R^2*cA^6
       + 64899031/16588800*cOldddff(cOlpR1,cOlpR2,cOlpA1)*cA
       - 17/12*cOldff554(cOlpR1,cOlpR2,cOlpA1)
       - 159094348241/5016453120000*d33(cOlpR1,cOlpR2)*cA^5
       + 35065709/17915904000*d33(cOlpR1,cOlpR2)*d44(cOlpA1,cOlpA2)*NA^-1*cA
       + 9012683/18662400*d44(cOlpR1,cOlpR2)*cA^4
       + 318287/3110400*d44(cOlpR1,cOlpR2)*d44(cOlpA1,cOlpA2)*NA^-1
       - 7/27*d44(cOlpR1,cOlpA1)*I2R*cA^3
       - 431647/12441600*d44(cOlpR1,cOlpA1)*d44(cOlpR2,cOlpA2)*NA^-1
       + 11/360*d44(cOlpA1,cOlpA2)*I2R^2*cA^2
       - 359/432*d55(cOlpR1,cOlpR2)*cA^3
       + 49/24*d66(cOlpR1,cOlpR2)*cA^2
       + 4/9*d66(cOlpR1,cOlpA1)*I2R*cA
       + 20096267/41472000*d543(cOlpR1,cOlpA1,cOlpR2)*cA^2
       + 256036391/580608000*d543(cOlpR2,cOlpA1,cOlpR1)*cA^2
       - 1099101299/248832000*d444(cOlpR1,cOlpR2,cOlpA1)*cA^2
       - 7/30*d444(cOlpR1,cOlpA1,cOlpA2)*I2R*cA
       + 17/1575*d444(cOlpA1,cOlpA2,cOlpA3)*I2R^2
       - 7/3*d77(cOlpR1,cOlpR2)*cA
       + 4794133/4147200*d653(cOlpA1,cOlpR1,cOlpR2)*cA
       + 70310809/58060800*d653(cOlpA1,cOlpR2,cOlpR1)*cA
       - 49/36*d644(cOlpR1,cOlpR2,cOlpA1)*cA
       + 7/45*d644(cOlpR1,cOlpA1,cOlpA2)*I2R
       - 49/36*d644(cOlpR2,cOlpR1,cOlpA1)*cA
       + 7804501/1814400*d644(cOlpA1,cOlpR1,cOlpR2)*cA
       - 5/27*d644(cOlpA1,cOlpR1,cOlpA2)*I2R
       - 359/180*d554(cOlpR1,cOlpR2,cOlpA1)*cA
       - 19720033979/104509440000*d4433b(cOlpA1,cOlpA2,cOlpR1,cOlpR2)*cA
       + d88(cOlpR1,cOlpR2)
       + 35/96*d853(cOlpA1,cOlpR1,cOlpR2)
       + 13/1440*d853(cOlpA1,cOlpR2,cOlpR1)
       + 199/540*d844(cOlpA1,cOlpR1,cOlpR2)
       - 1/3*d763(cOlpR1,cOlpA1,cOlpR2)
       - 1/3*d763(cOlpR2,cOlpA1,cOlpR1)
       + 7/6*d754(cOlpR2,cOlpR1,cOlpA1)
       + 7/4*d664(cOlpR1,cOlpR2,cOlpA1)
       - 5/6*d664(cOlpR1,cOlpA1,cOlpR2)
       - 5/6*d664(cOlpR2,cOlpA1,cOlpR1)
       - 1/6*d655(cOlpA1,cOlpR1,cOlpR2)
       - 7437791/41472000*d6433b(cOlpA1,cOlpA2,cOlpR1,cOlpR2)
       - 12199/1382400*d6433c(cOlpA1,cOlpA2,cOlpR1,cOlpR2)
       - 119473/6220800*d6433c(cOlpA1,cOlpA2,cOlpR2,cOlpR1)
       + 1403/115200*d5443a(cOlpR1,cOlpA1,cOlpA2,cOlpR2)
       - 226219/4147200*d5443a(cOlpR2,cOlpA1,cOlpA2,cOlpR1)
       + 18757/57600*d5443c(cOlpR1,cOlpA1,cOlpA2,cOlpR2)
       + 951979/2073600*d5443c(cOlpR2,cOlpA1,cOlpA2,cOlpR1)
       - 253697/1658880*d4444a(cOlpR1,cOlpR2,cOlpA1,cOlpA2)
       + 1167989/2764800*d4444a(cOlpR1,cOlpA1,cOlpR2,cOlpA2)
       + 1498639/4147200*d4444b(cOlpR2,cOlpA1,cOlpR1,cOlpA2)
       - 76367/1036800*d4444b(cOlpA1,cOlpA2,cOlpR1,cOlpR2)
      ;

Local QG8 = -QG8
       + 353/54432*NA*I2R*cA^7
       + 4641769/44789760*d44(cOlpR1,cOlpA1)*cA^4
       + 2638717/12441600*d44(cOlpR1,cOlpA1)*d44(cOlpA2,cOlpA3)*NA^-1
       - 19/180*d44(cOlpA1,cOlpA2)*I2R*cA^3
       + 163/72*d66(cOlpR1,cOlpA1)*cA^2
       - 2085523/24883200*d444(cOlpR1,cOlpA1,cOlpA2)*cA^2
       + 52/1575*d444(cOlpA1,cOlpA2,cOlpA3)*I2R*cA
       - 77/60*d644(cOlpR1,cOlpA1,cOlpA2)*cA
       - 3874757/26127360*d644(cOlpA1,cOlpR1,cOlpA2)*cA
       - 2/135*d644(cOlpA1,cOlpA2,cOlpA3)*I2R
       + d88(cOlpR1,cOlpA1)
       + 199/540*d844(cOlpA1,cOlpR1,cOlpA2)
       + 11/12*d664(cOlpR1,cOlpA1,cOlpA2)
       - 5/6*d664(cOlpA1,cOlpA2,cOlpR1)
       + 2914957/4147200*d4444a(cOlpR1,cOlpA1,cOlpA2,cOlpA3)
       - 120809/829440*d4444b(cOlpA1,cOlpA2,cOlpR1,cOlpA3)
      ;

Local GG8 = -GG8
       + 61/15552*NA*cA^8
       - 13/432*d44(cOlpA1,cOlpA2)*cA^4
       + 91/90*d444(cOlpA1,cOlpA2,cOlpA3)*cA^2
       - 16/27*d644(cOlpA1,cOlpA2,cOlpA3)*cA
       + d4444a(cOlpA1,cOlpA2,cOlpA3,cOlpA4)
      ;

Local girth14 = -girth14
       + 1/648*NA*cA^7
       - 8/15*d444(cOlpA1,cOlpA2,cOlpA3)*cA
       + 16/9*d644(cOlpA1,cOlpA2,cOlpA3)
      ;

Local fiveq = -fiveq
       + 1/192*NA*I2R^5*cA^3
       + 1/4*d33(cOlpR1,cOlpR2)*I2R^3*cA^2
       + 5/48*d33(cOlpR1,cOlpR2)*d33(cOlpR3,cOlpR4)*NA^-1*I2R*cA
       + 5/48*d33(cOlpR1,cOlpR3)*d33(cOlpR2,cOlpR4)*NA^-1*I2R*cA
       + 1/8*d33(cOlpR1,cOlpR4)*d33(cOlpR2,cOlpR3)*NA^-1*I2R*cA
       + 1/16*d44(cOlpR1,cOlpA1)*I2R^4
       + 3/8*d433(cOlpR3,cOlpR1,cOlpR2)*I2R^2*cA
       + 1/2*d3333(cOlpR1,cOlpR2,cOlpR3,cOlpR4)*I2R*cA
       + d43333a(cOlpR5,cOlpR2,cOlpR1,cOlpR4,cOlpR3)
      ;
.sort

#do ex = {`activeexprnames_'}
	#if `ZERO_`ex'' == 0
		#message Error in `ex'
		#terminate
	#endif
#enddo
*--#]

.end
