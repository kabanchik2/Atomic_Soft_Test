#pragma once

#include "ColoredObject.h"
#include <iostream>

#include <thread>
#include <mutex>
#include <chrono>

#include <queue>

#include <condition_variable>
#include <atomic>

#include <fstream>

using namespace ColoredObjectStuff;

namespace AppStuff
{
	class App
	{
	public:
		App() :
			m_bFinished(false)
		{};
		~App() = default;

		void sequencesAutoGen();
		void sequencesGenFromConsole();
		void consumer();

		void sequenceOutput(const std::vector<ColoredObject>& vec, std::ofstream& fout, const bool toConsole = false);


		std::vector<ColoredObject> generateVector(const std::string& priorityStr, const bool outVecToConsole = false);
		void sortVector(std::vector<ColoredObject>& vec);

		std::string randomString(const size_t length);

	private:
		mutable std::mutex m_mutex;
		std::queue<std::vector<ColoredObject>> m_data;
		std::atomic_bool m_bFinished;
		std::condition_variable m_cv;
	};

	void appTest(const bool autoGen = false);
}
