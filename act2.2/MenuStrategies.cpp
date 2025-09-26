#include <fstream>
#include <sstream>
#include <string>
#include <exception>
#include "MenuStrategies.hpp"
#include "App.hpp"
#include "MenuUtils.hpp"

IMenuStrategy::IMenuStrategy(App* app): app(app) {}

/*
Create Huffman Tree Menu (input):
- Create Tree from .txt File
- Create Tree from .bin File
- Create Tree from Terminal Input (plain text)
*/
CreateTreeFromTxtFileStrategy::CreateTreeFromTxtFileStrategy(App* app): IMenuStrategy(app) {}
std::string CreateTreeFromTxtFileStrategy::menuSelectionText() {
	return "Create Tree from Txt File";
}
void CreateTreeFromTxtFileStrategy::execute() {
	std::cout << "Creating Tree from Txt File: " << app->getPathInTXT() << "\n";
	std::ifstream inputFile(app->getPathInTXT());
	if (!inputFile.is_open()) {
        throw std::runtime_error("Error: Could not open file " + app->getPathInTXT());
    }
	std::stringstream buffer;
    buffer << inputFile.rdbuf();
    std::string text = buffer.str();
    inputFile.close();

	app->createTree(text);
	std::cout << "Huffman Tree Created succesfully!\n";
	waitForEnter();	
}

CreateTreeFromBinFileStrategy::CreateTreeFromBinFileStrategy(App* app): IMenuStrategy(app) {}
std::string CreateTreeFromBinFileStrategy::menuSelectionText() {
	return "Create Tree from Bin File";
}
void CreateTreeFromBinFileStrategy::execute() {
	std::cout << "Creating Tree from Bin File: " << app->getPathInBIN() << "\n";
	
	HuffmanTree* newTree = HuffmanTree::loadFromBin(app->getPathInBIN());
	
	if(newTree){
		app->setTree(newTree);
		std::cout << "Huffman Tree Created succesfully!\n";
	} else {
		std::cerr << "Error Creating Tree from: " << app->getPathInBIN() << "\n";
	}
	
	waitForEnter();
}

CreateTreeFromInputStrategy::CreateTreeFromInputStrategy(App* app): IMenuStrategy(app) {}
std::string CreateTreeFromInputStrategy::menuSelectionText() {
	return "Create Tree from Terminal Input";
}
void CreateTreeFromInputStrategy::execute() {
	std::cout << "Creating Tree from Terminal Input\n";
	std::string text;
	std::cin.sync();
    std::getline(std::cin, text);

	app->createTree(text);
	std::cout << "Huffman Tree Created succesfully!\n";
	waitForEnter();
}

CreateTreeStrategy::CreateTreeStrategy(App* app): IMenuStrategy(app) {
	strategies.push_back(new CreateTreeFromTxtFileStrategy(app));
	strategies.push_back(new CreateTreeFromBinFileStrategy(app));
	strategies.push_back(new CreateTreeFromInputStrategy(app));
}
CreateTreeStrategy::~CreateTreeStrategy(){
	for(IMenuStrategy* strategy : strategies) {
        delete strategy;
    }
    strategies.clear();
}
std::string CreateTreeStrategy::menuSelectionText() {
	return "Create Huffman Tree from File/Terminal Input";
}
void CreateTreeStrategy::execute() {
	runMenuFromStrategies(this->strategies);
}

/*
Encode Decode File/Terminal Input Menu (output):
- Encode File .txt to .bin (Destructive - Generates new Tree from scratch)
- Decode File .bin to .txt (Destructive - Generates new Tree from header :'v)
- Encode Terminal Input (shows in console - using current Tree)
- Decode Terminal Input (shows in console - using current Tree)
*/
EncodeFileStrategy::EncodeFileStrategy(App* app): IMenuStrategy(app) {}
std::string EncodeFileStrategy::menuSelectionText() {
	return "Encode File (txt -> bin)";
}
void EncodeFileStrategy::execute() {
	std::cout << "Encoding from " << app->getPathInTXT() << " to " << app->getPathOutBIN() << "\n";
	std::ifstream inputFile(app->getPathInTXT());
	if (!inputFile.is_open()) {
        throw std::runtime_error("Error: Could not open file " + app->getPathInTXT());
    }
	std::stringstream buffer;
    buffer << inputFile.rdbuf();
    std::string text = buffer.str();
    inputFile.close();

	app->createTree(text);
	std::cout << "Huffman Tree Created succesfully!\n";

	app->getCurrentTree()->saveToBin(app->getPathOutBIN());

	waitForEnter();
}

