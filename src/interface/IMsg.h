#ifndef IMESSAGE_H
#define IMESSAGE_H

#include <memory>

class IMsg;

typedef std::shared_ptr<IMsg> IMsgSp;

class IMsg
{
public:
	virtual char* buf() = 0;
	virtual int size() = 0;
	virtual char* data() = 0;
	virtual int dataSize() = 0;
	virtual bool isRspFor(IMsgSp req) = 0;
	virtual ~IMsg() = default;
};

#endif // IMESSAGE_H
