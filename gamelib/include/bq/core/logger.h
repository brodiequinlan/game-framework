#pragma once
#undef PLATFORM_WINDOWS
#ifdef PLATFORM_WINDOWS
#include <Windows.h>
#endif

#include <fstream>
#include <iostream>

namespace bq {
	
	constexpr size_t WHITE = 7;
	constexpr size_t INFO_COLOR = 5;
	constexpr size_t WARN_COLOR = 14;
	constexpr size_t DEBUG_COLOR = 6;
	constexpr size_t CRITICAL_COLOR = 12;

	class logger {
		static std::string m_file;
		static bool m_file_logging;
	public:

		static void log_to_file(const std::string& file, bool clear_file) {
#ifdef PLATFORM_WINDOWS
			m_file = file;
			m_file_logging = true;
			if (clear_file) {
				std::ofstream ofs;
				
				ofs.open(file, std::ofstream::out, std::ofstream::trunc);
				ofs << "";
				ofs.close();
			}
#endif
		}
		static void debug(const std::string& message) {
#ifdef DEBUG
#ifdef PLATFORM_WINDOWS
			HANDLE  hConsole;
			hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, DEBUG_COLOR);
#endif
				std::cout << "DEBUG: " << message << std::endl;
				if (m_file_logging) {
					std::ofstream ofs;
					ofs.open(m_file, std::ofstream::out | std::ofstream::app);
					ofs << "DEBUG: " << message << std::endl;
					ofs.close();
				}
#ifdef PLATFORM_WINDOWS
			SetConsoleTextAttribute(hConsole, WHITE);
#endif
#endif
		}


		static void info(const std::string& message) {
#ifdef PLATFORM_WINDOWS
			HANDLE  hConsole;
			hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, INFO_COLOR);
#endif
			std::cout << "INFO: " << message << std::endl;
			if (m_file_logging) {
				std::ofstream ofs;
				ofs.open(m_file, std::ofstream::out | std::ofstream::app);
				ofs << "INFO: " << message << std::endl;
				ofs.close();
			}
#ifdef PLATFORM_WINDOWS
			SetConsoleTextAttribute(hConsole, WHITE);
#endif
			
		}
		static void warn(const std::string& message) {
#ifdef PLATFORM_WINDOWS
			HANDLE  hConsole;
			hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, WARN_COLOR);
#endif
			std::cout << "WARN: " << message << std::endl;
			if (m_file_logging) {
				std::ofstream ofs;
				ofs.open(m_file, std::ofstream::out | std::ofstream::app);
				ofs << "WARN: " << message << std::endl;
				ofs.close();
			}
#ifdef PLATFORM_WINDOWS
			SetConsoleTextAttribute(hConsole, WHITE);
#endif
			
		}
		static void critical(const std::string& message) {
#ifdef PLATFORM_WINDOWS
			HANDLE  hConsole;
			hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, CRITICAL_COLOR);
#endif
			std::cout << "CRITICAL: " << message << std::endl;
			if (m_file_logging) {
				std::ofstream ofs;
				ofs.open(m_file, std::ofstream::out | std::ofstream::app);
				ofs << "CRITICAL: " << message << std::endl;
				ofs.close();
			}
#ifdef PLATFORM_WINDOWS
			SetConsoleTextAttribute(hConsole, WHITE);
#endif
			
		}
	};
}