DecodeFileStrategy::DecodeFileStrategy(App* app): IMenuStrategy(app) {}
std::string DecodeFileStrategy::menuSelectionText() {
	return "Decode File (bin -> txt)";
}
void DecodeFileStrategy::execute() {
	std::cout << "Decoding from " << app->getPathInBIN() << " to " << app->getPathOutTXT() << "\n";
	
	HuffmanTree* newTree = HuffmanTree::loadFromBin(app->getPathInBIN());

	if(newTree){
		app->setTree(newTree);
		std::cout << "Huffman Tree loaded successfully!\n";
		
		std::string decodedText = app->getCurrentTree()->decode();
		
		std::ofstream outputFile(app->getPathOutTXT());
		if (!outputFile.is_open()) {
			std::cerr << "Error: Could not open output file " << app->getPathOutTXT() << "\n";
		} else {
			outputFile << decodedText;
			outputFile.close();
			std::cout << "Text decoded and saved to " << app->getPathOutTXT() << " successfully!\n";
		}
	} else {
		std::cerr << "Error loading Tree from: " << app->getPathInBIN() << "\n";
	}
	
	waitForEnter();
}

EncodeInputStrategy::EncodeInputStrategy(App* app): IMenuStrategy(app) {}
std::string EncodeInputStrategy::menuSelectionText() {
	return "Encode String from Terminal Input";
}
void EncodeInputStrategy::execute() {
	std::cout << "Encoding String from Terminal Input\n";
	std::string text;
	std::cin.sync();
    std::getline(std::cin, text);

	std::string result;

	if(app->getCurrentTree()){
		result = app->getCurrentTree()->encode(text);
		std::cout << result << "\n";
	} else {
		std::cout << "There is currently no HuffmanTree. Create one before encoding.\n";
	}
	waitForEnter();
}

DecodeInputStrategy::DecodeInputStrategy(App* app): IMenuStrategy(app) {}
std::string DecodeInputStrategy::menuSelectionText() {
	return "Decode String from Terminal Input";
}
void DecodeInputStrategy::execute() {
	std::cout << "Decoding String from Terminal Input\n";
	std::string text;
	std::cin.sync();
    std::getline(std::cin, text);

	std::string result;

	if(app->getCurrentTree()){
		result = app->getCurrentTree()->decode(text);
		std::cout << result << "\n";
	} else {
		std::cout << "There is currently no HuffmanTree. Create one before decoding.\n";
	}
	waitForEnter();
}

EncodeDecodeInputStrategy::EncodeDecodeInputStrategy(App* app): IMenuStrategy(app) {
	strategies.push_back(new EncodeFileStrategy(app));
	strategies.push_back(new DecodeFileStrategy(app));
	strategies.push_back(new EncodeInputStrategy(app));
	strategies.push_back(new DecodeInputStrategy(app));
}
EncodeDecodeInputStrategy::~EncodeDecodeInputStrategy(){
	for(IMenuStrategy* strategy : strategies) {
        delete strategy;
    }
    strategies.clear();
}
std::string EncodeDecodeInputStrategy::menuSelectionText() {
	return "Encode/Decode from File/Terminal Input";
}
void EncodeDecodeInputStrategy::execute() {
	runMenuFromStrategies(this->strategies);
}

/*
Visualization Menu:
- Print Table
- Print Current Huffman Tree
*/
PrintTableStrategy::PrintTableStrategy(App* app): IMenuStrategy(app) {}
std::string PrintTableStrategy::menuSelectionText() {
	return "Print Current List of Characters and Probabilities";
}
void PrintTableStrategy::execute() {
	if(app->getCurrentTree()){
		app->getCurrentTree()->printTable();
	} else {
		std::cout << "There is currently no HuffmanTree. Create one before printing.\n";
	}
	waitForEnter();
}

PrintTreeStrategy::PrintTreeStrategy(App* app): IMenuStrategy(app) {}
std::string PrintTreeStrategy::menuSelectionText() {
	return "Print Current Huffman Tree";
}
void PrintTreeStrategy::execute() {
	if(app->getCurrentTree()){
		app->getCurrentTree()->printTree();
	} else {
		std::cout << "There is currently no HuffmanTree. Create one before printing.\n";
	}
	waitForEnter();
}

VisualizationStrategy::VisualizationStrategy(App* app): IMenuStrategy(app) {
	strategies.push_back(new PrintTableStrategy(app));
	strategies.push_back(new PrintTreeStrategy(app));
}
VisualizationStrategy::~VisualizationStrategy(){
	for(IMenuStrategy* strategy : strategies) {
        delete strategy;
    }
    strategies.clear();
}
std::string VisualizationStrategy::menuSelectionText() {
	return "Visualization Submenu";
}
void VisualizationStrategy::execute() {
	runMenuFromStrategies(this->strategies);
}