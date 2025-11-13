#-

* Run a sort which overflows the large buffer, creating disk patches. Pad the
* terms to increase the size, rather than generating even more small terms,
* which takes a bit too long.

#ifndef `COMPR'
	#define COMPR ""
#endif
`COMPR'

#define BLOWUP "{2^8}"
#define NTERMS "{2^17}"
#define TERMPADDING "15"

On stats;
On fewerstats,0;
On threadstats;

#include- ../sort-large/sort-test.h

.end
