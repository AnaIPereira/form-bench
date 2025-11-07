#-
#: LargeSize    4G
#: SmallSize    600M
#: TermsInSmall 20M
#: SortIOSize   10M
#: ScratchSize  4G
#: WorkSpace    100M
#: MaxTermSize  40K
#: Filepatches  512
*
#define NEWPLANE "1"
#define GAUGE "0"
#define SCHEME "0"
#define POW "7"
#define LONGINT "4"
#define TRIM "0"
#define CURRENT "POL"
#include mincer2c.h
* #include standard.h
propercount on;
#ifdef `CHILD'
#message GAUGE = `GAUGE', POW = `POW', CHILD = `CHILD';
#else
#message GAUGE = `GAUGE', POW = `POW';
#endif
*
*	Now some variables that are calcdia specific
*
S	sgn,n,[n-4],s,proexp,eq,[(D-3)/(D-2)];
AutoDeclare Symbol xx, SGN, sgn, z, x, k, y, xdia;
AutoDeclare CFunction DL;
AutoDeclare index ii;
CF	XX1, signs, del, fxn, qpow;
F	XX2;
V	Q,p;
S	fermi1,fermi2,fermi3,gluon1,gluon2,gluon3,ghost1,ghost2,ghost3;
F	fp,vqg,vqz,vqgz;
CF	vgh,Dg,Dgh,v3g,epexp;
CF	Dg,v2gp,v2gi,v2gc,v3g,v3gp,Ds,DL,v4g,V4G,v3gc,v3gi,v4g,v4gp,v4gc,
	v2gz,v3gz,v4gz,v2hz,v3hz;
I	K1,K2,al1,al2,be1,be2;
CF	withp;
*
CF	nofun,befun,lafun,fafun,<o1fun>,...,<o4fun>,<y1fun>,...,<y5fun>;
S	ssigny1,...,ssigny8;
S	j,n0,...,25,l1,...,l25;
.global
#ifdef `SORTREALLOCATE'
	On sortreallocate;
#endif
#ifdef `CHILD'
#include diagram`CHILD'.h
#else
#include diagram.h
#endif
		;
multiply ep^3*i_;
Multiply,1/2;
id	withp(x?) = 1;
id	signs(x?number_) = x;
.sort
*
*	Now we look for special cases that can be made faster
*
#call special(`TOPO')
#call rename(`TOPO')
id	sgn1 = sign_(`POW');
id	sgn3 = sign_(`POW');
id	mncsgn3 = sign_(`POW');
*
.sort:Special cases;
#call treatgzgz(`POW')
#call integral(`TOPO')
#if `GAUGE' == 0
id	mncxi = 0;
id  xi = 0;
id	mncxii = 1;
id	xii = 1;
#else
Multiply replace_(mncxi,xi,mncxii,xii);
#endif
.sort
#call trim(`TOPO')
repeat id xi*xii = -1+xii;
B	signs;
print;
.end
