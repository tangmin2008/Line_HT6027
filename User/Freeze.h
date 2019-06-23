
#define DIRECT			0		//ֱ�Ӷ���
#define INDIRECT		1		//���ݶ������ڶ���


#define Freeze0_CapNUM			30				//��������0��󲶻�����		//˲ʱ���� (class_id=9) 		    
#define Freeze1_CapNUM			30				//��������1��󲶻�����		//�붳�� (class_id=9) 		    
#define Freeze2_CapNUM			30				//��������2��󲶻�����		//���Ӷ��� (class_id=9) 		    
#define Freeze3_CapNUM			30				//��������3��󲶻�����		//Сʱ���� (class_id=9) 		    
#define Freeze4_CapNUM			30				//��������4��󲶻�����		//�ն��� (class_id=9) 		    
#define Freeze5_CapNUM			50				//��������5��󲶻�����		//�����ն��� (class_id=9) 		    
#define Freeze6_CapNUM			50				//��������6��󲶻�����		//�¶��� (class_id=9) 		    
#define Freeze7_CapNUM			30				//��������7��󲶻�����		//�궳�� (class_id=9) 		    
#define Freeze8_CapNUM			30				//��������8��󲶻�����		//ʱ�����л����� (class_id=9) 		    
#define Freeze9_CapNUM			30				//��������9��󲶻�����		//��ʱ�α��л����� (class_id=9) 		    
#define FreezeA_CapNUM			30				//��������A��󲶻�����		//���ʵ���л����� (class_id=9) 		    
#define FreezeB_CapNUM			30				//��������B��󲶻�����		//�����л����� (class_id=9) 		    
#define Freeze11_CapNUM			30				//��������11��󲶻�����	//���ݽ��㶳�� (class_id=9) 		    

short GetRecordFreezeClassData( short COM_No, unsigned char* Dest, unsigned char* Source ); 
short GetRecordFreezeClassDataTab( short COM_No, unsigned char* Dest, unsigned char* Source );
short GetFreezeBufferData( short COM_No, unsigned char* Dest, unsigned char* Source, short ItemNo );
short GetFreezeBufferDataSelector1( short COM_No, unsigned char* Dest, unsigned char* RSD, unsigned char* RCSD, short ItemNo );
short GetFreezeBufferDataSelector9( short COM_No, unsigned char* Dest, unsigned char* RSD, unsigned char* RCSD, short ItemNo );
short GetFreezeBufferDataSelector( short COM_No, unsigned char* Dest, unsigned char* RSD, unsigned char* RCSD, short ItemNo, short SelectType );
short GetFreezeBufferDataSelector2( short COM_No, unsigned char* Dest, unsigned char* RSD, unsigned char* RCSD, short ItemNo );
void Fill_null_data( unsigned char* Dest, unsigned short* GetLen );
void Get_single_freeze_content( unsigned char* DestPtr, unsigned short* GetLen, unsigned char* SelectorData, unsigned char* FreezeAux, unsigned short ItemNo, unsigned short CurveNo, short SelectType );
long GetFreezeLoadPWAds( unsigned char* Dest, FILECURVEPARA* FileCurvePara, unsigned char* Data, short Type );
short FreezeLoadPWJustDay( long LoadRecNo, FILECURVEPARA* FileCurvePara, unsigned char* Data, short Type );

void Get_freeze_First_content( unsigned char* DestPtr, unsigned char* SData, unsigned char* OAD, unsigned char OADItems, unsigned short ItemNo, short SelectType );
short Get_single_freeze_First_content( unsigned char* DestPtr, unsigned char* SData, unsigned char* EData, unsigned short ItemNo, unsigned short CurveNo, short SelectType );
long GetFirstFreezeLoadPWAds( unsigned char* Dest, FILECURVEPARA* FileCurvePara, unsigned char* SData, unsigned char* EData, short Type );
short FirstFreezeLoadPWJustDay( long LoadRecNo, FILECURVEPARA* FileCurvePara, unsigned char* SData, unsigned char* EData, short Type );

