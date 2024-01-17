OS HW1

# The Version Updates

So, for the version definition, I'll like to define it as the followings:
1. v1.X : the single process testing
2. v2.X : the multiple process testing

## v1.0
Now, heres the current updates:

### finishes
- command line can be read.
- command splits successfully.
- finishes the `output >` redirection.
- finishes the `input <` command (but only system call requires).
- built-in command finishes : `cd`, `help`, `echo`, `exit`

### tend to be updated
- updating the history saving linked list for the `record` and `replay` command.
- test the history saving method for all the commands.
- figure out the `backend &` signal problem.
- finishing `mypid` command build.

## v1.1

### finishes
- built-in command finishes : `replay`, `record`

### tend to be updated
- change the `background &` trigger strategy.
- finishing `mypid` command build.
- multi-pipeline bug fixing.

## v2.0
So, as far as you can see, our muti-pipeline turns out to be a totally succession.

### finishes

- multi-pipeline implementation.
- change the `background &` trigger strategy.

### tend to be updated

- finishing `mypid` command build.
- doing some explanation adding and code modification.

## v2.1
So, as far as you can see, our muti-pipeline turns out to be a totally succession.

### finishes

- finishing `mypid` command build.

### tend to be updated

- None