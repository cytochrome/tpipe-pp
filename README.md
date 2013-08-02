tpipe++  ...  tpipe clone with Unbuffered features.
===


##Overview##

**tpipe** is very useful program created by Juanjo Garcia.

but when I use it as pipe duplicator with dzen2,
it doesn't output characters to pipe immediately,
because of buffered-io between stdin/out/pipe (probably).

tpipe++ fixed this issue, and written in c++ on the way :p



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

      $ echo "mew" | tpipe "cat" "cat" | cat
      mew
      mew
      mew
      mew
      $


