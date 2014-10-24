#include "psignal.h"
#include "log_manager.h"

#include <signal.h>

void PSignal::Regist()
{
	struct sigaction sig;
	sigemptyset(&sig.sa_mask);
	sig.sa_handler = SIG_IGN;
	sig.sa_flags = SA_ONESHOT;

	for (int i = 0; i < SIGRTMIN; ++i)
	{
		if ((i != SIGINT) ||
			(i != SIGTERM) ||
			(i != SIGSEGV) ||
			(i != SIGABRT) ||
			(i != SIGFPE))
		{
			sigaction(i, &sig, NULL);
		}
	}

	sig.sa_handler = &PSignal::SignalHandler;
	sigaction(SIGINT, &sig, NULL);
	sigaction(SIGTERM, &sig, NULL);
	sigaction(SIGSEGV, &sig, NULL);
	sigaction(SIGABRT, &sig, NULL);
	sigaction(SIGFPE, &sig, NULL);

	sig.sa_handler = &PSignal::SignalHandlerWithCore;
	sigaction(SIGRTMIN, &sig, NULL);
}

void PSignal::SignalHandler(int signal)
{
	LogManager::getInstance()->Log("signal catched:%d", signal);
}

void PSignal::SignalHandlerWithCore(int signal)
{
	LogManager::getInstance()->Err("signal catched:%d", signal);
	SignalHandler(signal);
	abort();
}
