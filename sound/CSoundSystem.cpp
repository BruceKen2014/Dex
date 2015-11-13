

#define INITGUID

//#include "stdafx.h"
#include "../DexBase/typedefine.h"
#include "CSoundSystem.h"
#include "../state/DexGameEngine.h"


std::string CSound::m_soundPath = "sound/";
CSound::CSound()
{
	ZeroMemory(m_pDirectSoundBuffer, sizeof(LPDIRECTSOUNDBUFFER8) * 4);
	m_pDirectSound = NULL;
	m_volume = 0;                     //0为原有音量，即最大音量
}

CSound::~CSound()
{
	ShutDown();
}
void CSound::ShutDown()
{
	for (int i = 0; i < 4; i++)
		if(m_pDirectSoundBuffer[i])
		{
			m_pDirectSoundBuffer[i]->Release();
			m_pDirectSoundBuffer[i] = NULL;
		}
	if(m_pDirectSound)
	{
		m_pDirectSound->Release();
		m_pDirectSound = NULL;
	}
}

bool CSound::CreateBuffer(char *filename, int index)
{
    FILE *fp;
    if((fp=fopen(filename, "rb"))==NULL)
	{
		_Message("文件%s打开失败!信息：CSound::CreateBuffer(char *filename, int index)", filename);
        return false;
	}
    LPDIRECTSOUNDBUFFER pDirectBuffer;

	sWaveHeader header;
    fseek(fp, 0, SEEK_SET);
    fread(&header, 1, sizeof(sWaveHeader), fp);
    if(memcmp(header.RiffSig, "RIFF", 4)   ||                     
	   memcmp(header.WaveSig, "WAVE", 4)   ||                      
	   memcmp(header.FormatSig, "fmt ", 4) ||                   
	   memcmp(header.DataSig, "data",4))
	{
		_Message("sWaveHeader信息操作错误!信息：CSound::CreateBuffer(char *filename, int index)");
        return false;
	} 
    DSBUFFERDESC dsbd;
    WAVEFORMATEX wfex;

    ZeroMemory(&wfex, sizeof(WAVEFORMATEX));
    wfex.wFormatTag = WAVE_FORMAT_PCM;
    wfex.nChannels = header.Channels;
    wfex.nSamplesPerSec = header.SampleRate;
    wfex.wBitsPerSample = header.BitsPerSample;
    wfex.nBlockAlign = wfex.wBitsPerSample / 8 * wfex.nChannels;
    wfex.nAvgBytesPerSec = wfex.nSamplesPerSec * wfex.nBlockAlign;

    ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
    dsbd.dwSize = sizeof(DSBUFFERDESC);
    dsbd.dwFlags = DSBCAPS_CTRLVOLUME;
    dsbd.dwBufferBytes = header.DataSize;
    dsbd.lpwfxFormat = &wfex;
    if(FAILED(m_pDirectSound->CreateSoundBuffer(&dsbd, &pDirectBuffer, NULL)))
	{
		_Message("创建缓冲区失败!信息：CSound::CreateBuffer(char *filename, int index)");
        return false;
	} 
    if(FAILED(pDirectBuffer->QueryInterface(IID_IDirectSoundBuffer8,     
				   (void**)&m_pDirectSoundBuffer[index]))) 
	{
		_Message("缓冲区接口查询失败!信息：CSound::CreateBuffer(char *filename, int index)");
        return false;
	} 
    pDirectBuffer->Release();		 //释放，防止内存泄露
	if(!CopyFileData(fp, header.DataSize, index))
		return false;
	fclose(fp);			             //关闭文件
	return true;
}		
	
bool CSound::CopyFileData(FILE * file, int size, int index)
{
    BYTE *Ptr1, *Ptr2;
    DWORD Size1, Size2;
    if(!size)
	{
		_Message("波形文件大小有误!信息：CSound::CopyFileData(FILE * file, int size, int index)");
        return false;
	} 
    if(FAILED(m_pDirectSoundBuffer[index]->Lock(0, size, (void**)&Ptr1, &Size1, (void**)&Ptr2, &Size2, 0)))
        return false;

    fread(Ptr1, 1, Size1, file);
    if(Ptr2 != NULL)
      fread(Ptr2, 1, Size2, file);
    m_pDirectSoundBuffer[index]->Unlock(Ptr1, Size1, Ptr2, Size2);
    return true;
}

