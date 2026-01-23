#include "bqpch.h"




int main(int argc, char* argv[]) 
{
	try {
		return application::m_app->main(argc, argv);
	}
	catch (const char* msg) {
		bq::logger::critical(msg);
	}
	catch (std::exception const& e) {
		bq::logger::critical(e.what());
	}
	catch (...) {
		bq::logger::critical("unknown error thown");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}


application* application::m_app = nullptr;


int application::main(int argc, char* argv[])
{
	m_args.assign(argv, argv + argc);
	init();
	return execute();
}


application::application()
{
	if (m_app)
	{
		bq::logger::critical("no duplicate app instances!");
		std::terminate();
	}
	m_app = this;
}