//unsigned char Read_File( unsigned char* Dest, FILECURVEPARA* FileCurvePara, long Addr, short Len );
unsigned char Read_File( unsigned char* Dest, FILECURVEPARA* FileCurvePara, long Addr, long Offset, short Len );
short GetFreezeClassData( short COM_No, unsigned char* Dest, unsigned char* Source ); 
unsigned short GetFreezeClassTabItemNo( unsigned char* OAD );
short GetFreezeClassDataTab( short COM_No, unsigned char* Dest, unsigned char* Source );
short GetFreezeCaptureItems( short ProfItemNo );
short Get_sigle_freeze_capture_objectsPara( unsigned char* Dest, unsigned short Addr, unsigned short CaptureItems, unsigned short* Length );
short GetFreeze_capture_objectsData( short COM_No, unsigned char* Dest, short ProfItemNo );
unsigned char GetBakFlag( short ProfileNo, unsigned char* OAD );
//unsigned char Judge_freeze_capture_objects( unsigned char* Data, short ProfileNo, unsigned short* Length, unsigned short* ItemLen );
unsigned char Judge_freeze_capture_objects( unsigned char* Data, short ProfileNo, unsigned short* Length, unsigned short* ItemLen, short Type );
unsigned short GetFreezeHeadLen( unsigned short ItemNo, unsigned char BakFlag );
void CreatFileName( unsigned char* Dest, unsigned short ItemNo, unsigned char* OAD, unsigned char BakFlag );
//void DeleteFreezeFile( unsigned short ItemNo, unsigned char* OAD );
void DeleteFreezeFile( unsigned short ItemNo, unsigned short CurveNo, unsigned char* OAD );		//16.10.12
void DeleteFreezeProfileFile( unsigned short ItemNo );
unsigned char DeleteFreezeCapture_OAD( unsigned short ItemNo, unsigned char* OAD );
unsigned char OADIsAlreadyExist( unsigned short ItemNo, unsigned char* OAD );
unsigned char AddFreezeCapture_OAD( unsigned short ItemNo, unsigned char* Source );
unsigned char InitialFreezeProfilePara( unsigned short ItemNo, unsigned short CurveNo, unsigned char* Source, unsigned short ItemLen );
unsigned char SetFreezeClassData( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen ); 
unsigned char SetFreezeClassDataTab( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen ); 
unsigned char ActionFreezeClassData( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen, unsigned short* BackLen ); 
unsigned char ActionFreezeClassDataTab( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen, unsigned short* BackLen ); 
void GetFileCurvePara( FILECURVEPARA* FileCurvePara );
long LoadFilePtrChk( FILECURVEPARA* FileCurvePara, long Type );
unsigned short GetFreezeSpace( short ChannelNo );
unsigned char AllowFreeze( short ChannelNo, short Type );
unsigned char RecordSingleFreeze( short ChannelNo, unsigned long FinalRecNo, unsigned char* FreezeTime, short Type );
void RecordFreeze( short ItemNo, short Type );
short CreatFileCurveData( unsigned char* Dest, FILECURVEPARA* FileCurvePara, unsigned long FinalRecNo, unsigned char* FreezeTime );

long GetFileRecRealFlashAddr( FILECURVEPARA* FileCurvePara, long CurRecNo );
unsigned char JudgeSearchTimeout( unsigned long StartTime );

void ResetFreezeMD( short ChannelNo );

#if( JiangSuRequire == YesCheck )			//����Ҫ��
short Get_single_freeze_time( unsigned char* DestPtr, unsigned short ItemNo, unsigned short CurveNo, long LoadRecNo );
short GetMinFreezeTimeSpace( short ItemNo, unsigned short* Space, unsigned char* FreezeTime );
void GetSecondTime( unsigned char* Dest, unsigned char* Source, unsigned short Space );
short JudgeComplementMinFreeze( unsigned char* FirstTime, unsigned char* SecondTime );				
void ComplementRecordMinFreeze( void );
#endif











