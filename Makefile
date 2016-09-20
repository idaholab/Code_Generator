# Modify this line to point to where XSD is installed
xsd_install := /home/ymj/tarballs/xsd-4.0.0-x86_64-linux-gnu

root := $(xsd_install)/examples

include $(root)/build/cxx/rules.make

XSD := $(shell if test -x $(xsd_install)/bin/xsd; \
               then echo $(xsd_install)/bin/xsd; \
               else echo xsd; fi)

override CPPFLAGS += $(shell if test -d $(xsd_install)/libxsd; \
                             then echo -I$(xsd_install)/libxsd; fi)

override LIBS := -lxerces-c $(LIBS)

# Rules.
#
.PRECIOUS: %.cxx %.hxx %.ixx

%.cxx %.hxx %.ixx: %.xsd
	$(XSD) cxx-tree $(XSDFLAGS) $<

CXX = g++
OBJS = kern/generator.o kern/node.o kern/network-hardware.o kern/hub.o kern/point-to-point.o kern/bridge.o kern/ap.o kern/application.o kern/ping.o kern/udp-echo.o kern/tcp-large-transfer.o kern/tap.o kern/emu.o kern/utils.o kern/udp.o kern/udp-cosem.o kern/data-concentrator.o kern/meter-data-management.o kern/demand-response.o

override XSDFLAGS += --generate-inline --generate-ostream --generate-serialization

# Build.
#
ns-3-codegenerator: ns-3-codegenerator.o TopologySchema.o $(OBJS)

TopologySchema.o: TopologySchema.cxx TopologySchema.hxx TopologySchema.ixx
ns-3-codegenerator.o: ns-3-codegenerator.cxx TopologySchema.hxx TopologySchema.ixx

TopologySchema.cxx TopologySchema.hxx TopologySchema.ixx: TopologySchema.xsd


# Build documentation
#
.PHONY: docs
docs:
	$(shell $(xsd_install)/bin/xsd cxx-tree --generate-serialization --generate-doxygen TopologySchema.xsd)
	$(shell doxygen -g TopologySchema.doxygen)
	$(shell doxygen TopologySchema.doxygen)


# Test
#
.PHONY: test
test: ns-3-codegenerator AMISimpleWireless_xml
	./ns-3-codegenerator AMISimpleWireless_xml AMISimpleWireless_cc.cc


# Clean.
#
.PHONY: clean
clean:
	rm -f TopologySchema.o TopologySchema.?xx ns-3-codegenerator.o ns-3-codegenerator AMISimpleWireless_cc.cc kern/*.o


# Clean documentation
#
.PHONY: cleandocs
cleandocs:
	rm -rf ./html ./latex TopologySchema.doxygen
