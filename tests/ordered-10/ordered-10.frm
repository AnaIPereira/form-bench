#-
#: TermsInSmall 3M
#: SmallSize 5G

Off stats;

* Make an expression of a certain size, and sort a bunch of times, pointlessly.
* We want to investigate how the parallel scaling depends on the expression size.

Symbol x,n;
CFunction sum;

#define NSORTS "1000"
#define NTERMS "10000"
#define PARTS "128"
#define TERMSPERPART "{`NTERMS'/`PARTS'}"

Local test1 =
	#do p = 0,{`PARTS'-2}
		+ sum(n,{`p'*`TERMSPERPART'+1},{(`p'+1)*`TERMSPERPART'},x^n)
	#enddo
	+ sum(n,{(`PARTS'-1)*`TERMSPERPART'+1},`NTERMS',x^n);
.sort
Identify sum(?a) = sum_(?a);
.sort

#do i = 1,`NSORTS'
	Identify x^n? = x^n;
	.sort:sort `i';
#enddo

On stats;
On fewerstats,0;
Off threadstats;
On humanstats;
.end
