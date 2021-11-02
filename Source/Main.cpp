#include "Crypto.h"
#include "DataStore/DataStore.h"

std::vector<std::uint8_t> stringToVector(std::string_view string) {
	return std::vector<std::uint8_t>(string.begin(), string.end());
}

int main() {
	Crypto::init();
	Crypto::Key* key = Crypto::getKey("BallsBeItching");

	DataStore dataStore;
	dataStore.deserialize(key, "test.ds");

	dataStore.addElement("MyVerySecretPassword", stringToVector("IShatMyself553"));
	std::uint64_t id = dataStore.addElement("hjasjkd", {});
	dataStore.removeElement(id);

	dataStore.serialize(key, "test.ds");

	Crypto::deleteKey(key);
	Crypto::deinit();
}