bool CSound::Initial()
{
  if(FAILED(DirectSoundCreate8(NULL, &m_pDirectSound, NULL)))
  {
      MessageBox(NULL, "Unable to create DirectSound object", "Error", MB_OK);
      return false;
  }
  m_pDirectSound->SetCooperativeLevel(DexGameEngine::getEngine()->GetHwnd(), DSSCL_NORMAL);
  return true;
}
void CSound::Play(char *filename, int index, bool loop)
{
	string name = m_soundPath + string(filename);
	ReleaseBuffer(index);                          //必须释放之前的资源，否则会出现内存泄露
	if(!CreateBuffer((char*)name.c_str(), index))			   //申请缓冲区资源
		return;
	DWORD flag;
	if(loop)
		flag = DSBPLAY_LOOPING;	
	else 
		flag = 0;
	m_pDirectSoundBuffer[index]->SetCurrentPosition(0);
	m_pDirectSoundBuffer[index]->SetVolume(m_volume);
	m_pDirectSoundBuffer[index]->Play(0, 0, flag);
}

void CSound::Play(int index, bool loop)
{
	if(m_pDirectSoundBuffer[index] == NULL)
		return;
	DWORD flag;
	if(loop)
		flag = DSBPLAY_LOOPING;	
	else 
		flag = 0;
	m_pDirectSoundBuffer[index]->SetCurrentPosition(0);
	m_pDirectSoundBuffer[index]->SetVolume(m_volume);
	m_pDirectSoundBuffer[index]->Play(0, 0, flag);
}

void CSound::SetVolume( long volume )
{
	if(volume > 100 || volume < 0)
		return;
	m_volume = volume * 100 - 10000;      //从100~0到0~-10000的数值转换
}

void CSound::Stop( int index )
{
	if(m_pDirectSoundBuffer[index] == NULL)
		return;
	m_pDirectSoundBuffer[index]->Stop();
}

void CSound::ReleaseBuffer( int index )
{
	if(m_pDirectSoundBuffer[index] == NULL)
		return;
	m_pDirectSoundBuffer[index]->Release();   
	m_pDirectSoundBuffer[index] = NULL;
}
CMusicMidi::CMusicMidi()
{
	m_bStop = true;
}

bool CMusicMidi::Initial( )
{	
	m_pDirectMusicPerformance = NULL;
	m_pDirectMusicLoader      = NULL;
	m_pDirectMusicSegment     = NULL;
	m_time                    = 0;
	m_volume                  = -200;
	CoInitialize(0);      //初始化COM
    CoCreateInstance(CLSID_DirectMusicLoader, NULL, CLSCTX_INPROC, 
                   IID_IDirectMusicLoader8, (void**)&m_pDirectMusicLoader);

    CoCreateInstance(CLSID_DirectMusicPerformance, NULL, CLSCTX_INPROC, 
                   IID_IDirectMusicPerformance8, (void**)&m_pDirectMusicPerformance);

    m_pDirectMusicPerformance->InitAudio(NULL, NULL, DexGameEngine::getEngine()->GetHwnd(),
                              DMUS_APATH_SHARED_STEREOPLUSREVERB, 128,
                              DMUS_AUDIOF_ALL , NULL);
    char strPath[MAX_PATH];
    WCHAR wstrSearchPath[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, strPath);
    MultiByteToWideChar(CP_ACP, 0, strPath, -1, wstrSearchPath, MAX_PATH);
    m_pDirectMusicLoader->SetSearchDirectory(GUID_DirectMusicAllTypes, wstrSearchPath, FALSE);
	return true;
}

