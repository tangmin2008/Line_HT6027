short JudgeYearSaveDay( unsigned char* Source );									//�¹���	//13.11.28
unsigned char GetYearMonSaveMode( unsigned char* YearSaveDay );					//�¹���	//13.11.28	
void MonSaveBarely( void );														//�¹���	//13.11.29
void YearSaveBarely( void );													//�¹���	//13.11.29
void YearMonSaveDaySwitchProcess( unsigned char* OldData, unsigned char* NewData );	//�¹���	//13.11.28	
void YearDec( unsigned char *Data );											//�¹���	//13.11.28
void YearInc( unsigned char *Data );											//�¹���	//13.11.28
void UpdateLastYearSaveTime( unsigned char* YearSaveDayBuff );					//�¹���	//13.11.28
void CreateYearSaveTimeRec( unsigned short EventNo, unsigned char* SaveTime );	//�¹���	//13.11.28
short AdjustYear( unsigned char* YearSaveDayBuff, short* YearSaveCnt );				//�¹���	//13.11.28
void MoveYearSaveDayEC( void );													//�¹���	//13.11.28
unsigned char GetCurYearMonSaveMode( unsigned char* Dest );						//�¹���	//13.11.29
short Year_Save(void);															//�¹���	//13.11.28
