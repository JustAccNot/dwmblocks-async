#ifndef CONFIG_H
#define CONFIG_H

// String used to delimit block outputs in the status.
#define DELIMITER "^f15^"

// Maximum number of Unicode characters that a block can output.
#define MAX_BLOCK_OUTPUT_LENGTH 45

// Control whether blocks are clickable.
#define CLICKABLE_BLOCKS 0

// Control whether a leading delimiter should be prepended to the status.
#define LEADING_DELIMITER 0

// Control whether a trailing delimiter should be appended to the status.
#define TRAILING_DELIMITER 0

#define SCRIPTS(s) "~/.local/share/dwmblocks/scripts/" s
// Define blocks for the status feed as X(icon, cmd, interval, signal).
#define BLOCKS(X)             \
    X("",  SCRIPTS("bar-notfs"),  60,  9)  \
    X("",  SCRIPTS("bar-bt"),     10,  6)  \
    X("",  SCRIPTS("bar-net"),    10,  7)  \
    X(" ", SCRIPTS("bar-volume"), 0,   5)  \
    X(" ", SCRIPTS("bar-date"),   1,   0)
#endif
