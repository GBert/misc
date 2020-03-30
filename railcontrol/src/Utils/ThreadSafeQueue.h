/*
RailControl - Model Railway Control Software

Copyright (c) 2017-2020 Dominik (Teddy) Mahrer - www.railcontrol.org

RailControl is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3, or (at your option) any
later version.

RailControl is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with RailControl; see the file LICENCE. If not see
<http://www.gnu.org/licenses/>.
*/

#pragma once

#include <condition_variable>
#include <queue>
#include <mutex>

namespace Utils
{
	template<class T>
	class ThreadSafeQueue
	{
		public:
			ThreadSafeQueue(void)
			:	queue(),
				mutex(),
				run(true)
			{}

			~ThreadSafeQueue(void)
			{
				Terminate();
			}

			void Enqueue(T t)
			{
				std::unique_lock<std::mutex> lock(mutex);
				queue.push(t);
				cv.notify_all();
			}

			T Dequeue(void)
			{
				std::unique_lock<std::mutex> lock(mutex);
				while (true)
				{
					if (queue.empty() == false)
					{
						break;
					}
					if (run == false)
					{
						return T();
					}
					cv.wait_for(lock, std::chrono::seconds(1));
				}
				T val = queue.front();
				queue.pop();
				return val;
			}

			bool IsEmpty()
			{
				std::unique_lock<std::mutex> lock(mutex);
				return queue.empty();
			}

			void Terminate()
			{
				run = false;
				cv.notify_all();
			}

		private:
			std::queue<T> queue;
			mutable std::mutex mutex;
			std::condition_variable cv;
			volatile bool run;
	};
}
