#ifndef MESSAGE_H
#define MESSAGE_H

#include <QFile>
#include <QDate>
#include <QTime>

#include "IMsg.h"
#include "QStr.h"

class Msg : public IMsg
{
public:
	enum {
		LOGUE_SIZE = 3,
		ADDR_SIZE = 4,
		TYPE_SIZE = 4,
		SIZE_SIZE = 4,
		SUM_SIZE = 2,
		MSG_MAX_SIZE = 512
	};

#pragma pack(push, 1)
	struct Head
	{
		char		prologue[LOGUE_SIZE];
		char		dst[ADDR_SIZE];
		char		src[ADDR_SIZE];
		char		type[TYPE_SIZE];
		char		dataSize[SIZE_SIZE];
#pragma warning(push)
#pragma warning(disable : 4200)
		char		data[0];
#pragma warning(pop)
	};

	struct Tail
	{
		char		epilogue[LOGUE_SIZE];
		char		sum[SUM_SIZE];
	};
#pragma pack(pop)

	enum {
		MSG_MIN_SIZE = sizeof(Head) + sizeof(Tail),
		DATA_MAX_SIZE = MSG_MAX_SIZE - MSG_MIN_SIZE
	};

	explicit Msg(int size);	//allocate
	explicit Msg(const char *buf);	//parse
	explicit Msg(const char *buf, int size);	//allocate and copy
	explicit Msg(int type, const char *data, int dataSize);	//allocate and copy

	virtual ~Msg();

	char* buf() override;
	int size() override;
	char* data() override;
	int dataSize() override;
	virtual bool isRspFor(IMsgSp req) override;

	static const char PROLOGUE[LOGUE_SIZE];
	static const char EPILOGUE[LOGUE_SIZE];
	static const char HOST_ADDR[ADDR_SIZE];
	static const char CARD_ADDR_DEFAULT[ADDR_SIZE];
	static const char CARD_ADDR_BROADCAST[ADDR_SIZE];

	int type();
	Tail *tail();
	uint8_t sum();
	void setSum();
	bool valid();
	bool isRspOk();

protected:
	static char* ito4b10(char *str, int value);
	static char* ito2b16(char *str, int value);
	static int stoi(char *str, int len, int base);

	Head *head;
	bool alloc;
	int dataLen;
};

struct EmptyReqMsg : public Msg
{
	explicit EmptyReqMsg(int type);
};

#define FILE_MSG_NAME_SIZE 16

#pragma pack(push, 1)
struct FileMsgDataHead
{
	char		name[FILE_MSG_NAME_SIZE];
	int			offset;
#pragma warning(push)
#pragma warning(disable : 4200)
	char		data[0];
#pragma warning(pop)
};
#pragma pack(pop)

struct FileSetReqMsg : public Msg
{
	enum {
		FILE_DATA_MAX_SIZE = DATA_MAX_SIZE - sizeof(FileMsgDataHead)
	};
	FileSetReqMsg(int type, char* name, int offset, int fileDataSize);
	char* fileData();
	int fileDataSize;

	friend class FileMsgFactory;
};

class FileMsgFactory
{
public:
	static bool isScf(const QString& path);
	static bool isSef(const QString& path);
	static bool isDcf(const QString& path);
	static bool isContentFile(const QString& path);
	static bool isRcf(const QString& path);
	static bool isNcf(const QString& path);

	FileMsgFactory(const QString& path);

	IMsgSp next();

	QFile file;
	char name[FILE_MSG_NAME_SIZE];
	int msgType;
	qint64 size;
	bool eof;
};

struct WifiSetReqMsg : public Msg
{
	WifiSetReqMsg(char* ssid, char* pwd);
};

struct ScanDotReqMsg : public Msg
{
	ScanDotReqMsg(const char *send, int size);
};
struct ContentCtrlReqMsg : public Msg
{
	explicit ContentCtrlReqMsg(bool save/**< or clear*/);
};

struct DisplaySetReqMsg : public Msg
{
	explicit DisplaySetReqMsg();
	void setDate(QDate date);
	void setTime(QTime time);
	void setDateTimeNow();
	void setOpenClose(bool open/**< or close*/);
	void setOpenTime(bool open, QTime time);
	void setCloseTime(bool close, QTime time);
	void setBrightnessMode(bool manual/**< or auto*/);
	void setBrightnessLevel(int level);
};

struct RelativeTimeSetReqMsg : public Msg
{
	explicit RelativeTimeSetReqMsg();
	void setTime(QTime time);
};

struct DisplaySaveReqMsg : public Msg
{
	DisplaySaveReqMsg();
};

struct InfoGetReqMsg : public Msg
{
	InfoGetReqMsg();
};

struct InfoGetRspData
{
	InfoGetRspData(const QByteArray& data);

	QString model;
	QString product;
	QString version;
	QString totalSpace;
	QString freeSpace;
	QString powerVoltage;
	QString batteryVoltage;
	QString temperature;
	QString intfStats08;
	QString intfStats12;
	QString id;
	QString width;
	QString height;
	QString colorSpace;
};

struct ProgramPlayReqMsg : public Msg
{
	explicit ProgramPlayReqMsg(int id);
};

#endif // MESSAGE_H
