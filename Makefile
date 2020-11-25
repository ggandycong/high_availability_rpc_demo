
$(shell mkdir -p bin)

SUBDIRS := zk_handle_test register_test discovery_test server client

all: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS)

	
clean:
	rm -rf bin/*
	rm -rf common/*.o
	rm -rf zk_handle_test/*.o
	rm -rf register_test/*.o
	rm -rf discovery_test/*.o
	rm -rf server/*.o
	rm -rf client/*.o
	
