#-

* Run a sort which overflows the small buffer, and creates large patches. Don't
* overflow onto the disk, increase the default LargePatches for this.
#: LargePatches 1024
#: LargeSize 4G

#ifndef `COMPR'
	#define COMPR ""
#endif
`COMPR'

* 0.5B terms. 1B terms also runs in the large buffer with the above settings,
* but takes a bit long.
#define BLOWUP "{2^8}"
#define NTERMS "{2^21}"
#define TERMPADDING "0"

On stats;
On fewerstats,0;
On threadstats;

#include- sort-test.h

.end
