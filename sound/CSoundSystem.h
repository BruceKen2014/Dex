

/*************************************************************
 ****DemoEx引擎声音系统处理文件CSoundSystem.h
 ****简介：CSound类负责音效处理
    	 : CMusic类负责音乐处理
 ****作者：BruceKen
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
	static std::string	 m_soundPath; //声音文件路径 这样播放时只需要传入文件名即可 不需要传入路径
	LPDIRECTSOUND8       m_pDirectSound;
	LPDIRECTSOUNDBUFFER8 m_pDirectSoundBuffer[4];
	long                 m_volume;                       //系统音效音量，取值0~100，音量由小到大
protected:
	bool CreateBuffer(char * filename, int index);		 //创建缓冲区
	bool CopyFileData(FILE * file, int size, int index); //拷贝文件数据
	
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
    void Play(char * filename, int index = 0, bool loop = false);	  //加载新文件播放
	void Play(int index = 0, bool loop = false);                      //播放已经存在于对应缓冲区的资源
	void Stop(int index = 0);                                         //对于决定循环播放的文件可以在必要的时候让其停止
	void ReleaseBuffer(int index = 0);								  //释放相应的缓冲区资源
	void SetVolume(long volume)	;								
};

class CMusic
{
public:
	CMusic() {};
	virtual ~CMusic() {};

	virtual void ShutDown() = 0;
	virtual bool Initial(HWND hwnd) = 0;
	virtual void Play(char * filename, bool loop = true) = 0;	  //播放一个新文件
	virtual void Play() = 0;									  //继续播放当前文件
	virtual void Stop() = 0;
	virtual void SetVolume(long volume) = 0;
};


class CMusicMidi//: public CMusic
{
protected:
	IDirectMusicPerformance8  *m_pDirectMusicPerformance ;
	IDirectMusicLoader8       *m_pDirectMusicLoader      ;
	IDirectMusicSegment8      *m_pDirectMusicSegment     ;
	MUSIC_TIME                 m_time					 ;   //暂停位置
	long                       m_volume;
	bool                       m_bStop;             //是否处于停止播放状态
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
