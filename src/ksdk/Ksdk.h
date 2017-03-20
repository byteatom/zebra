#ifndef KSDK_H
#define KSDK_H

#if defined KSDK_LIB
#define KSDK_API __declspec(dllexport)
#else
#define KSDK_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "FadeStyle.h"

/**
 * 结果码
 */
enum KsdkResult {
	KSDK_OK = 0,
	KSDK_ERR_OPEN = 1,
	KSDK_ERR_PARA_SERIAL_PORT_NAME = 2,
	KSDK_ERR_CREATE_UDP = 3,
	KSDK_ERR_CREATE_SERIAL = 4,
	KSDK_ERR_CREATE_PROTOCOL = 5,
	KSDK_ERR_PARA_PROGRAM = 6,
	KSDK_ERR_PARA_PAGE = 7,
	KSDK_ERR_OPEN_TEMPLATE_FILE = 8,
	KSDK_ERR_READ_TEMPLATE_FILE = 9,
	KSDK_ERR_PARSE_TEMPLATE_FILE = 10,
	KSDK_ERR_PARA_PAGE_IMAGE_PATH = 11,
	KSDK_ERR_OPEN_JSON_FILE = 12,
	KSDK_ERR_PARSE_JSON_FILE = 13,
	KSDK_ERR_SEND_SCF = 14,
	KSDK_ERR_SEND_SEF = 15,
	KSDK_ERR_SEND_DCF = 16,
	KSDK_ERR_SYNC_TIME = 17,
	KSDK_ERR_SAVE_DISPLAY = 18,
	KSDK_ERR_CLOSE_SCREEN = 19,
	KSDK_ERR_CLEAR_CONTENT = 20,
	KSDK_ERR_SEND_CONTENT = 21,
	KSDK_ERR_SAVE_CONTENT = 22,
	KSDK_ERR_OPEN_SCREEN = 23,
	KSDK_ERR_PLAY = 24,
	KSDK_ERR_OBJ = 25,
	KSDK_ERR_SET_BRIGHTNESS = 26
};

/**
 * 一个页面
 */
struct KsdkPage {
	wchar_t* imagePath;		/**< 图片文件路径 */
	FadeStyle style;		/**< 出场效果，参见FadeStyle.h */
	unsigned int speed;	/**< 速度，0: 最慢, 20: 最快*/
	unsigned int stay; /**< 停留秒数 */
};

/**
 * 一个节目
 */
struct KsdkProgram {
	KsdkPage* pageArray;	/**< 页面数组 */
	unsigned int pageCount;	/**< 页面个数 */
};

/**
 * 结果回调函数的类型
 */
typedef void (*KsdkResultPf)(KsdkResult result /**< 结果码 */);

/**
 * 进度回调函数的类型
 */
typedef void (*KsdkProgressPf)(const unsigned int max,	/**< 进度最大值 */
								const unsigned int value	/**< 进度当前值 */);

/**
 * 发送节目数组
 */
KSDK_API void ksdkSend(const char* serialPortName,	/**< 串口名字，比如"COM1" */
						const KsdkProgram* programArray,	/**< 节目数组 */
						const unsigned int programCount,	/**< 节目个数 */
						KsdkProgressPf progressPf,	/**< 进度回调函数指针，如果不关心进度，可传入NULL */
						KsdkResultPf resultPf	/**< 本函数执行结果回调函数指针，如果不关心结果，可传入NULL */);

/**
 * 切换到index参数指定的节目
 */
KSDK_API void ksdkPlay(const char* serialPortName,	/**< 串口名字，比如"COM1" */
						const unsigned int index,	/**< 节目编号， 从0开始*/
						KsdkResultPf resultPf	/**< 本函数执行结果回调函数指针，如果不关心结果，可传入NULL */);

/**
 * 切换到index参数指定的节目
 */
KSDK_API void ksdkSetBrightness(const char* serialPortName,	/**< 串口名字，比如"COM1" */
						const unsigned int brightness,	/**< 亮度， 1-16*/
						KsdkResultPf resultPf	/**< 本函数执行结果回调函数指针，如果不关心结果，可传入NULL */);


#ifdef __cplusplus
}
#endif

#endif // KSDK_H
