# scripter_parser
Check md5sum and content of configuration between two files

## Goals

Compare the given two files, one is made in hex-format, and the other one is modified config file.

## Key file part

The key file should be exactlly the same with config file at first.
It indicates some un-changable cmd and value.

To avoid modification, it will be encrypt into hex format and calculate a MD5SUM.

xxxx=yyyy <br>
xxxx(yyyy) <br>
xxxx.sh yyyy <br>
And also, there is '#' for comment, or the space may be presented.

For thus command that could be overwrite by config file will mark '*' in the frist position of each line
Otherwise, all variables could not be changed.

## Config file part

Same content with key file, but stored in ASCII format.
And also, it will record a MD5sum in any line of this file.

`##_<MD5SUM>`

The program will read this variable, and calculate 'key' file's md5sum.
This two values should be the same(w/o modification by user)

## Check content

Act like diff function.
The program will check the number of each command that appear in key file.
And also it's value for each command.

And then, do the comparison with config file, these command which mark '*' will be ignored.
Returns {1, 0, -1} if {pass, failed, error case}


