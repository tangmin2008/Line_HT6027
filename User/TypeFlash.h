


//����FLASH�洢�ռ䣬�ÿռ����ΪһƬ������AT25DF041A��AT26DF161��ǰһ���ֿռ䡣
//ÿһҳΪ4K��4096�ֽڣ�ÿһ�������Ͷ�����ţ�����ռ��2ҳ���ҳ�ռ�
//ÿһ�������ݶ������һҳ�ռ䣬��Ϊ�����ֻ��á�
//��ʷ��������ʷ������˫���ݣ������¼��������ޱ��ݡ�																					���ټ�¼��	
//#define AT7022DataZonePage			1800						// У�����ݴ����(1)		//10.11.02
#define AT7022DataZonePage				FLASH_SECTOR_COUNT			// У�����ݴ����(1)		//10.11.02
#define AT7022DataZonePageBak			AT7022DataZonePage+1			// У�����ݴ�ű�����(1)	//10.11.02

/*		//17.04.15																																//���ټ�¼��	
#define Event_log0_1_Page				AT7022DataZonePageBak+1		//�¼���¼0��¼��1��ҳ��(2)	//ʧѹ (class_id=24) 					����ȫ��Ϊ10��
#define Event_log0_2_Page				Event_log0_1_Page+2*1		//�¼���¼0��¼��2��ҳ��(2)				
#define Event_log0_3_Page				Event_log0_1_Page+2*2		//�¼���¼0��¼��3��ҳ��(2)				
#define Event_log0_4_Page				Event_log0_1_Page+2*3		//�¼���¼0��¼��4��ҳ��(2)				
#define Event_log1_1_Page				Event_log0_1_Page+2*4		//�¼���¼1��¼��1��ҳ��(2)	//Ƿѹ (class_id=24) 					
#define Event_log1_2_Page				Event_log0_1_Page+2*5		//�¼���¼1��¼��2��ҳ��(2)				
#define Event_log1_3_Page				Event_log0_1_Page+2*6		//�¼���¼1��¼��3��ҳ��(2)				
#define Event_log1_4_Page				Event_log0_1_Page+2*7		//�¼���¼1��¼��4��ҳ��(2)				
#define Event_log2_1_Page				Event_log0_1_Page+2*8		//�¼���¼2��¼��1��ҳ��(2)	//��ѹ (class_id=24) 		
#define Event_log2_2_Page				Event_log0_1_Page+2*9		//�¼���¼2��¼��2��ҳ��(2)				
#define Event_log2_3_Page				Event_log0_1_Page+2*10		//�¼���¼2��¼��3��ҳ��(2)				
#define Event_log2_4_Page				Event_log0_1_Page+2*11		//�¼���¼2��¼��4��ҳ��(2)				
#define Event_log3_1_Page				Event_log0_1_Page+2*12		//�¼���¼3��¼��1��ҳ��(2)	//���� (class_id=24) 		
#define Event_log3_2_Page				Event_log0_1_Page+2*13		//�¼���¼3��¼��2��ҳ��(2)				
#define Event_log3_3_Page				Event_log0_1_Page+2*14		//�¼���¼3��¼��3��ҳ��(2)				
#define Event_log3_4_Page				Event_log0_1_Page+2*15		//�¼���¼3��¼��4��ҳ��(2)				
#define Event_log4_1_Page				Event_log0_1_Page+2*16		//�¼���¼4��¼��1��ҳ��(2)	//ʧ�� (class_id=24) 		
#define Event_log4_2_Page				Event_log0_1_Page+2*17		//�¼���¼4��¼��2��ҳ��(2)				
#define Event_log4_3_Page				Event_log0_1_Page+2*18		//�¼���¼4��¼��3��ҳ��(2)				
#define Event_log4_4_Page				Event_log0_1_Page+2*19		//�¼���¼4��¼��4��ҳ��(2)				
#define Event_log5_1_Page				Event_log0_1_Page+2*20		//�¼���¼5��¼��1��ҳ��(2)	//���� (class_id=24) 		
#define Event_log5_2_Page				Event_log0_1_Page+2*21		//�¼���¼5��¼��2��ҳ��(2)				
#define Event_log5_3_Page				Event_log0_1_Page+2*22		//�¼���¼5��¼��3��ҳ��(2)				
#define Event_log5_4_Page				Event_log0_1_Page+2*23		//�¼���¼5��¼��4��ҳ��(2)				
#define Event_log6_1_Page				Event_log0_1_Page+2*24		//�¼���¼6��¼��1��ҳ��(2)	//���� (class_id=24) 		
#define Event_log6_2_Page				Event_log0_1_Page+2*25		//�¼���¼6��¼��2��ҳ��(2)				
#define Event_log6_3_Page				Event_log0_1_Page+2*26		//�¼���¼6��¼��3��ҳ��(2)				
#define Event_log6_4_Page				Event_log0_1_Page+2*27		//�¼���¼6��¼��4��ҳ��(2)				
#define Event_log7_1_Page				Event_log0_1_Page+2*28		//�¼���¼7��¼��1��ҳ��(2)	//�������� (class_id=24) 		
#define Event_log7_2_Page				Event_log0_1_Page+2*29		//�¼���¼7��¼��2��ҳ��(2)				
#define Event_log7_3_Page				Event_log0_1_Page+2*30		//�¼���¼7��¼��3��ҳ��(2)				
#define Event_log7_4_Page				Event_log0_1_Page+2*31		//�¼���¼7��¼��4��ҳ��(2)				
#define Event_log8_1_Page				Event_log0_1_Page+2*32		//�¼���¼8��¼��1��ҳ��(2)	//���� (class_id=24) 		    
#define Event_log8_2_Page				Event_log0_1_Page+2*33		//�¼���¼8��¼��2��ҳ��(2)				
#define Event_log8_3_Page				Event_log0_1_Page+2*34		//�¼���¼8��¼��3��ҳ��(2)				
#define Event_log8_4_Page				Event_log0_1_Page+2*35		//�¼���¼8��¼��4��ҳ��(2)				
#define Event_log9_Page					Event_log0_1_Page+2*36		//�¼���¼9��ҳ��(2)		//�����й��������� (class_id=7) 		    
#define Event_logA_Page					Event_log0_1_Page+2*37		//�¼���¼A��ҳ��(2)		//�����й��������� (class_id=24) 		    
#define Event_logB_1_Page				Event_log0_1_Page+2*38		//�¼���¼B��¼��1��ҳ��(2)	//�޹��������� (class_id=24) 		    
#define Event_logB_2_Page				Event_log0_1_Page+2*39		//�¼���¼B��¼��1��ҳ��(2)	//�޹��������� (class_id=24) 		    
#define Event_logB_3_Page				Event_log0_1_Page+2*40		//�¼���¼B��¼��1��ҳ��(2)	//�޹��������� (class_id=24) 		    
#define Event_logB_4_Page				Event_log0_1_Page+2*41		//�¼���¼B��¼��1��ҳ��(2)	//�޹��������� (class_id=24) 		    
#define Event_logC_Page					Event_log0_1_Page+2*42		//�¼���¼C��ҳ��(2)		//������������ (class_id=7) 		    
#define Event_logD_Page					Event_log0_1_Page+2*43		//�¼���¼D��ҳ��(2)		//ȫʧѹ (class_id=7) 		    
#define Event_logE_Page					Event_log0_1_Page+2*44		//�¼���¼E��ҳ��(2)		//������Դʧѹ (class_id=7) 		    
#define Event_logF_Page					Event_log0_1_Page+2*45		//�¼���¼F��ҳ��(2)		//��ѹ������ (class_id=7) 		    
#define Event_log10_Page				Event_log0_1_Page+2*46		//�¼���¼10��ҳ��(2)		//���������� (class_id=7) 		    
#define Event_log11_Page				Event_log0_1_Page+2*47		//�¼���¼11��ҳ��(2)		//���� (class_id=7) 		    
#define Event_log12_Page				Event_log0_1_Page+2*48		//�¼���¼12��ҳ��(2)		//���ܱ��� (class_id=7) 		    
#define Event_log13_Page				Event_log0_1_Page+2*49		//�¼���¼13��ҳ��(2)		//������� (class_id=7) 		    
#define Event_log14_Page				Event_log0_1_Page+2*50		//�¼���¼14��ҳ��(2)		//�������� (class_id=7) 		    
#define Event_log15_Page				Event_log0_1_Page+2*51		//�¼���¼15��ҳ��(2)		//�¼����� (class_id=7) 		    
#define Event_log16_Page				Event_log0_1_Page+2*52		//�¼���¼16��ҳ��(2)		//Уʱ (class_id=7) 		    
#define Event_log17_Page				Event_log0_1_Page+2*53		//�¼���¼17��ҳ��(7)		//ʱ�α��� (class_id=7) 		    
#define Event_log18_Page				Event_log17_Page+7			//�¼���¼18��ҳ��(2)		//ʱ������ (class_id=7) 		    
#define Event_log19_Page				Event_log18_Page+2*1		//�¼���¼19��ҳ��(2)		//�����ձ�� (class_id=7) 		    
#define Event_log1A_Page				Event_log18_Page+2*2		//�¼���¼1A��ҳ��(2)		//�����ձ�� (class_id=7) 		    
#define Event_log1B_Page				Event_log18_Page+2*3		//�¼���¼1B��ҳ��(2)		//���� (class_id=7) 		    
#define Event_log1C_Page				Event_log18_Page+2*4		//�¼���¼1C��ҳ��(2)		//����ť�� (class_id=7) 		    
#define Event_log1D_Page				Event_log18_Page+2*5		//�¼���¼1D��ҳ��(2)		//��ѹ��ƽ�� (class_id=7) 		    
#define Event_log1E_Page				Event_log18_Page+2*6		//�¼���¼1E��ҳ��(2)		//������ƽ�� (class_id=7) 		    
#define Event_log1F_Page				Event_log18_Page+2*7		//�¼���¼1F��ҳ��(2)		//��բ (class_id=7) 		    
#define Event_log20_Page				Event_log18_Page+2*8		//�¼���¼20��ҳ��(2)		//��բ (class_id=7) 		    
#define Event_log21_Page				Event_log18_Page+2*8		//�¼���¼21��ҳ��(7)		//�ڼ��ձ�� (class_id=7) 		    
#define Event_log22_Page				Event_log21_Page+7			//�¼���¼22��ҳ��(2)		//�й���Ϸ�ʽ��� (class_id=7) 		    
#define Event_log23_Page				Event_log22_Page+2*1		//�¼���¼23��ҳ��(2)		//�޹���Ϸ�ʽ��� (class_id=7) 		    
#define Event_log24_Page				Event_log22_Page+2*2		//�¼���¼24��ҳ��(2)		//���ʲ������ (class_id=7) 		    
#define Event_log25_Page				Event_log22_Page+2*3		//�¼���¼25��ҳ��(2)		//���ݱ��� (class_id=7) 		    
#define Event_log26_Page				Event_log22_Page+2*4		//�¼���¼26��ҳ��(2)		//��Կ���� (class_id=7) 		    
#define Event_log27_Page				Event_log22_Page+2*5		//�¼���¼27��ҳ��(2)		//�쳣�忨 (class_id=7) 		    
#define Event_log28_Page				Event_log22_Page+2*6		//�¼���¼28��ҳ��(2)		//�����¼ (class_id=7) 		    
#define Event_log29_Page				Event_log22_Page+2*7		//�¼���¼29��ҳ��(2)		//�˷Ѽ�¼ (class_id=7) 		    
#define Event_log2A_Page				Event_log22_Page+2*8		//�¼���¼2A��ҳ��(2)		//�㶨�ų����� (class_id=7) 		    
#define Event_log2B_Page				Event_log22_Page+2*9		//�¼���¼2B��ҳ��(2)		//���ɿ������� (class_id=7) 		    
#define Event_log2C_Page				Event_log22_Page+2*10		//�¼���¼2C��ҳ��(2)		//��Դ�쳣 (class_id=7) 		    
#define Event_log2D_Page				Event_log22_Page+2*11		//�¼���¼2D��ҳ��(2)		//�������ز�ƽ�� (class_id=7) 		    
#define Event_log2E_Page				Event_log22_Page+2*12		//�¼���¼2E��ҳ��(2)		//ʱ�ӹ��� (class_id=7) 		    
#define Event_log2F_Page				Event_log22_Page+2*13		//�¼���¼2F��ҳ��(2)		//����оƬ���� (class_id=7) 		    
#define Event_log30_Page				Event_log22_Page+2*14		//�¼���¼30��ҳ��(2)		//ͨѶģ���� (class_id=7) 		    
*/

