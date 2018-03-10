#ifndef GUARD_COMMUNICATION
#define GUARD_COMMUNICATION

#include <ipc.h>

namespace msim{

	class Feed{
	public:
		Feed(){}
		virtual ~Feed(){}
	private:
		Feed(const Feed&);
		Feed& operator=(const Feed&);
	};

}

#endif // !GUARD_COMMUNICATION
