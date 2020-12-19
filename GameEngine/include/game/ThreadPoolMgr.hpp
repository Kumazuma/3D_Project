#pragma once
#include<memory>
#include<functional>
namespace Kumazuma
{
	namespace ThreadPool
	{
		class Task;
		class TaskContext;
		class Manager
		{
		protected:
			Manager() = default;
			Manager(Manager const&) = delete;
			virtual ~Manager() = default;
		public:
			static auto Instance()->std::shared_ptr<Manager>;
		public:
			virtual auto QueueTask(std::function<void(TaskContext&)> process)->std::shared_ptr<Task> = 0;
			virtual auto DispatchTask()->void = 0;
		};

		class TaskContext
		{
		public:
			auto Then(std::function<void()>  fn)->void;
			auto GetNext()->std::function<void()> const&;
		private:
			std::function<void()> process;
		};
		inline auto TaskContext::Then(std::function<void()> then) -> void
		{
			process = then;
		}
		inline auto TaskContext::GetNext() -> std::function<void()> const&
		{
			return process;
		}
		class Task
		{
		protected:
			Task() = default;
			Task(Task const&) = delete;
			virtual ~Task() = default;
		public:
			virtual auto Wait()->void = 0;
		};
	}
}