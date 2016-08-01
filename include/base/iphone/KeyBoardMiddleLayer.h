/*
 *  KeyBoardMiddleLayer.h
 *  BaiWan
 *
 *  Copyright 2010 JQBar. All rights reserved.
 *
 */

#ifndef _KEY_BOARD_MIDDLE_LAYER_H_
#define _KEY_BOARD_MIDDLE_LAYER_H_

class KeyBoardMiddleLayer
{
public:
	enum  {
		MAX_INPUT_EDIT = 65536,
	};
	static KeyBoardMiddleLayer* getInstance();
	static bool createInstance(void* jqbarController);
	static void destoryInstance();
	
	KeyBoardMiddleLayer(void* controller);
	~KeyBoardMiddleLayer();
	
	bool startInput(const unsigned short* curString, int max_len, unsigned int flag);
	void endInput(void* textField);
	
	
private:
	static KeyBoardMiddleLayer* _instance;

	void* m_mobileViewController;
	
	void* m_textViewController;

	unsigned short m_wbuffer[MAX_INPUT_EDIT];
};

#endif