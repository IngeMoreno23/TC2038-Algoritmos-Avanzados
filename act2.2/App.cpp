#include "App.hpp"
#include "MenuStrategies.hpp"
#include "MenuUtils.hpp"

void App::createEmptyFileIfNotExist(const std::string& path, bool binary) {
	std::ifstream infile(path, binary ? std::ios::binary : std::ios::in);
	if (!infile.good()) {
		std::ofstream outfile(path, binary ? std::ios::binary : std::ios::out);
		if(outfile) {
			std::cout << "Created empty file: " << path << "\n";
		} else {
			std::cerr << "Failed to create file: " << path << "\n";
		}
	}
}

App::App(): inTXT(""), outTXT(""), inBIN(""), outBIN(""), currentTree(nullptr) {}

App::~App(){
	for(IMenuStrategy* strategy : strategies) {
        delete strategy;
    }
    strategies.clear();
	
	delete currentTree;
}

void App::setPathInTXT(const std::string& path){
	inTXT = path;
	createEmptyFileIfNotExist(inTXT, false);
}
void App::setPathOutTXT(const std::string& path){
	outTXT = path;
	createEmptyFileIfNotExist(outTXT, false);
}
void App::setPathInBIN(const std::string& path){
	inBIN = path;
	createEmptyFileIfNotExist(inBIN, true);
}
void App::setPathOutBIN(const std::string& path){
	outBIN = path;
	createEmptyFileIfNotExist(outBIN, true);
}
const std::string& App::getPathInTXT(){
	return inTXT;
}
const std::string& App::getPathOutTXT(){
	return outTXT;
}
const std::string& App::getPathInBIN(){
	return inBIN;
}
const std::string& App::getPathOutBIN(){
	return outBIN;
}

void App::createTree(std::string text){
	delete currentTree;
	currentTree = new HuffmanTree(text);
}

void App::setTree(HuffmanTree* tree){
	delete currentTree;
	currentTree = tree;
}
		
HuffmanTree* App::getCurrentTree(){
	return currentTree;
}

void App::run(){
	runMenuFromStrategies(this->strategies);
}

void App::addStrategy(IMenuStrategy* strategy){
	strategies.push_back(strategy);
}