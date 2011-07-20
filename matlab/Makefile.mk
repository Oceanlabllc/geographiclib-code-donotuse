# $Id: c2bd6a29f9a2244a74ab3f3b6ca8d96f37e05763 $

FUNCTIONS = utmupsforward utmupsreverse mgrsforward mgrsreverse \
	geodesicdirect geodesicinverse geodesicline \
	geoidheight geocentricforward geocentricreverse \
	localcartesianforward localcartesianreverse polygonarea

MATLAB_COMPILESCRIPT = geographiclibinterface.m

MATLABFILES = $(addsuffix .cpp,$(FUNCTIONS)) $(addsuffix .m,$(FUNCTIONS)) \
	 $(MATLAB_COMPILESCRIPT)

DEST = $(PREFIX)/libexec/GeographicLib/matlab
INSTALL = install -b

all:
	@:

install:
	test -d $(DEST) || mkdir -p $(DEST)
	$(INSTALL) -m 644 $(MATLABFILES) $(DEST)/

clean:
	rm -f *.mex* *.oct

list:
	@echo $(MATLABFILES)

.PHONY: all install list clean
