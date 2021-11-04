#include "Crypto.h"
#include "DataStore/DataStore.h"
#include "UI/UI.h"

// std::vector<std::uint8_t> stringToVector(std::string_view string) {
//	return std::vector<std::uint8_t>(string.begin(), string.end());
// }

int main() {
	UI::App app;
	app.run();

	/*Crypto::init();*/

	/*std::string output;
	Crypto::Key* key = Crypto::getTOTPKey("GEZDGNBVGY3TQOJQGEZDGNBVGY3TQOJQ");
	Crypto::getTOTP(key, 20000000000, 30, 8, output);
	Crypto::deleteTOTPKey(key);
	std::cout << output << std::endl;*/

	/*Crypto::Key* key = Crypto::getKey("BallsBeItching");

	DataStore dataStore;
	dataStore.deserialize(key, "test.ds");

	dataStore.addElement("MyVerySecretPassword", stringToVector("IShatMyself553"));
	std::uint64_t id = dataStore.addElement("hjasjkd", {});
	dataStore.removeElement(id);

	dataStore.serialize(key, "test.ds");

	Crypto::deleteKey(key);*/
	/*Crypto::deinit();*/
}
