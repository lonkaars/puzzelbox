PROTOCARGS += --cpp_out .

all: puzbusv1.pb.cc puzbusv1.pb.h

%.pb.cc %.pb.h &: %.proto
	protoc $(PROTOCARGS) $<

