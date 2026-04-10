#-

#: ScratchSize 1G

Off stats;

#include- forcer.h
CF f,f1,f2,f3;
V p2,p3;
S x3;

* Give 1 or -1. n1 is not used.
Table randomsign(n1?);
Fill randomsign() = random_(2)*2-3;

* Zip two functions as:
*   zip(f1,f2(p1,...,pN),f3(q1,...,qN)) -> f1(p1,q1,...,pN,qN),
* for N >= 1.
Table zip(f1?(?a1),f2?(p2?,?a2),f3?(p3?,?a3));
Fill zip() =
  + thetap_(nargs_(?a2,?a3)) * zip(f1(?a1,p2,p3),f2(?a2),f3(?a3))
  + delta_(nargs_(?a2,?a3))  * f1(?a1,p2,p3)
;

* Element-wise multiplication as:
*   emul(f1,f2(p1,...,pN),f3(a1,...,aN)) -> f1(p1*a1,...,pN*aN)
* for N >= 1.
Table emul(f1?(?a1),f2?(p2?,?a2),f3?(x3?,?a3));
Fill emul() =
  + thetap_(nargs_(?a2,?a3)) * emul(f1(?a1,p2*x3),f2(?a2),f3(?a3))
  + delta_(nargs_(?a2,?a3))  * f1(?a1,p2*x3)
;

#ifndef `DIFFICULTY'
	#define DIFFICULTY "1"
#endif
#if `DIFFICULTY' == 1
	#define dot "6"
#elseif `DIFFICULTY' == 2
	#define dot "7"
#else
	#define dot "8"
#endif

L F1 =
  #do i=1,3
    + Zno`i'(1,1,1,1,1,1,1,1,1,1,1,0,0,0)
  #enddo
;

L F2 =
  #do i=1,3
    + Zno`i'(`dot',1,1,1,1,1,1,1,1,1,1,0,0,0)
    + Zno`i'(1,`dot',1,1,1,1,1,1,1,1,1,0,0,0)
    + Zno`i'(1,1,`dot',1,1,1,1,1,1,1,1,0,0,0)
    + Zno`i'(1,1,1,`dot',1,1,1,1,1,1,1,0,0,0)
    + Zno`i'(1,1,1,1,`dot',1,1,1,1,1,1,0,0,0)
    + Zno`i'(1,1,1,1,1,`dot',1,1,1,1,1,0,0,0)
    + Zno`i'(1,1,1,1,1,1,`dot',1,1,1,1,0,0,0)
    + Zno`i'(1,1,1,1,1,1,1,`dot',1,1,1,0,0,0)
    + Zno`i'(1,1,1,1,1,1,1,1,`dot',1,1,0,0,0)
    + Zno`i'(1,1,1,1,1,1,1,1,1,`dot',1,0,0,0)
    + Zno`i'(1,1,1,1,1,1,1,1,1,1,`dot',0,0,0)
    + Zno`i'(1,1,1,1,1,1,1,1,1,1,1,-`dot',0,0)
    + Zno`i'(1,1,1,1,1,1,1,1,1,1,1,0,-`dot',0)
    + Zno`i'(1,1,1,1,1,1,1,1,1,1,1,0,0,-`dot')
  #enddo
;

id Zno1(n1?,...,n14?) =
  +vx(-Q,p4,p5)
  *vx(p3,-p4,p10)
  *vx(p2,-p3,p9)
  *vx(p1,-p2,p11)
  *vx(-p5,p6,-p11)
  *vx(-p6,p7,-p10)
  *vx(-p7,p8,-p9)
  *vx(-p1,-p8,Q)
  /<p1.p1^n1>/.../<p11.p11^n11>
  /p2.p4^n12/Q.p2^n13/Q.p3^n14
;

id Zno2(n1?,...,n14?) =
  +vx(-Q,p4,p5)
  *vx(p3,-p4,p11)
  *vx(p6,p7,p10)
  *vx(p2,-p3,-p10)
  *vx(p1,-p2,p9)
  *vx(-p5,-p6,-p9)
  *vx(-p7,p8,-p11)
  *vx(-p1,-p8,Q)
  /<p1.p1^n1>/.../<p11.p11^n11>
  /Q.p2^n12/p1.p4^n13/Q.p3^n14
;

