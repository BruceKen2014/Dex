

/*************************************************************
 ****DemoEx��������ϵͳ�����ļ�CSoundSystem.h
 ****��飺CSound�ฺ����Ч����
    	 : CMusic�ฺ�����ִ���
 ****���ߣ�BruceKen
 *************************************************************/

#ifndef _CSOUNDSYSTEM_H
#define	_CSOUNDSYSTEM_H

#include <mmsystem.h>
#include <dsound.h>
#include <dmusicc.h>
#include <dmusici.h>
#include <cguid.h>
#include <string>




typedef struct sWaveHeader
{
  char  RiffSig[4];         
  long  WaveformChunkSize; 
  char  WaveSig[4];      
  char  FormatSig[4];      
  long  FormatChunkSize;    
  short FormatTag;       
  short Channels;       
  long  SampleRate;       
  long  BytesPerSec;       
  short BlockAlign;        
  short BitsPerSample;    
  char  DataSig[4];        
  long  DataSize;           
} sWaveHeader;


class CSound
{
protected:
	static std::string	 m_soundPath; //�����ļ�·�� ��������ʱֻ��Ҫ�����ļ������� ����Ҫ����·��
	LPDIRECTSOUND8       m_pDirectSound;
	LPDIRECTSOUNDBUFFER8 m_pDirectSoundBuffer[4];
	long                 m_volume;                       //ϵͳ��Ч������ȡֵ0~100��������С����
protected:
	bool CreateBuffer(char * filename, int index);		 //����������
	bool CopyFileData(FILE * file, int size, int index); //�����ļ�����
	
public:
	static CSound& getSoundSingleton()
	{
		static CSound g_sound;
		static  char i = 0;
		if( i == 0)
		{
			g_sound.Initial();
			i++;
		}
		return g_sound;
	}
	CSound();
   ~CSound();  
    void ShutDown();
    bool Initial();
    void Play(char * filename, int index = 0, bool loop = false);	  //�������ļ�����
	void Play(int index = 0, bool loop = false);                      //�����Ѿ������ڶ�Ӧ����������Դ
	void Stop(int index = 0);                                         //���ھ���ѭ�����ŵ��ļ������ڱ�Ҫ��ʱ������ֹͣ
	void ReleaseBuffer(int index = 0);								  //�ͷ���Ӧ�Ļ�������Դ
	void SetVolume(long volume)	;								
};

class CMusic
{
public:
	CMusic() {};
	virtual ~CMusic() {};

	virtual void ShutDown() = 0;
	virtual bool Initial(HWND hwnd) = 0;
	virtual void Play(char * filename, bool loop = true) = 0;	  //����һ�����ļ�
	virtual void Play() = 0;									  //�������ŵ�ǰ�ļ�
	virtual void Stop() = 0;
	virtual void SetVolume(long volume) = 0;
};


class CMusicMidi//: public CMusic
{
protected:
	IDirectMusicPerformance8  *m_pDirectMusicPerformance ;
	IDirectMusicLoader8       *m_pDirectMusicLoader      ;
	IDirectMusicSegment8      *m_pDirectMusicSegment     ;
	MUSIC_TIME                 m_time					 ;   //��ͣλ��
	long                       m_volume;
	bool                       m_bStop;             //�Ƿ���ֹͣ����״̬
public:
	static HWND&               m_hwnd;
private:
	CMusicMidi()  ;
	~CMusicMidi() { ShutDown();};

public:
	static CMusicMidi& getMidiSingleton()
	{
		static CMusicMidi g_midi;
		static  char i = 0;
		if( i == 0)
		{
			g_midi.Initial();
			i++;
		}
		return g_midi;
	}

	virtual void ShutDown();
    virtual bool Initial();
    virtual void Play(char * filename, bool loop = true) ;
	virtual void Play() ;
    virtual void Stop();
    virtual void SetVolume(long volume);
	bool IsPlaying() { return !m_bStop;};
};

#endif
