

# https://stackoverflow.com/questions/17834582/run-make-in-each-subdirectory

#SUBDIRS := $(wildcard */.)
SUBDIRS := zk_handle_test register_test

all: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS)

	
clean:
	rm -rf bin/*
	rm -rf zk_handle_test/*.o
	rm -rf register_test/*.o