//17.04.15
#define Event_log0_1_Page				AT7022DataZonePageBak+1		//�¼���¼0��¼��1��ҳ��(3)	//ʧѹ (class_id=24) 					����ȫ��Ϊ10��
#define Event_log0_2_Page				Event_log0_1_Page+3*1		//�¼���¼0��¼��2��ҳ��(3)				
#define Event_log0_3_Page				Event_log0_1_Page+3*2		//�¼���¼0��¼��3��ҳ��(3)				
#define Event_log0_4_Page				Event_log0_1_Page+3*3		//�¼���¼0��¼��4��ҳ��(3)				
#define Event_log1_1_Page				Event_log0_1_Page+3*4		//�¼���¼1��¼��1��ҳ��(3)	//Ƿѹ (class_id=24) 					
#define Event_log1_2_Page				Event_log0_1_Page+3*5		//�¼���¼1��¼��2��ҳ��(3)				
#define Event_log1_3_Page				Event_log0_1_Page+3*6		//�¼���¼1��¼��3��ҳ��(3)				
#define Event_log1_4_Page				Event_log0_1_Page+3*7		//�¼���¼1��¼��4��ҳ��(3)				
#define Event_log2_1_Page				Event_log0_1_Page+3*8		//�¼���¼2��¼��1��ҳ��(3)	//��ѹ (class_id=24) 		
#define Event_log2_2_Page				Event_log0_1_Page+3*9		//�¼���¼2��¼��2��ҳ��(3)				
#define Event_log2_3_Page				Event_log0_1_Page+3*10		//�¼���¼2��¼��3��ҳ��(3)				
#define Event_log2_4_Page				Event_log0_1_Page+3*11		//�¼���¼2��¼��4��ҳ��(3)				
#define Event_log3_1_Page				Event_log0_1_Page+3*12		//�¼���¼3��¼��1��ҳ��(3)	//���� (class_id=24) 		
#define Event_log3_2_Page				Event_log0_1_Page+3*13		//�¼���¼3��¼��2��ҳ��(3)				
#define Event_log3_3_Page				Event_log0_1_Page+3*14		//�¼���¼3��¼��3��ҳ��(3)				
#define Event_log3_4_Page				Event_log0_1_Page+3*15		//�¼���¼3��¼��4��ҳ��(3)				
#define Event_log4_1_Page				Event_log0_1_Page+3*16		//�¼���¼4��¼��1��ҳ��(3)	//ʧ�� (class_id=24) 		
#define Event_log4_2_Page				Event_log0_1_Page+3*17		//�¼���¼4��¼��2��ҳ��(3)				
#define Event_log4_3_Page				Event_log0_1_Page+3*18		//�¼���¼4��¼��3��ҳ��(3)				
#define Event_log4_4_Page				Event_log0_1_Page+3*19		//�¼���¼4��¼��4��ҳ��(3)				
#define Event_log5_1_Page				Event_log0_1_Page+3*20		//�¼���¼5��¼��1��ҳ��(3)	//���� (class_id=24) 		
#define Event_log5_2_Page				Event_log0_1_Page+3*21		//�¼���¼5��¼��2��ҳ��(3)				
#define Event_log5_3_Page				Event_log0_1_Page+3*22		//�¼���¼5��¼��3��ҳ��(3)				
#define Event_log5_4_Page				Event_log0_1_Page+3*23		//�¼���¼5��¼��4��ҳ��(3)				
#define Event_log6_1_Page				Event_log0_1_Page+3*24		//�¼���¼6��¼��1��ҳ��(3)	//���� (class_id=24) 		
#define Event_log6_2_Page				Event_log0_1_Page+3*25		//�¼���¼6��¼��2��ҳ��(3)				
#define Event_log6_3_Page				Event_log0_1_Page+3*26		//�¼���¼6��¼��3��ҳ��(3)				
#define Event_log6_4_Page				Event_log0_1_Page+3*27		//�¼���¼6��¼��4��ҳ��(3)				
#define Event_log7_1_Page				Event_log0_1_Page+3*28		//�¼���¼7��¼��1��ҳ��(3)	//�������� (class_id=24) 		
#define Event_log7_2_Page				Event_log0_1_Page+3*29		//�¼���¼7��¼��2��ҳ��(3)				
#define Event_log7_3_Page				Event_log0_1_Page+3*30		//�¼���¼7��¼��3��ҳ��(3)				
#define Event_log7_4_Page				Event_log0_1_Page+3*31		//�¼���¼7��¼��4��ҳ��(3)				
#define Event_log8_1_Page				Event_log0_1_Page+3*32		//�¼���¼8��¼��1��ҳ��(3)	//���� (class_id=24) 		    
#define Event_log8_2_Page				Event_log0_1_Page+3*33		//�¼���¼8��¼��2��ҳ��(3)				
#define Event_log8_3_Page				Event_log0_1_Page+3*34		//�¼���¼8��¼��3��ҳ��(3)				
#define Event_log8_4_Page				Event_log0_1_Page+3*35		//�¼���¼8��¼��4��ҳ��(3)				
#define Event_log9_Page					Event_log0_1_Page+3*36		//�¼���¼9��ҳ��(2)		//�����й��������� (class_id=7) 		    
#define Event_logA_Page					Event_log9_Page+2*1			//�¼���¼A��ҳ��(2)		//�����й��������� (class_id=24) 		    
#define Event_logB_1_Page				Event_log9_Page+2*2			//�¼���¼B��¼��1��ҳ��(2)	//�޹��������� (class_id=24) 		    
#define Event_logB_2_Page				Event_log9_Page+2*3			//�¼���¼B��¼��1��ҳ��(2)	//�޹��������� (class_id=24) 		    
#define Event_logB_3_Page				Event_log9_Page+2*4			//�¼���¼B��¼��1��ҳ��(2)	//�޹��������� (class_id=24) 		    
#define Event_logB_4_Page				Event_log9_Page+2*5			//�¼���¼B��¼��1��ҳ��(2)	//�޹��������� (class_id=24) 		    
#define Event_logC_Page					Event_log9_Page+2*6			//�¼���¼C��ҳ��(2)		//������������ (class_id=7) 		    
#define Event_logD_Page					Event_log9_Page+2*7			//�¼���¼D��ҳ��(2)		//ȫʧѹ (class_id=7) 		    
#define Event_logE_Page					Event_log9_Page+2*8			//�¼���¼E��ҳ��(2)		//������Դʧѹ (class_id=7) 		    
#define Event_logF_Page					Event_log9_Page+2*9			//�¼���¼F��ҳ��(3)		//��ѹ������ (class_id=7) 		    
#define Event_log10_Page				Event_logF_Page+3*1			//�¼���¼10��ҳ��(3)		//���������� (class_id=7) 		    
#define Event_log11_Page				Event_logF_Page+3*2			//�¼���¼11��ҳ��(2)		//���� (class_id=7) 		    
#define Event_log12_Page				Event_log11_Page+2*1		//�¼���¼12��ҳ��(2)		//���ܱ��� (class_id=7) 		    
#define Event_log13_Page				Event_log11_Page+2*2		//�¼���¼13��ҳ��(3)		//������� (class_id=7) 		    
#define Event_log14_Page				Event_log13_Page+3*1		//�¼���¼14��ҳ��(3)		//�������� (class_id=7) 		    
#define Event_log15_Page				Event_log13_Page+3*2		//�¼���¼15��ҳ��(2)		//�¼����� (class_id=7) 		    
#define Event_log16_Page				Event_log15_Page+2*1		//�¼���¼16��ҳ��(2)		//Уʱ (class_id=7) 		    
#define Event_log17_Page				Event_log15_Page+2*2		//�¼���¼17��ҳ��(2)		//ʱ�α��� (class_id=7) 		    
#define Event_log18_Page				Event_log15_Page+2*3		//�¼���¼18��ҳ��(2)		//ʱ������ (class_id=7) 		    
#define Event_log19_Page				Event_log15_Page+2*4		//�¼���¼19��ҳ��(2)		//�����ձ�� (class_id=7) 		    
#define Event_log1A_Page				Event_log15_Page+2*5		//�¼���¼1A��ҳ��(2)		//�����ձ�� (class_id=7) 		    
#define Event_log1B_Page				Event_log15_Page+2*6		//�¼���¼1B��ҳ��(2)		//���� (class_id=7) 		    
#define Event_log1C_Page				Event_log15_Page+2*7		//�¼���¼1C��ҳ��(2)		//����ť�� (class_id=7) 		    
#define Event_log1D_Page				Event_log15_Page+2*8		//�¼���¼1D��ҳ��(3)		//��ѹ��ƽ�� (class_id=7) 		    
#define Event_log1E_Page				Event_log1D_Page+3*1		//�¼���¼1E��ҳ��(3)		//������ƽ�� (class_id=7) 		    
#define Event_log1F_Page				Event_log1D_Page+3*2		//�¼���¼1F��ҳ��(2)		//��բ (class_id=7) 		    
#define Event_log20_Page				Event_log1F_Page+2*1		//�¼���¼20��ҳ��(2)		//��բ (class_id=7) 		    
#define Event_log21_Page				Event_log1F_Page+2*2		//�¼���¼21��ҳ��(2)		//�ڼ��ձ�� (class_id=7) 		    
#define Event_log22_Page				Event_log1F_Page+2*3		//�¼���¼22��ҳ��(2)		//�й���Ϸ�ʽ��� (class_id=7) 		    
#define Event_log23_Page				Event_log1F_Page+2*4		//�¼���¼23��ҳ��(2)		//�޹���Ϸ�ʽ��� (class_id=7) 		    
#define Event_log24_Page				Event_log1F_Page+2*5		//�¼���¼24��ҳ��(2)		//���ʲ������ (class_id=7) 		    
#define Event_log25_Page				Event_log1F_Page+2*6		//�¼���¼25��ҳ��(2)		//���ݱ��� (class_id=7) 		    
#define Event_log26_Page				Event_log1F_Page+2*7		//�¼���¼26��ҳ��(2)		//��Կ���� (class_id=7) 		    
#define Event_log27_Page				Event_log1F_Page+2*8		//�¼���¼27��ҳ��(2)		//�쳣�忨 (class_id=7) 		    
#define Event_log28_Page				Event_log1F_Page+2*9		//�¼���¼28��ҳ��(2)		//�����¼ (class_id=7) 		    
#define Event_log29_Page				Event_log1F_Page+2*10		//�¼���¼29��ҳ��(2)		//�˷Ѽ�¼ (class_id=7) 		    
#define Event_log2A_Page				Event_log1F_Page+2*11		//�¼���¼2A��ҳ��(2)		//�㶨�ų����� (class_id=7) 		    
#define Event_log2B_Page				Event_log1F_Page+2*12		//�¼���¼2B��ҳ��(2)		//���ɿ������� (class_id=7) 		    
#define Event_log2C_Page				Event_log1F_Page+2*13		//�¼���¼2C��ҳ��(2)		//��Դ�쳣 (class_id=7) 		    
#define Event_log2D_Page				Event_log1F_Page+2*14		//�¼���¼2D��ҳ��(3)		//�������ز�ƽ�� (class_id=7) 		    
#define Event_log2E_Page				Event_log2D_Page+3*1		//�¼���¼2E��ҳ��(2)		//ʱ�ӹ��� (class_id=7) 		    
#define Event_log2F_Page				Event_log2E_Page+2*1		//�¼���¼2F��ҳ��(2)		//����оƬ���� (class_id=7) 		    
#define Event_log30_Page				Event_log2F_Page+2*2		//�¼���¼30��ҳ��(2)		//ͨѶģ���� (class_id=7) 		    









