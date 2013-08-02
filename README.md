tpipe++  ...  tpipe clone with Unbuffered features.
===

##Overview##

**tpipe++** is unbuffered version of **tpipe**
which duplicates stdin to command-lines which has passed.


###details...###
**tpipe**, which has created by Juanjo Garcia,
with basic features to duplicate pipes, is very useful program.

When I used tpipe with dzen2, the output characters wouldn't be shown immediately.
I thought it may be caused by buffered-io between stdin/out/pipe.

and I wrote unbuffered piping program, in c++ on the way :p
and the issue is disappeared.

-----
##Usage##

here is usage from (my) program help.

-----
    Usage: ./tpipe [options] command1 [command2] ...
    
    Options:
      -v [ --version ]      show version.
      -h [ --help ]         show this message.
      -b [ --buffer ]       stop disabling buffering.(doesnt means do buffering)
      -f [ --force ]        ignore error.

-----


##Examples##

      $ echo "mew" | tpipe "cat" "cat" "cat | cat" | cat
      mew
      mew
      mew
      mew
      $


