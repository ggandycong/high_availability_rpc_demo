

# https://stackoverflow.com/questions/17834582/run-make-in-each-subdirectory

#SUBDIRS := $(wildcard */.)
SUBDIRS := zk_handle_test register_test discovery_test

all: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS)

	
clean:
	rm -rf bin/*
	rm -rf zk_handle_test/*.o
	rm -rf register_test/*.o
	rm -rf discovery_test/*.o
	rm -rf common/*.o
