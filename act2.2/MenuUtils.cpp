#include "MenuUtils.hpp"
#include "MenuStrategies.hpp"

void waitForEnter(){
    std::cin.sync();
    
    std::cout << "Press Enter to continue...";

    std::string dummy;
    std::getline(std::cin, dummy);
}

void runMenuFromStrategies(std::vector<IMenuStrategy*> strategies){
    bool running = true;
	while(running){
		int option;

		system("cls");
		std::cout << "Menu:\n";
		for(size_t i = 0; i < strategies.size(); i++){
			std::cout << i << ": " << strategies[i]->menuSelectionText() << "\n";
		}
		std::cout << "Enter option (or -1 to quit): ";

		if(!(std::cin >> option)){
			std::cin.clear();
			std::cout << "Invalid input. Please enter a number.\n";
			waitForEnter();
		} else if(option == -1){
			running = false;
		} else if(option >= 0 && option < (int)strategies.size()){
			strategies[option]->execute();
		} else {
			std::cout << "Invalid option. Please try again.\n";
			waitForEnter();
		}
	}
}