id Zno3(n1?,...,n14?) =
  +vx(-Q,p3,p4)
  *vx(p6,p8,p10)
  *vx(p5,-p10,p11)
  *vx(p1,-p3,-p5)
  *vx(-p4,-p8,p9)
  *vx(p7,-p9,-p11)
  *vx(p2,-p6,-p7)
  *vx(-p1,-p2,Q)
  /<p1.p1^n1>/.../<p11.p11^n11>
  /Q.p6^n12/Q.p8^n13/p3.p6^n14
;

* Make a random permutation of the loop momenta. The result should be the same.
multiply f1(p1,...,p11);
multiply ranperm_(f2,p1,...,p11);
multiply f3(<randomsign(1)>,...,<randomsign(11)>);
id f2(?a)*f3(?b) = emul(f2,f2(?a),f3(?b));
id f1(?a)*f2(?b) = zip(f1,f1(?a),f2(?b));
id f1(?a) = replace_(?a);

ModuleOption noparallel;
.sort:input;

* polyratfunexpand=12 is required for dot=5, and suffices for dot=7.
* TODO for now the dot=8 result was not compared with ep-exact FORCER.
#call Forcer(msbarexpand=4,polyratfunexpand=12)
.sort

Local diff1 = F1 - (
	+ ep^-1 * (  - 35/2*z5 )

	+ 21/2*z7 - 175/4*z6 + 105/4*z5 - 95/2*z3^2
	);

Local diff2 = F2 - (
	#if `dot' == 5
		+ ep^-4 * ( 167365/41472 )

		+ ep^-3 * ( 38025341/1244160 )

		+ ep^-2 * ( 1418597569/74649600 + 25/256*z3 )

		+ ep^-1 * (  - 83938280977/373248000 + 14907/32*z5 + 75/512*z4 -
		  322625375/248832*z3 )

		+ 5075581276567/1658880000 - 40481/128*z7 + 74535/64*z6 + 11410373/960*
		  z5 - 321955915/165888*z4 - 107825043623/3732480*z3 + 498253/160*z3^2
	#elseif `dot' == 6
		+ ep^-4 * ( 972391591/82944000 )

		+ ep^-3 * ( 29816188383/286720000 )

		+ ep^-2 * ( 297277743905521/1463132160000 + 913/512*z3 )

		+ ep^-1 * ( 8975485253207295457/12290310144000000 - 17220181/5376*z5 +
		  2739/1024*z4 + 4403771251/12441600*z3 )

		- 2959506156914525043127/1290482565120000000 - 299467/512*z7 - 86100905/
		  10752*z6 - 37620273443/752640*z5 + 45010104101/82944000*z4 +
		  62677739499043/1451520000*z3 - 365030027/26880*z3^2
	#elseif `dot' == 7
		+ ep^-4 * ( 1286882893/165888000 )

		+ ep^-3 * ( 5258354381401/69672960000 )

		+ ep^-2 * ( 87622079059577/464486400000 + 623/7680*z3 )

		+ ep^-1 * (  - 4691088409791115043/4096770048000000 + 2377171/512*z5 +
		  623/5120*z4 - 2411555298439/497664000*z3 )

		+ 41192426678428043326199/5161930260480000000 - 676851/1024*z7 +
		  11885855/1024*z6 + 195222162367/2257920*z5 - 2408981532653/331776000*
		  z4 - 29541943103360227/209018880000*z3 + 413182131/17920*z3^2
	#elseif `dot' == 8
		+ ep^-4 * ( 28766651329/1896652800 )

		+ ep^-3 * ( 44657054122121387/286773903360000 )

		+ ep^-2 * ( 28466251144180329341/60222519705600000 + 7721/7680*z3 )

		+ ep^-1 * ( 216788532618715902599327/50586916552704000000 - 57207929/
		  3584*z5 + 7721/5120*z4 + 450054702949361/56899584000*z3 )

		- 105452336325677781014407351/10623252476067840000000 - 564067/512*z7
		   - 286039645/7168*z6 - 2541756991511/9031680*z5 + 450630035975941/
		  37933056000*z4 + 6812849858815290439/21508042752000*z3 - 1280001601/
		  17920*z3^2
	#endif
	);

B ep;
P F1, F2;
.sort

#if `ZERO_diff1' != 1
	#message Error in results for F1
	Bracket ep;
	Print +s diff1;
	.sort
	#terminate
#endif
#if `ZERO_diff2' != 1
	#message Error in results for F2 dot `dot'
	Bracket ep;
	Print +s diff2;
	.sort
	#terminate
#endif

.end