//#define L1PhAFailRecPage			L1Mon_MD_Pp0_PageBak+4			//��10��A��ʧѹ��¼��ҳ��(2)
//#define L1PhBFailRecPage			L1PhAFailRecPage+2*1			//��10��B��ʧѹ��¼��ҳ��(2)
//#define L1PhCFailRecPage			L1PhAFailRecPage+2*2			//��10��C��ʧѹ��¼��ҳ��(2)
//
//#define L1PhALowerRecPage			L1PhCFailRecPage+2				//��10��A��Ƿѹ��¼��ҳ��(2)
//#define L1PhBLowerRecPage			L1PhALowerRecPage+2*1			//��10��B��Ƿѹ��¼��ҳ��(2)
//#define L1PhCLowerRecPage			L1PhALowerRecPage+2*2			//��10��C��Ƿѹ��¼��ҳ��(2)
//
//#define L1PhAOverRecPage			L1PhCLowerRecPage+2				//��10��A���ѹ��¼��ҳ��(2)
//#define L1PhBOverRecPage			L1PhAOverRecPage+2*1			//��10��B���ѹ��¼��ҳ��(2)
//#define L1PhCOverRecPage			L1PhAOverRecPage+2*2			//��10��C���ѹ��¼��ҳ��(2)
//
//#define L1PhABreakRecPage			L1PhCOverRecPage+2				//��10��A������¼��ҳ��(2)
//#define L1PhBBreakRecPage			L1PhABreakRecPage+2*1			//��10��B������¼��ҳ��(2)
//#define L1PhCBreakRecPage			L1PhABreakRecPage+2*2			//��10��C������¼��ҳ��(2)
//
//#define L1VoltRevPhaseRecPage		L1PhCBreakRecPage+2				//��10�ε�ѹ�������¼��ҳ��(2)
//#define L1CuRevPhaseRecPage			L1VoltRevPhaseRecPage+2			//��10�ε����������¼��ҳ��(2)
//#define L1VoltImbalanceRecPage		L1CuRevPhaseRecPage+2			//��10�ε�ѹ��ƽ���¼��ҳ��(2)
//#define L1CuImbalanceRecPage		L1VoltImbalanceRecPage+2		//��10�ε�����ƽ���¼��ҳ��(2)
//#define L1CuBestImbalanceRecPage	L1CuImbalanceRecPage+2			//��10�ε������ز�ƽ���¼��ҳ��(2)
//
//#define L1CuAFailRecPage			L1CuBestImbalanceRecPage+2		//��10��A��ʧ����¼��ҳ��(2)
//#define L1CuBFailRecPage			L1CuAFailRecPage+2*1			//��10��B��ʧ����¼��ҳ��(2)
//#define L1CuCFailRecPage			L1CuAFailRecPage+2*2			//��10��C��ʧ����¼��ҳ��(2)
//
//#define L1CuAOverRecPage			L1CuCFailRecPage+2				//��10��A�������¼��ҳ��(2)
//#define L1CuBOverRecPage			L1CuAOverRecPage+2*1			//��10��B�������¼��ҳ��(2)
//#define L1CuCOverRecPage			L1CuAOverRecPage+2*2			//��10��C�������¼��ҳ��(2)
//
//#define L1CuABreakRecPage			L1CuCOverRecPage+2				//��10��A�������¼��ҳ��(2)
//#define L1CuBBreakRecPage			L1CuABreakRecPage+2*1			//��10��B�������¼��ҳ��(2)
//#define L1CuCBreakRecPage			L1CuABreakRecPage+2*2			//��10��C�������¼��ҳ��(2)
//
//#define L1RevPowerARecPage			L1CuCBreakRecPage+2				//��10��A�ೱ�������¼��ҳ��(2)
//#define L1RevPowerBRecPage			L1RevPowerARecPage+2*1			//��10��B�ೱ�������¼��ҳ��(2)
//#define L1RevPowerCRecPage			L1RevPowerARecPage+2*2			//��10��C�ೱ�������¼��ҳ��(2)
//
//#define L1PowerOverARecPage			L1RevPowerCRecPage+2			//��10��A����ؼ�¼��ҳ��(2)
//#define L1PowerOverBRecPage			L1PowerOverARecPage+2*1			//��10��B����ؼ�¼��ҳ��(2)
//#define L1PowerOverCRecPage			L1PowerOverARecPage+2*2			//��10��C����ؼ�¼��ҳ��(2)
//
//#define L1ECClrRecPage				L1PowerOverCRecPage+2			//��10�ε��������¼��ҳ��(4)
//#define L1MDClrRecPage				L1ECClrRecPage+4				//��10�����������¼��ҳ��(4)
//
//#define L1SegTabPrgRecPage			L1MDClrRecPage+4				//��10��ʱ�α��̼�¼��ҳ��(4)
//#define L1ZonePrgRecPage			L1SegTabPrgRecPage+4			//��10��ʱ�����̼�¼��ҳ��(2)
//#define L1HolidayPrgRecPage			L1ZonePrgRecPage+2				//��10�νڼ��ձ�̼�¼��ҳ��(5)
//
////#define L1OpenCoverRecPage			L1HolidayPrgRecPage+5			//��10�ο���Ǽ�¼��ҳ��(2)
////#define L1OpenTerminalRecPage		L1OpenCoverRecPage+2			//��10�ο���ť�Ǽ�¼��ҳ��(2)
//
//#define L1AppointFreezeRecPage		L1HolidayPrgRecPage+5			//��60�ζ�ʱ�����¼��ҳ��(31)
//#define L1InstantFreezeRecPage		L1AppointFreezeRecPage+31		//��3��˲ʱ�����¼��ҳ��(3)
//#define L1ZoneSWFreezeRecPage		L1InstantFreezeRecPage+3		//��2��ʱ���л������¼��ҳ��(2)
//#define L1SegTabSWFreezeRecPage		L1ZoneSWFreezeRecPage+2			//��2��ʱ�α��л������¼��ҳ��(2)
//#define L1FeeSWFreezeRecPage		L1SegTabSWFreezeRecPage+2		//��2�η����л������¼��ҳ��(2)
//#define L1StepSWFreezeRecPage		L1FeeSWFreezeRecPage+2			//��2�ν����л������¼��ҳ��(2) 				
//#define L1HourFreezeRecPage			L1StepSWFreezeRecPage+2			//��96�����㶳���¼��ҳ��(2) 				
//#define L1DayFreezeRecPage			L1HourFreezeRecPage+2			//��62���ն����¼��ҳ��(32) 				
//
//#if ( NWProtocol ==	YesCheck )								//11.09.14			
//#define L1AllFailRecPage			L1DayFreezeRecPage+32			//��10��ȫʧѹ��¼��ҳ��(2)
//#define L1PowerDownRecPage			L1AllFailRecPage+2				//��10�ε����������¼��ҳ��(2)
//
//#define L1ECErrorRecPage			L1PowerDownRecPage+2			//��10�ε����ݴ��¼���¼��ҳ��(3)
//#define L1MDErrorRecPage			L1ECErrorRecPage+3				//��10�������ݴ��¼���¼��ҳ��(3)
//
//#else
//
//#define L1ECErrorRecPage			L1DayFreezeRecPage+32			//��10�ε����ݴ��¼���¼��ҳ��(3)
//#define L1MDErrorRecPage			L1ECErrorRecPage+3				//��10�������ݴ��¼���¼��ҳ��(3)
//
//#endif

