#include "BsLog.h"
#include "BsException.h"

namespace BansheeEngine
{
	LogEntry::LogEntry(const String& msg, UINT32 channel)
		:mMsg(msg), mChannel(channel)
	{ }

	Log::Log()
	{
	}

	Log::~Log()
	{
		BS_LOCK_RECURSIVE_MUTEX(mMutex);

		for(auto iter = mEntries.begin(); iter != mEntries.end(); ++iter)
			bs_delete(*iter);
	}

	void Log::logMsg(const String& message, UINT32 channel)
	{
		BS_LOCK_RECURSIVE_MUTEX(mMutex);

		LogEntry* newEntry = bs_new<LogEntry>(message, channel);
		mEntries.push_back(newEntry);
		mUnreadEntries.push(newEntry);
	}

	void Log::clear()
	{
		BS_LOCK_RECURSIVE_MUTEX(mMutex);

		for(auto iter = mEntries.begin(); iter != mEntries.end(); ++iter)
			bs_delete(*iter);

		mEntries.clear();

		while (!mUnreadEntries.empty())
			mUnreadEntries.pop();
	}

	bool Log::getUnreadEntry(LogEntry& entry)
	{
		BS_LOCK_RECURSIVE_MUTEX(mMutex);

		if (mUnreadEntries.empty())
			return false;

		entry = *mUnreadEntries.front();
		mUnreadEntries.pop();

		return true;
	}
}