#pragma once
#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <thread>

namespace Blanco {
	struct ProFileResult {
		std::string Name;
		long long Start, End;
		size_t ThreadID;
	};

	struct InstrumentSession
	{
		std::string Name;
	};

	class Instrumentor {
	public:
		Instrumentor():m_ProfileCount(0),m_CurrentSession(nullptr){};

		void BeginSession(const std::string& name, const std::string& filepath="result.json") {
			m_Outfile.open(filepath);
			WriteHeader();
			m_CurrentSession = new InstrumentSession{ name };
		}

		void EndSession() {
			WriteFooter();
			m_Outfile.close();
			delete m_CurrentSession;
			m_ProfileCount = 0;
			m_CurrentSession = nullptr;
		}

		void WriteHeader() {
			m_Outfile << "{\"traceEvents\":[";
			m_Outfile.flush();
		}

		void WriteProfile(const ProFileResult& result) {
			if (m_ProfileCount++ > 0)
				m_Outfile << ",";

			m_Outfile << "{";
			m_Outfile << "\"cat\":\"function\"";
			m_Outfile << ",\"dur\":" << result.End-result.Start ;
			m_Outfile << ",\"name\":\"" << result.Name << "\"";
			m_Outfile << ",\"ph\":\"X\"";
			m_Outfile << ",\"pid\":0";
			m_Outfile << ",\"tid\":" << result.ThreadID;
			m_Outfile << ",\"ts\":" << result.Start;
			m_Outfile << "}";

			m_Outfile.flush();
		}

		void WriteFooter() {
			m_Outfile << "]}";
			m_Outfile.flush();
		}

		static Instrumentor& Get() {
			static Instrumentor instance;
			return instance;
		}

	private:
		std::ofstream m_Outfile;
		int m_ProfileCount;
		InstrumentSession* m_CurrentSession;
	};

	class InstrumentationTimer {
	public:
		InstrumentationTimer(const char* name) :m_Name(name), m_Stopped(false) {
			m_Begin = std::chrono::steady_clock::now();
		}
		~InstrumentationTimer() {
			if (!m_Stopped)
				Stop();
		}
		void Stop() {
			auto timePoint = std::chrono::steady_clock::now();
			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_Begin).time_since_epoch().count();
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>(timePoint).time_since_epoch().count();
			size_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
			Instrumentor::Get().WriteProfile({m_Name,start,end,threadID});
		}
	private:
		const char* m_Name;
		bool m_Stopped;
		std::chrono::time_point<std::chrono::steady_clock> m_Begin;
	};
}

#define PROFILING 0
#if PROFILING 
#define CONNECT(x,y) x##y
#define BL_PROFILE_FUNCTION() ::Blanco::InstrumentationTimer CONNECT(timer,__LINE__)(__FUNCSIG__)
#define BL_PROFILE_SCOPE(name) ::Blanco::InstrumentationTimer timer##__LINE__(name)
#define BL_PROFILE_BEGIN_SESSION(name,filepath) ::Blanco::Instrumentor::Get().BeginSession(name,filepath)
#define BL_PROFILE_END_SESSION() ::Blanco::Instrumentor::Get().EndSession()
#else
#define BL_PROFILE_FUNCTION()
#define BL_PROFILE_SCOPE(name)
#define BL_PROFILE_BEGIN_SESSION(name,filepath)
#define BL_PROFILE_END_SESSION()
#endif