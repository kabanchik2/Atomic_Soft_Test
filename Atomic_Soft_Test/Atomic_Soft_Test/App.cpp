#include "App.h"

using namespace AppStuff;


/*
just generate sequences and push them,
to stop - close the console =)
*/
void App::sequencesAutoGen()
{
	std::vector<ColoredObject> vec;
	std::string priorityStr;

	std::cout << "Input priority rule (like R<G<B): ";
	std::cin >> priorityStr;

	while (true)
	{
		//true for output to console
		vec = generateVector(priorityStr, false);

		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_data.push(vec);

			vec.clear();
		}

		m_cv.notify_one();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}


/*
Primitively generates sequences on command and pushes them
*/
void App::sequencesGenFromConsole()
{
	std::vector<ColoredObject> vec;
	std::string priorityStr = "R<G<B";
	std::string command;

	do
	{
		std::cout << "Input command setRule/start/exit: ";
		std::cin >> command;
		
		if (command == "start")
		{
			while (true)
			{
				std::cout << "Generate new sequence? command Y/setRule/stop/exit: ";
				std::cin >> command;

				if (command == "Y")
				{
					//true for output to console
					vec = generateVector(priorityStr, true);

					{
						std::unique_lock<std::mutex> lock(m_mutex);
						m_data.push(vec);
					}

					vec.clear();
					m_cv.notify_one();
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				}
				else if (command == "setRule")
				{
					std::cout << "Input priority rule (like R<G<B): ";
					std::cin >> priorityStr;
				}
				else if (command == "stop")
				{
					break;
				}
				else if (command == "exit")
				{
					std::unique_lock<std::mutex> lock(m_mutex);
					m_data.push(vec);
					m_cv.notify_one();
					break;
				}
				else
				{
					std::cout << "Wrong command" << std::endl;
				}
			}
		}
		else if (command == "setRule")
		{
			std::cout << "Input priority rule (like R<G<B): ";
			std::cin >> priorityStr;
		}
		else if (command == "exit")
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_data.push(vec);
			m_cv.notify_one();
			break;
			
		}
		else
		{
			std::cout << "Wrong command" << std::endl;
		}

	} while (command != "exit");
	

	m_bFinished = true;
}


/*
* Accepts sequences, sorts them and writes to file/console
*/
void App::consumer()
{
	std::vector<ColoredObject> tmpVec;
	std::ofstream fout;
	fout.open("sorted.txt");

	do
	{	
		std::unique_lock<std::mutex> lock(m_mutex);	

		while (m_data.empty() )
		{
			m_cv.wait(lock, [&]() { return !m_data.empty(); });
		}
		
		while (!m_data.empty())
		{
			tmpVec = m_data.front();
			sortVector(tmpVec);
			
			//true for output to console, false only in file
			sequenceOutput(tmpVec, fout, true);
			
			m_data.pop();
		}
		//std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	} while (!m_bFinished);
	
	fout.close();
}

void AppStuff::App::sequenceOutput(const std::vector<ColoredObject>& vec, std::ofstream& fout, const bool toConsole)
{
	if (toConsole)
	{
		if (fout.is_open())
		{
			for (auto i : vec)
			{
				std::cout << colorToString(i.getColor());
				fout << colorToString(i.getColor());
				
			}

			std::cout << std::endl;
			fout << std::endl;
		}
	}
	else
	{
		if (fout.is_open())
		{
			for (auto i : vec)
			{
				fout << colorToString(i.getColor());

			}
			fout << std::endl;
		}
	}
}


/*
Randomly generates a string, converts it into a vector
of colorful objects, and prioritizes all objects
*/
std::vector<ColoredObject> App::generateVector(const std::string& priorityStr, const bool outVecToConsole)
{
	srand((unsigned)time(0));
	std::string strOfObjects = randomString(30);

	std::vector<ColoredObject> vec;

	if (outVecToConsole)
		std::cout << "Sequence of objects: " << strOfObjects << std::endl;

	for (auto i : strOfObjects)
	{
		switch (i)
		{
		case 'R':
			vec.push_back(ColoredObject(ColoredObject::Red));
			break;
		case 'G':
			vec.push_back(ColoredObject(ColoredObject::Green));
			break;
		case 'B':
			vec.push_back(ColoredObject(ColoredObject::Blue));
			break;
		default:
			break;
		}
	}

	if (priorityStr[0] == 'R')
		if (priorityStr[2] == 'G')
			setPriorityForAll(vec, ColoredObject::Red, ColoredObject::Green, ColoredObject::Blue);
		else
			setPriorityForAll(vec, ColoredObject::Red, ColoredObject::Blue, ColoredObject::Green);
	else if (priorityStr[0] == 'G')
		if (priorityStr[2] == 'R')
			setPriorityForAll(vec, ColoredObject::Green, ColoredObject::Red, ColoredObject::Blue);
		else
			setPriorityForAll(vec, ColoredObject::Green, ColoredObject::Blue, ColoredObject::Green);
	else if (priorityStr[2] == 'R')
		setPriorityForAll(vec, ColoredObject::Blue, ColoredObject::Red, ColoredObject::Green);
	else
		setPriorityForAll(vec, ColoredObject::Blue, ColoredObject::Green, ColoredObject::Red);

	return vec;
}

void App::sortVector(std::vector<ColoredObject>& vec)
{

	std::sort(vec.begin(), vec.end(),
		[](const ColoredObject& lhs, const ColoredObject& rhs) {return lhs.getPriority() < rhs.getPriority(); });

}

std::string App::randomString(const size_t length)
{
	auto randChar = []() -> char
	{
		const char charSet[] = "RGB";
		const size_t max_index = (sizeof(charSet) - 1);
		return charSet[rand() % max_index];
	};

	std::string str(length, 0);
	std::generate_n(str.begin(), length, randChar);
	return str;
}

void AppStuff::appTest(const bool autoGen)
{
	App app;
	std::thread sortingThread(&App::consumer, &app);

	if (autoGen)
	{
		std::thread autoGeneratorThread(&App::sequencesAutoGen, &app);
		autoGeneratorThread.join();
	}
	else
	{
		std::thread consoleGeneratorThread(&App::sequencesGenFromConsole, &app);
		consoleGeneratorThread.join();
	}

	sortingThread.join();
	std::cout << "loop exited" << std::endl;
}