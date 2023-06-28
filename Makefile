.PHONY: all clean

all: nordump

nordump: src/*.c
	$(CC) -o $@ -Wall -Wextra -O3 -framework CoreFoundation -framework IOKit $^
	codesign -s - --entitlements ent.plist $@

clean:
	rm -f nordump
