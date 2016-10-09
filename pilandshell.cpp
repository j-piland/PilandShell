#include<iostream>
#include<vector>
#include<string>
#include<chrono>
#include<algorithm>
#include<iterator>
#include<cstdlib>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

std::vector<char*> parseString(std::string toParse) {
	std::vector<char*> toReturn;

	int begin = 0;
	int wordBegin;

	while (toParse[begin] == ' ') {
		begin++;
	}

	wordBegin = begin;

	for (int c = begin; c < toParse.size()+1; c++) {
		
		if (toParse[c] == ' ' || c == toParse.size()) {
			int wordCopyIterator = 0;
			char* toPush = new char[128];
			while (wordBegin < c) {
				toPush[wordCopyIterator] = toParse[wordBegin];
				wordBegin++;
				wordCopyIterator++;
			}
			toPush[wordCopyIterator] = '\0';
			toReturn.push_back(toPush);

			while (toParse[c] == ' ' && c < toParse.size()) {
				c++;
			}
			wordBegin = c;
		}
	}

	toReturn.push_back(NULL);

	return toReturn;

}

void displayHistory(std::vector<std::string> history) {
	for (int c = 0; c < history.size(); c++) {
		std::cout << history.size() - c << ". " << history[c] << "\n";
	}
}

void parseTime(double time) {
	int min = static_cast<int>(time / (60  * 1000));
	time -= (min * 60 * 1000);
	int sec = static_cast<int>(time / (1000));
	time -= (sec * 1000);

	std::cout << "The previous process used " << min << " minutes, " << sec << " seconds, and " << time << " milliseconds.";
}

std::string selectHistory(std::vector<std::string> history, std::string cmd) {
	std::string cmdInt (20,'a');
	std::copy(cmd.begin() + 1, cmd.end(), cmdInt.begin());
	int historySelector = stoi(cmdInt);
	return history[history.size() - historySelector];
}

int main() {
	std::vector<std::string> cmdStore;
	bool exitbool = false;
	std::vector<char*> cmdWords;
	std::string cmd;
	double lastTime = 0;

	while (!exitbool) {
		std::cout << "[cmd]: ";
		std::getline(std::cin, cmd);

		cmdStore.push_back(cmd);
		
		if (cmd[0] == '^') {
			cmd = selectHistory(cmdStore, cmd);
		}
		if (cmd == "whte_rbt.obj") {
			std::cout << "Dennis Nedry pushed open the door marked FERTILIZATION, With the perimeter power out, all the security-card locks were disarmed. Every door in the building opened with a touch. The problems with the security system were high on Jurassic Park's bug list. Nedry wondered if anybody ever imagined that it wasn't a bug - tbat Nedry had programmed it that way.He had built in a classic trap door.Few programmers of large computer systems could resist the temptation to leave themselves a secret entrance.Partly it was common sense : if inept users locked up the system - and then called you for help - you always had a way to get in and repair the mess. And partly it was a kind of signature : Kilroy was here.";
		}
		else if (cmd == "ptime") {
			parseTime(lastTime);
			std::cout << "\n";
		}
		else if (cmd == "exit") {
			exitbool=true;
		}
		else if (cmd == "history") {
			displayHistory(cmdStore);
		}
		
		else {
			cmdWords = parseString(cmd);
				
			auto start = std::chrono::steady_clock::now();
			
			pid_t pid = fork();
			
			if(pid == 0){
				std::vector<char*> argv;
					
				for(std::vector<char*>::iterator cIt = cmdWords.begin(); cIt != cmdWords.end(); ++cIt){
					argv.push_back(&(*cIt)[0]);
				}

				execvp(cmdWords[0], &argv[0]);
				perror("error");
				exit(EXIT_FAILURE);

			}else if(pid>0){
				int *status;
				wait(status);
			}else{
				std::cout << "fork failure";
			}

			auto end = std::chrono::steady_clock::now();
			lastTime = std::chrono::duration <double, std::milli> (end-start).count();

		}

		
		std::cout << "\n";
	}
}