/*
//FLASH�е���ʷ��������������������ǰ��������������ǰ������
//��ʱ���ᡢ˲ʱ���ᡢʱ����ʱ�ζ���ĵ�������������ͬEEPROM�ж���ĵ�����������ʽ�������ݾ���У��͡�
//���¶��嶳�������еģ���ѹ�����������޹����ڹ��ʡ��������ء���ǡ����ߵ���������Ƶ�ʣ�����У��
#define FreezeTime		0x0					//����ʱ��(5)

#define FreezeUa		0x0					//A���ѹ(2)
#define FreezeUb		FreezeUa+2*1		//B���ѹ(2)
#define FreezeUc		FreezeUa+2*2		//C���ѹ(2)

#define FreezeIa		FreezeUc+2			//A�����(3)
#define FreezeIb		FreezeIa+3*1		//B�����(3)
#define FreezeIc		FreezeIa+3*2		//C�����(3)
#define FreezeIt		FreezeIa+3*3		//N�����(3)

#define FreezePt		FreezeIt+3			//���й�����(3)
#define FreezePa		FreezePt+3*1		//A���й�����(3)
#define FreezePb		FreezePt+3*2		//B���й�����(3)
#define FreezePc		FreezePt+3*3		//C���й�����(3)

#define FreezeQt		FreezePc+3			//���޹�����(3)
#define FreezeQa		FreezeQt+3*1		//A���޹�����(3)
#define FreezeQb		FreezeQt+3*2		//B���޹�����(3)
#define FreezeQc		FreezeQt+3*3		//C���޹�����(3)

#define FreezeSt		FreezeQc+3			//�����ڹ���(3)
#define FreezeSa		FreezeSt+3*1		//A�����ڹ���(3)
#define FreezeSb		FreezeSt+3*2		//B�����ڹ���(3)
#define FreezeSc		FreezeSt+3*3		//C�����ڹ���(3)

#define FreezePft		FreezeSc+3			//�ܹ�������(2)
#define FreezePfa		FreezePft+2*1		//A�๦������(2)
#define FreezePfb		FreezePft+2*2		//B�๦������(2)
#define FreezePfc		FreezePft+2*3		//C�๦������(2)

#define FreezeAngleA	FreezePfc+2			//A�����(2)
#define FreezeAngleB	FreezeAngleA+2*1	//B�����(2)
#define FreezeAngleC	FreezeAngleA+2*2	//C�����(2)
#define FreezeAngleZ	FreezeAngleA+2*3	//�����(2)

#define FreezeFreq		FreezeAngleZ+2		//Ƶ��(2)
*/





