#include <cstdio>
#include <string>

#include "puzbusv1.pb.h"

int main() {
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	puzbus::I2CMsg test_msg;

	test_msg.set_address(0x39);
	test_msg.set_data("Test message data!");

	std::string output;
	test_msg.SerializeToString(&output);

	printf("output[%lu]:\n", output.size());
	for (size_t i = 0; i < output.size(); i++) {
		printf("%02x ", output[i]);
	}
	printf("\n");

	return 0;
}

