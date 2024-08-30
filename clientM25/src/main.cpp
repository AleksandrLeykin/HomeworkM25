//#include "clientTest.h"

#include "myChat.h"
#include <vector>

int main() {
	setlocale(LC_ALL, "ru");
	std::cout << "\t\t\tConsole Chat!" << std::endl;
	
	std::unique_ptr<MyChat> user1 = std::make_unique<MyChat>();
	user1->interChat();

	

	return 0;
} 
