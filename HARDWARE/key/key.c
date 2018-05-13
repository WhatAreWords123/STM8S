#include "key.h"
#include "delay.h"

uint8_t key_driver(void) 
{ 
  static uint8_t key_state = key_state_0, key_time = 0; 
  uint8_t  key_return = N_key; 
	uint8_t key_press;
	key_press = TEST1;                    // ������I/O��ƽ 
	switch (key_state) 
	{ 
		case key_state_0:                              // ������ʼ̬ 
			if (!key_press) key_state = key_state_1;      // �������£�״̬ת��������������ȷ��״̬ 
			break; 
		case key_state_1:                      // ����������ȷ��̬ 
			if (!key_press){
				key_time = 0;                   //  
				key_state = key_state_2;   // ������Ȼ���ڰ��£�������ɣ�״̬ת�������¼�ʱ��ļ�ʱ״̬�������صĻ����޼��¼� 
			}else 
				key_state = key_state_0;   // ������̧��ת����������ʼ̬���˴���ɺ�ʵ�������������ʵ�����İ��º��ͷŶ��ڴ������ġ� 
			break;
		case key_state_2: 
			if(key_press){ 
				key_return = S_key;        // ��ʱ�����ͷţ�˵���ǲ���һ�ζ̲���������S_key 
				key_state = key_state_0;   // ת����������ʼ̬ 
			}else if (++key_time >= 200){     // �������£���ʱ��10ms��10msΪ������ѭ��ִ�м���� 
				key_return = L_key;        // ����ʱ��>1000ms���˰���Ϊ�������������س����¼� 
				key_state = key_state_3;   // ת�����ȴ������ͷ�״̬ 
			} 
			break; 
		case key_state_3:                 // �ȴ������ͷ�״̬����״ֻ̬�����ް����¼� 
			if (key_press) key_state = key_state_0; //�������ͷţ�ת����������ʼ̬ 
        break; 
    } 
	return key_return; 
} 

/*============= 
�м�㰴�������������õͲ㺯��һ�Σ�����˫���¼����жϣ������ϲ���ȷ���޼���������˫��������4�������¼��� 
���������ϲ�ѭ�����ã����10ms 
===============*/ 
uint8_t key_read(void) 
{ 
	static uint8_t key_m = key_state_0, key_time_1 = 0; 
	uint8_t key_return = N_key,key_temp; 
	key_temp = key_driver(); 
	switch(key_m) 
	{ 
		case key_state_0: 
		if (key_temp == S_key ){ 
			key_time_1 = 0;               // ��1�ε����������أ����¸�״̬�жϺ����Ƿ����˫�� 
			key_m = key_state_1; 
		}else 
			key_return = key_temp;        // �����޼�������������ԭ�¼� 
		break; 
		case key_state_1: 
			if (key_temp == S_key){             // ��һ�ε���������϶�<500ms�� 
				key_return = D_key;           // ����˫�����¼����س�ʼ״̬ 
				key_m = key_state_0; 
		}else{                                  // ����500ms�ڿ϶������Ķ����޼��¼�����Ϊ����>1000ms����1sǰ�Ͳ㷵�صĶ����޼� 
			if(++key_time_1 >= 50){ 
				key_return = S_key;      // 500ms��û���ٴγ��ֵ����¼���������һ�εĵ����¼� 
				key_m = key_state_0;     // ���س�ʼ״̬ 
			} 
		} 
		break; 
	}
	return key_return; 
}
void Key_event(void)
{
	
	if(key.time_10ms_ok){  	//time_10ms_ok = 1����ʾ��ʱ����10MS����10MSɨ��һ�ΰ�����
		key.time_10ms_ok = 0; //�����ʱ10MS��־
		key.key = key_read(); //����ɨ�谴�����򣬷���һ����ֵ
		if (key.key == L_key){ //����������P1���ϵ�8��LED�ơ����͵�ƽ������
			A_EN2 = !A_EN2;
			CE = true;
			delay_ms(10);
			CE = false;
			key.Key_Dlay_Enable = true;
			key.key_switch_protection = true;
		}else if(key.key == D_key){//˫��������P1���ϵڶ���LED�ơ����͵�ƽ������  
			LED = !LED;
		}else if(key.key == S_key){//����������P1���ϵ�һ��LED�ơ����͵�ƽ������    
		}		
	}
}