void CMusicMidi::Play(char *filename, bool loop)
{
	if(m_pDirectMusicSegment != NULL)
	{
		m_pDirectMusicSegment->Release();   //如果之前有内容则释放，避免内存泄露
	    m_pDirectMusicSegment = NULL;
		m_pDirectMusicLoader->ClearCache(GUID_DirectMusicAllTypes);	   //清除缓存
	} 
	if(m_pDirectMusicPerformance->IsPlaying(m_pDirectMusicSegment, NULL))	 //如果之前在播放则停止，否则会出现噪音杂音,
		m_pDirectMusicPerformance->StopEx(m_pDirectMusicSegment, 0, 0);	 //本操作必须在g_pDMSegment释放之后进行，否则会有杂音

    DMUS_OBJECTDESC dmod;

    ZeroMemory(&dmod, sizeof(DMUS_OBJECTDESC));
    dmod.dwSize = sizeof(DMUS_OBJECTDESC);
    dmod.guidClass = CLSID_DirectMusicSegment;
    dmod.dwValidData = DMUS_OBJ_CLASS | DMUS_OBJ_FILENAME | DMUS_OBJ_FULLPATH ;
    mbstowcs(dmod.wszFileName, filename, MAX_PATH);
    if(FAILED(m_pDirectMusicLoader->GetObject(&dmod, IID_IDirectMusicSegment8, (LPVOID*)&m_pDirectMusicSegment)))
	{
		_Message("m_pDirectMusicLoader调用失败!信息：CMusicMidi::Play(char *filename, bool loop)");
        return ;
	} 

    if(strstr(filename, ".mid") != NULL)
        if(FAILED(m_pDirectMusicSegment->SetParam(GUID_StandardMIDIFile, -1, 0, 0, (void *)m_pDirectMusicPerformance)))
		{
			_Message("m_pDirectMusicSegment调用失败!信息：CMusicMidi::Play(char *filename, bool loop)");
			return ;
		}
    m_pDirectMusicSegment->SetParam(GUID_Download, -1, 0, 0, (void *)m_pDirectMusicPerformance);


    if(FAILED(m_pDirectMusicSegment->Download(m_pDirectMusicPerformance)))
	{
		_Message("m_pDirectMusicSegment调用失败!信息：CMusicMidi::Play(char *filename, bool loop)");
        return ;
	} 

	DWORD flag;
	if(loop)
		flag = DMUS_SEG_REPEAT_INFINITE;
	else 
		flag = 0;
    m_pDirectMusicSegment->SetRepeats(flag);

    m_pDirectMusicPerformance->PlaySegmentEx(m_pDirectMusicSegment, NULL, NULL, 0, 0, NULL, NULL, NULL);
	m_bStop = false;
}

void CMusicMidi::Play()
{
	if(m_pDirectMusicPerformance->IsPlaying(m_pDirectMusicSegment, NULL) != S_OK)
	{
		m_pDirectMusicSegment->SetStartPoint(m_time);    //从上次暂停的位置继续播放
		m_pDirectMusicPerformance->PlaySegmentEx(m_pDirectMusicSegment, NULL, NULL, 0, 0, NULL, NULL, NULL);
		m_bStop = false;
	}
}
void CMusicMidi::Stop()
{
	if(m_pDirectMusicPerformance->IsPlaying(m_pDirectMusicSegment, NULL) == S_OK)	//正在播放
	{	
		m_pDirectMusicPerformance->StopEx(m_pDirectMusicSegment, 0, DMUS_SEGF_DEFAULT);
		m_pDirectMusicPerformance->GetTime(NULL, &m_time);	
		m_bStop = true;
	}
}

void CMusicMidi::ShutDown()
{
	if(m_pDirectMusicLoader)
	{
		m_pDirectMusicLoader->ReleaseObjectByUnknown(m_pDirectMusicSegment);
		m_pDirectMusicLoader->ClearCache(GUID_DirectMusicAllTypes);
		m_pDirectMusicLoader->Release();
		m_pDirectMusicLoader = NULL; 
	}
	if(m_pDirectMusicSegment)
	{
		m_pDirectMusicSegment->Unload(m_pDirectMusicPerformance);
		m_pDirectMusicSegment->Release();
		m_pDirectMusicSegment = NULL; 
	}
	if(m_pDirectMusicPerformance)
	{
		m_pDirectMusicPerformance->Release();
		m_pDirectMusicPerformance = NULL; 
	}
	CoUninitialize();

}
void CMusicMidi::SetVolume(long volume)
{
	//m_pDirectMusicPerformance->SetParam(GUID_PerfMasterVolume, sizeof(long), 0, 0, &m_volume );
}
