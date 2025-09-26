#include <iostream>
#include <string>
#include "App.hpp"
#include "MenuStrategies.hpp"

int main(){
	std::cout << "Huffman Codes\n";
	std::string inTXT = "./act2.2/files/inputText.txt";
	std::string outTXT = "./act2.2/files/outputText.txt";
	std::string inBIN = "./act2.2/files/inputBinary.bin";
	std::string outBIN = "./act2.2/files/outputBinary.bin";

	App app = App();
	app.setPathInTXT(inTXT);
	app.setPathOutTXT(outTXT);
	app.setPathInBIN(inBIN);
	app.setPathOutBIN(outBIN);

	app.addStrategy(new CreateTreeStrategy(&app));
	app.addStrategy(new EncodeDecodeInputStrategy(&app));
	app.addStrategy(new VisualizationStrategy(&app));

	app.run();

	return 0;
}