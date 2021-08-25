# Makefile to compile various libs from Michael Bernstein

build:
	mkdir -p lib
	mkdir -p ../lib
	mkdir -p include
	mkdir -p ../include
	cp inc/* include/
	cd bytestream && $(MAKE) && $(MAKE) install
	cd datastore/avl && $(MAKE) && $(MAKE) install
	cd datastore/map && $(MAKE) && $(MAKE) install
	cd datastore/baum && $(MAKE) && $(MAKE) install
	cd datastore/dliste && $(MAKE) && $(MAKE) install
	cd datastore/liste && $(MAKE) && $(MAKE) install
	cd datastore/menge && $(MAKE) && $(MAKE) install
	cd datastore/queue && $(MAKE) && $(MAKE) install
	cd datastore/stack && $(MAKE) && $(MAKE) install
	cd fsm && $(MAKE) && $(MAKE) install
	cd scanner && $(MAKE) && $(MAKE) install
	cd inipars && $(MAKE) && $(MAKE) install
	cp lib/* ../lib/
	cp include/* ../include/

clean:
	rm -rf include/*
	rm -rf lib/*
	cd bytestream && $(MAKE) clean
	cd datastore/avl && $(MAKE) clean
	cd datastore/map && $(MAKE) clean
	cd datastore/baum && $(MAKE) clean
	cd datastore/dliste && $(MAKE) clean
	cd datastore/liste && $(MAKE) clean
	cd datastore/menge && $(MAKE) clean
	cd datastore/queue && $(MAKE) clean
	cd datastore/stack && $(MAKE) clean
	cd fsm && $(MAKE) clean
	cd scanner && $(MAKE) clean
	cd inipars && $(MAKE) clean
