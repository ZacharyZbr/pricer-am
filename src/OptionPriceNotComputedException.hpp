#include <iostream>

using namespace std;

class OptionPriceNotComputedException : public std::exception {
private:
	char* message;
public:
	OptionPriceNotComputedException(char* msg) :message(msg) {};
	char* what() {
		return "Sorry this pricer does not compute the price of this kind of option";
	}
};