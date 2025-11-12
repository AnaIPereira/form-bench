* Test the sorting systems, by generating large numbers of terms which all
* cancel. Keep "distance" between the cancelling terms, such that they only
* cancel in later stages of the merging.

* With the default buffer sizes, FORM keeps 2M (~2^21) terms in the small buffer.
* We generate NTERMS*BLOWUP terms in total.

CFunction f,g,sum;
CFunction g1,...,g10;
Symbol n,x,y;

#ifndef `BLOWUP'
	#define BLOWUP "{2^6}"
#endif
#ifndef `NTERMS'
	#define NTERMS "{2^18}"
#endif
#ifndef `TERMPADDING'
	#define TERMPADDING "0"
#endif

#message sort-test.h: generating terms: {`BLOWUP'*`NTERMS'}

#define PARTS "{2^6}"
#define TERMSPERPART "{`NTERMS'/`PARTS'}"

Local test1 =
	#do p = 0,{`PARTS'-2}
		+ sum(n,{`p'*`TERMSPERPART'+1},{(`p'+1)*`TERMSPERPART'},f(n))
	#enddo
	+ sum(n,{(`PARTS'-1)*`TERMSPERPART'+1},`NTERMS',f(n));
.sort
Identify sum(?a) = sum_(?a);
.sort
#printtimes

#message sort-test.h: expand and cancel
Identify f(x?) = f(x)
	#do b = 1,{`BLOWUP'-1}
		- f(1+mod_(`NTERMS'/`b'-x,`NTERMS'))/{`BLOWUP'-1}
	#enddo
	;
* Make the terms bigger, to consume more disk space?
#if `TERMPADDING' > 0
	Identify f(x?) = f(x)*g((x+y)^`TERMPADDING');
#endif
.sort
#printtimes

#if `ZERO_test1' == 0
	#message sort-test.h: error: term cancellation problems
	Print +s test1;
	.sort
	#terminate
#endif

